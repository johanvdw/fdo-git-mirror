// 
//  
//  Copyright (C) 2008 Autodesk Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include "SltCapabilities.h"
#include "SQLiteProvider.h"

class SltMetadata;
class SpatialIndex;
class SltReader;
class RowidIterator;
class SltExtendedSelect;
struct NameOrderingPair;
class StringBuffer;
struct DBounds;

#include "SpatialIndexDescriptor.h"

// on read connection only the provider can open (internal) transactions
enum SQLiteActiveTransactionType
{
    SQLiteActiveTransactionType_None = 0,     // no transaction started
    SQLiteActiveTransactionType_Internal = 1, // internal transaction started on write connection
    SQLiteActiveTransactionType_User = 2      // user transaction started on write connection
};

struct string_less
{	// functor for operator<
bool operator()(const char* _Left, const char* _Right) const
	{	// apply operator< to operands
	    return strcmp(_Left, _Right) < 0;
	}
};

struct wstring_less
{	// functor for operator<
bool operator()(FdoString* _Left, FdoString* _Right) const
	{	// apply operator< to operands
	    return wcscmp(_Left, _Right) < 0;
	}
};

// about usage more details above QueryCacheRecInfo::m_usageCount
enum SQLiteClearActionType
{
    SQLiteClearActionType_All = 0,     // we clean up everything
    SQLiteClearActionType_ReleaseUsage1 = 1, // we clean up only statements with usage = 1
    SQLiteClearActionType_RelUsage1DecOthers = 2  // we clean up only with usage = 1 and we decrement usage for others
};

struct QueryCacheRec
{
    QueryCacheRec() : stmt(NULL), inUse(false) {}
    QueryCacheRec(sqlite3_stmt* s) : stmt(s), inUse(true) {}

    sqlite3_stmt* stmt;
    bool inUse;
};

typedef std::vector<QueryCacheRec> QueryCacheRecList;
struct QueryCacheRecInfo
{
    QueryCacheRecInfo() : m_usageCount(1), m_usedStmt(0) {}

    // this value is incremented each time a statement is used.
    // in case a statement is only once used, next time we call clean it will be removed.
    FdoInt64 m_usageCount;
    FdoInt32 m_usedStmt;
    QueryCacheRecList m_lst;
};

typedef std::map<char*, QueryCacheRecInfo*, string_less> QueryCache;

typedef std::map<char*, SltMetadata*, string_less> MetadataCache;

typedef std::map<FdoString*, FdoUniqueConstraint*, wstring_less> UniqueConstraints;

typedef std::map<const char*, SpatialIndexDescriptor*, string_less> SpatialIndexCache;

class SltConnection : public FdoIConnection, 
                      public FdoIConnectionInfo,
                      public FdoIConnectionPropertyDictionary,
                      public FdoIDataStorePropertyDictionary
{
public:

    SltConnection();

protected:
    virtual ~SltConnection();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    SLT_IMPLEMENT_REFCOUNTING

    //-------------------------------------------------------
    // FdoIConnection implementation
    //-------------------------------------------------------

public:
    
    //FdoIConnection functions requiring SQLite specific logic
    //implemented in the .cpp file
    virtual FdoString*          GetConnectionString();
    virtual void                SetConnectionString(FdoString* value);
    virtual FdoConnectionState  Open();
    virtual void                Close();
    virtual FdoICommand*        CreateCommand(FdoInt32 commandType);

    
    //FdoIConnection function implementations
    virtual FdoIConnectionCapabilities* GetConnectionCapabilities() 
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoISchemaCapabilities*     GetSchemaCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoICommandCapabilities*    GetCommandCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIFilterCapabilities*     GetFilterCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIExpressionCapabilities* GetExpressionCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIRasterCapabilities*     GetRasterCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoITopologyCapabilities*   GetTopologyCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIGeometryCapabilities*   GetGeometryCapabilities()
                                                { return FDO_SAFE_ADDREF(m_caps); }
    virtual FdoIConnectionInfo*         GetConnectionInfo()
                                                { return FDO_SAFE_ADDREF(this); }
    virtual FdoConnectionState          GetConnectionState() 
                                                { return m_connState; }


    virtual FdoInt32 GetConnectionTimeout()                     { return 0; }
    virtual void SetConnectionTimeout(FdoInt32 value)           { }
    virtual FdoITransaction* BeginTransaction();
    virtual FdoPhysicalSchemaMapping* CreateSchemaMapping()     { return NULL; }
    virtual void SetConfiguration(FdoIoStream* stream)          { }
    virtual void Flush()                                        { }


    //-------------------------------------------------------
    // FdoIConnectionInfo implementation
    //-------------------------------------------------------

    virtual FdoString* GetProviderName()                { return SQLITE_PROVIDER_NAME; }
    virtual FdoString* GetProviderDisplayName()         { return SQLITE_PROVIDER_DISPLAY_NAME; }
    virtual FdoString* GetProviderDescription()         { return SQLITE_PROVIDER_DESCRIPTION; }
    virtual FdoString* GetProviderVersion()             { return SQLITE_PROVIDER_VERSION; }
    virtual FdoString* GetFeatureDataObjectsVersion()   { return SQLITE_FDO_VERSION; }
    
    virtual FdoIConnectionPropertyDictionary*   GetConnectionProperties()
                                                        { return FDO_SAFE_ADDREF(this); }
    virtual FdoProviderDatastoreType            GetProviderDatastoreType() 
                                                        { return FdoProviderDatastoreType_File; } 
    virtual FdoStringCollection*                GetDependentFileNames()
        { 
            FdoStringCollection* fsc = FdoStringCollection::Create();
            fsc->Add((*m_mProps)[PROP_NAME_FILENAME].c_str());
            return fsc; 
        }

//-------------------------------------------------------
// FdoIConnectionPropertyDictionary declaration
//-------------------------------------------------------

    virtual FdoString**     GetPropertyNames            (FdoInt32& count);
    virtual FdoString*      GetProperty                 (FdoString* name);
    virtual void            SetProperty                 (FdoString* name, FdoString* value);
    virtual FdoString*      GetPropertyDefault          (FdoString* name);
    virtual bool            IsPropertyRequired          (FdoString* name);
    virtual bool            IsPropertyProtected         (FdoString* name);
    virtual bool            IsPropertyEnumerable        (FdoString* name);
    virtual FdoString**     EnumeratePropertyValues     (FdoString* name, FdoInt32& count);
    virtual FdoString*      GetLocalizedName            (FdoString* name);
    virtual bool            IsPropertyFileName          (FdoString* name);
    virtual bool            IsPropertyFilePath          (FdoString* name);
    virtual bool            IsPropertyDatastoreName     (FdoString* name);

//--------------------------------------------------------
//   Command execution functions -- each of these corresponds
//   to an FDO command
//--------------------------------------------------------

    FdoFeatureSchemaCollection* 
                        DescribeSchema          (FdoStringCollection* classNames, bool makeACopy = true);
    
    FdoISpatialContextReader* 
                        GetSpatialContexts      ();
    
    void                CreateDatabase          ();

    SltReader*  Select                         (FdoIdentifier*           fcname, 
                                                FdoFilter*               filter, 
                                                FdoIdentifierCollection* props,
                                                bool                     scrollable,
                                                const std::vector<NameOrderingPair>& ordering,
                                                FdoParameterValueCollection*  parmValues,
                                                FdoJoinCriteriaCollection* joinCriteria = NULL,
                                                FdoIdentifier* alias = NULL);

    FdoIDataReader*     SelectAggregates       (FdoIdentifier*             fcname, 
                                                FdoIdentifierCollection*   properties,
                                                bool                       bDistinct,
                                                FdoFilter*                  filter,
                                                FdoOrderingOption          eOrderingOption,
                                                FdoIdentifierCollection*   ordering,
                                                FdoFilter*                 grFilter,
                                                FdoIdentifierCollection*   grouping,
                                                FdoParameterValueCollection*  parmValues,
                                                FdoJoinCriteriaCollection* joinCriteria = NULL,
                                                FdoIdentifier* alias = NULL);

    FdoInt32            Update                 (FdoIdentifier*              fcname, 
                                                FdoFilter*                  filter, 
                                                FdoPropertyValueCollection* propvals,
                                                FdoParameterValueCollection*  parmValues);
    
    FdoInt32            Delete                 (FdoIdentifier*              fcname, 
                                                FdoFilter*                  filter,
                                                FdoParameterValueCollection*  parmValues);

    SltReader*          SelectView             (FdoClassDefinition* fc,
                                                FdoIdentifierCollection* props,
                                                StringBuffer& strWhere,
                                                FdoParameterValueCollection*  parmValues,
                                                const std::vector<NameOrderingPair>& ordering);

    SltReader*          SelectJoin             (FdoClassDefinition* fc,
                                                FdoIdentifierCollection* props, 
                                                StringBuffer& strWhere,
                                                FdoParameterValueCollection*  parmValues,
                                                const std::vector<NameOrderingPair>& ordering,
                                                FdoJoinCriteriaCollection* joinCriteria,
                                                FdoIdentifier* alias = NULL);

    void               AppendSelectJoin        (StringBuffer& sb,
                                                FdoJoinCriteriaCollection* joinCriteria,
                                                FdoIdentifier* alias);

    void                ApplySchema            (FdoFeatureSchema* schema, bool ignoreStates);

    sqlite3*        GetDbConnection() { return m_dbWrite; }
    bool            GetExtents(const wchar_t* fcname, double ext[4]);
    SltMetadata*    GetMetadata(const char* table);
    SltReader*      CheckForSpatialExtents(FdoIdentifierCollection* props, FdoFeatureClass* fc, FdoFilter* filter, FdoParameterValueCollection*  parmValues);
    FdoInt64        GetFeatureCount(const char* table);
    
    sqlite3_stmt*   GetCachedParsedStatement(const char* sql);
    void            ReleaseParsedStatement(const char* sql, sqlite3_stmt* stmt);
    void            ClearQueryCache(SQLiteClearActionType type = SQLiteClearActionType_All);
    
    bool SupportsDetailedGeomType();
    
    int StartTransaction(bool isUserTrans = false); 
    int CommitTransaction(bool isUserTrans = false);
    int RollbackTransaction(bool isUserTrans = false);
    bool IsTransactionStarted() { return (m_transactionState != SQLiteActiveTransactionType_None); }
    void GetGeometryExtent(const unsigned char* ptr, int len, DBounds* ext);
    bool IsCoordSysLatLong(const char* tablename, const char* columnname);
    bool GetCSTolerances(const char* tablename, double& xyTolerance, double& zTolerance);
    bool IsReadOnlyConnection();
    
    // when SC not found: if valIfNotFound = 0 the default SC will be returned else that value will be returned.
    int FindSpatialContext(const wchar_t* name, int valIfNotFound = 0);
    int GetDefaultSpatialContext();
    bool SupportsTolerance();
    bool AddSupportForTolerance();
    void FreeCachedSchema ();
    void ClearClassFromCachedSchema(const char* table, bool fullDrop);
    FdoStringCollection* GetDbClasses();
    static bool IsMetadataTable(const char* table);
    bool CanUseFdoMetadata() {return m_bUseFdoMetadata && m_bHasFdoMetadata;};
    bool NeedsMetadataLoaded(const char* table);
    void AddMetadata(const char* table, SltMetadata* md);
    SltMetadata* FindMetadata(const char* table);

private :
    void AddGeomCol(FdoGeometricPropertyDefinition* gpd, const wchar_t* fcname);
    void AddDataCol(FdoDataPropertyDefinition* dpd, const wchar_t* fcname);
    void AddClassToSchema(FdoClassCollection* classes, FdoClassDefinition* fc);
    void DeleteClassFromSchema(FdoClassDefinition* fc);
    void DeleteClassFromSchema(const wchar_t* fcName);
    void UpdateClassFromSchema(FdoClassCollection* classes, FdoClassDefinition* fc, FdoClassDefinition* mainfc);
    bool GetExtentAndCountInfo(FdoFeatureClass* fc, FdoFilter* filter, bool isExtentReq, FdoInt64* countReq, DBounds* extReq, FdoParameterValueCollection*  parmValues);
    void GenerateAutoGenerateTrigger(FdoClassDefinition* fc, bool dropTriggerFirst = false);

    void CollectBaseClassProperties(FdoClassCollection* myclasses, FdoClassDefinition* fc, FdoClassDefinition* mainfc, 
        StringBuffer& sb, int mode, UniqueConstraints& simpleUniqueConstr);
    void AddClassPrimaryKeys(FdoClassDefinition* fc, StringBuffer& sb);
    void AddPropertyConstraintDefaultValue(FdoDataPropertyDefinition* prop, StringBuffer& sb);
    void AddPropertyUniqueConstraint(UniqueConstraints& simpleUniqueConstr, FdoDataPropertyDefinition* prop, StringBuffer& sb);
    void AddComplexUniqueConstraints(FdoUniqueConstraintCollection* uniqueConstr, FdoClassDefinition* fc, StringBuffer& sb);
    std::wstring GenerateValidConstrName(FdoString* name);
    RowidIterator* GetScrollableIterator(SltReader* rdr);
    static int PrepareSpatialDatabase(sqlite3* db, bool useFdoMetadata, bool isInMemory = false);
    SpatialIndexDescriptor* GetSpatialIndexDescriptor(const char* table, int* geomIdx = NULL);
    void RebuildSpatialOperator(SpatialIndexDescriptor* spDesc, SltMetadata* md = NULL);

    static int commit_hook(void* caller);
    static void rollback_hook(void* caller);

    static void* sqlite3_spatial_index(void* caller, const char* tablename, int* geomIdx);
    static void  sqlite3_update_spatial_index(void* caller, void* sid, int action, sqlite3_int64 id, const void* blob, int szBlob);
    static void  sqlite3_release_spatial_index(void* sid, const char* zTableName);
    static char  sqlite3_spatial_context(void* caller, const char* tablename, const char* columnname);
    static void* sqlite3_spatial_iterator(void* si, const void* blob, int szBlob);
    static sqlite3_int64 sqlite3_spatial_iterator_readnext(void* siit);
    static void sqlite3_spatial_iterator_release(void* siit);
    static void sqlite3_spatial_iterator_reset(void* siit);

    bool                                    m_changesAvailable;
    bool                                    m_isReadOnlyConnection;

    sqlite3*                                m_dbWrite;

    std::map<std::wstring, std::wstring>*   m_mProps;
    std::wstring                            m_connStr;
    FdoConnectionState                      m_connState;
    FdoFeatureSchemaCollection*             m_pSchema;

    MetadataCache                           m_mNameToMetadata;
    SpatialIndexCache                       m_mNameToSpatialIndex;
    QueryCache                              m_mCachedQueries;
    int                                     m_cleanCount;

    SltCapabilities*                        m_caps;

    bool                                    m_bUseFdoMetadata;
    bool                                    m_bHasFdoMetadata;
    char                                    m_cSupportsDetGeomType;
    SQLiteActiveTransactionType             m_transactionState;
    // geometry conversion buffer
    unsigned char*                          m_wkbBuffer;
    int                                     m_wkbBufferLen;
    int                                     m_defSpatialContextId;
    char                                    m_cSupportsTolerance; // -1 = not init; 0=false; 1=true
};
