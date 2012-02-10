#ifndef FDOSMPHSQSOWNER_H
#define FDOSMPHSQSOWNER_H       1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#include "../../../SchemaMgr/Ph/Owner.h"
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Rd/ViewRelObjectReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/Rd/SynonymReader.h>
#include <Sm/Ph/Rd/TableJoin.h>
#include "SchemaCollection.h"

class FdoSmPhSqsCoordinateSystem;

// SqlServer Provider implementation of an Owner.
// Represents an SqlServer schema (user)
class FdoSmPhSqsOwner : public FdoSmPhGrdOwner
{
public:

    // Constructs an instance of an Owner object.
    //
    // Parameters:
    //      name: owner name
    //      pDatabase: database containing this owner.
    //      elementState: determines whether this database object is new
    FdoSmPhSqsOwner(
        FdoStringP name,
        bool hasMetaSchema,
        const FdoSmPhDatabase* pDatabase,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    ~FdoSmPhSqsOwner(void);

    // Returns true if spatial contexts defined in SC metadata table (f_spatialcontexts)
    bool GetHasSCMetaSchema();

    // Returns true if classes are defined in class metadata table (f_classdefinition)
    bool GetHasClassMetaSchema();

    // Returns true if properties are defined in property metadata table (f_attributedefinition)
    bool GetHasAttrMetaSchema();

    // Returns true if association properties are defined in the metadata table (f_associationdefinition)
    bool GetHasAssocMetaSchema();

    // Returns true if object properties are defined in the metadata table (f_attributedependencies)
    bool GetHasObPropMetaSchema();

    // Returns true if Schema Attribute Dictionary items are defined in the metadata table (f_sad)
    bool GetHasSADMetaSchema();

    // Returns true if Schema option is defined in the metadata table (f_schemaoptions)
    bool GetHasSCOptionMetaSchema();

    // Returns true if options is defined in the metadata table (f_options)
    bool GetHasOptionMetaSchema();

    // Returns true if Schema option is defined in the metadata table (f_schemainfo)
    bool GetHasSCInfoMetaSchema();

    // Returns true if spatial contexts geometry defined in SC metadata table (f_spatialcontextgeom)
    bool GetHasSCGeomInfoMetaSchema();

    // Returns true if spatial contexts group defined in SC metadata table (f_spatialcontextgroup)
    bool GetHasSCGroupInfoMetaSchema();

    bool IsRdbObjNameAscii7();

    // Find a schema (given by name) in this owner.
    // Returns NULL if the schema is not in this owner.
    FdoSmPhSqsSchemaP FindSchema( FdoStringP schemaName );

    // Retrieves all schemas for this owner.
    // Caches them if not already cached
    FdoSmPhSqsSchemasP GetSchemas();

    // Given a Spatial Reference ID, return the coordinate system info
    // Returns NULL if coordinate system not found.
    virtual FdoSmPhCoordinateSystemP FindCoordinateSystem( FdoInt64 srid );

    // Return the coordinate system info for the given coordinate system name.
    // Returns NULL if coordinate system not found.
    virtual FdoSmPhCoordinateSystemP FindCoordinateSystem( FdoStringP csName );

    // Return the coordinate system info for the given well-known text.
    // Returns NULL if coordinate system not found.
    virtual FdoSmPhCoordinateSystemP FindCoordinateSystemByWkt( FdoStringP wkt );

    // Same as FindCoordinateSystem except that NULL is returned if the coordinate
    // system is not in sys.spatial_reference_systems. In other words, the extended
    // coordinate systems are not checked.
    virtual FdoSmPhCoordinateSystemP FindCataloguedCoordinateSystem( FdoInt64 srid );

	// Get the name of function to retrieve current database name
	FdoString* GetDbNameClause(bool isEqual);

    virtual FdoStringP GetBestSchemaName() const;

	void CreateMetaClass();

    // Removes a schema from the cache without dropping it from
    // the RDBMS.
    void DiscardSchema( FdoSmPhSqsSchema* schema );
    
    // Extend base function to commit schema changes.
    virtual void CommitChildren( bool isBeforeParent );

    FdoSchemaExceptionP Errors2Exception(FdoSchemaException* pFirstException ) const;
    
    // Make this owner the current schema
    virtual void SetCurrent();

    // Given a database object and geometric column name, returns it's SRID.
    // The SRID is determined by the value of the first row in the dbobject.
    // Returns -1 if the dbobject has no rows.
    //
    // Exception is thrown if the column does not exist or is not geometric.
    virtual FdoInt64 SampleColumnSrid( FdoStringP dbObjectName, FdoStringP columnName );
    
    virtual FdoInt64 GetMetadataColumnSrid (FdoStringP dbObjectName, FdoStringP columnName);

    // Create a new schema and add it to the cache.
    // Schema is added to the RDBMS when it or this owner are committed.
    FdoSmPhSqsSchemaP CreateSchema( FdoStringP schemaName );

    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDbObjectReader( FdoStringP dbObject = L"") const;

    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDbObjectReader( FdoStringsP objectNames ) const;

    // Create a reader to get all database objects for this join.
    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDbObjectReader( FdoSmPhRdTableJoinP join ) const;

    /// Create a reader to get all derived objects for this owner.
    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDerivedObjectReader( FdoStringP objectName = L"") const;

    /// Create a reader to get derived objects this owner and object name list.
    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDerivedObjectReader( FdoStringsP objectNames ) const;

    virtual FdoPtr<FdoSmPhRdBaseObjectReader> CreateBaseObjectReader() const;

    virtual FdoPtr<FdoSmPhRdBaseObjectReader> CreateBaseObjectReader( FdoStringsP objectNames ) const;

    virtual FdoPtr<FdoSmPhRdViewRelationsObjectReader> CreateViewRelationsObjectReader( FdoStringsP objectNames ) const;

    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoStringP constraintName ) const;

	virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoStringP tableName, FdoStringP constraintType) const;

	virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoStringsP tableNames, FdoStringP constraintType) const;

    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoSmPhRdTableJoinP join, FdoStringP constraintType) const;

    virtual FdoPtr<FdoSmPhRdFkeyReader> CreateFkeyReader() const;

    virtual FdoPtr<FdoSmPhRdFkeyReader> CreateFkeyReader(  FdoStringsP objectNames ) const;

    virtual FdoPtr<FdoSmPhRdIndexReader> CreateIndexReader() const;

    /// Create a reader to get all indexes for this owner and object name list.
    virtual FdoPtr<FdoSmPhRdIndexReader> CreateIndexReader( FdoStringsP objectNames ) const;

    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader() const;

    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader( FdoPtr<FdoSmPhRdTableJoin> join ) const;

    /// Create a reader to get primary keys for this owner and object name list.
    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader(  FdoStringsP objectNames ) const;

    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader() const;

    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader( FdoStringsP objectNames ) const;

    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader( FdoSmPhRdTableJoinP join ) const;

    virtual FdoSmPhRdSynonymReaderP CreateSynonymReader() const;

    virtual FdoSmPhRdSynonymReaderP CreateSynonymReader( FdoStringP synonymName ) const;

    virtual FdoSmPhRdSynonymReaderP CreateSynonymReader( FdoStringsP synonymNames ) const;

	/// Get reader to retrieve all spatial contexts for the connection (no metaschema).
	virtual FdoPtr<FdoSmPhRdSpatialContextReader> CreateRdSpatialContextReader();

	/// Get reader to retrieve all spatial contexts for a database object.
	virtual FdoPtr<FdoSmPhRdSpatialContextReader> CreateRdSpatialContextReader( FdoStringP dbObjectName );

	/// Get reader to retrieve all spatial contexts for a list of database objects.
	virtual FdoPtr<FdoSmPhRdSpatialContextReader> CreateRdSpatialContextReader( FdoStringsP objectNames );

    // Create a reader to get the coordinate system(s) of the given csysName.
    // When csysName is L"" then all coordinates systems for this datastore are read.
    virtual FdoPtr<FdoSmPhRdCoordSysReader> CreateCoordSysReader( FdoStringP csysName = L"") const;

    // Create a reader to get the coordinate system with the given SRID.
    virtual FdoPtr<FdoSmPhRdCoordSysReader> CreateCoordSysReader( FdoInt64 srid ) const;

   	void SetOptions();

protected:

    // Table, View and Synonym creation implementors
    virtual FdoSmPhDbObjectP NewTable(
        FdoStringP tableName,
        FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
    );

    virtual FdoSmPhDbObjectP NewView(
        FdoStringP viewName,
        FdoStringP rootDatabase,
        FdoStringP rootOwner,
        FdoStringP rootObjectName,
        FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
    );

    virtual FdoSmPhDbObjectP NewSynonym(
        FdoStringP sequenceName,
        FdoSmPhDbObjectP rootObject,
        FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
    );

    // Overrides for modifying this owner
    virtual bool Add();
    //TODO: nothing to update yet. However, should
    //throw an exception when Modify() is called.
    virtual bool Modify() {return true;}
    virtual bool Delete();

    virtual FdoInt32 GetCandFetchSize();

private:
    // Loads all schemas into this owner's cache.
    void LoadSchemas();

    // check if certain tables exists or not.
    void LoadFdoMetadata();

    // Loads all extended coordinate systems. 
    // SQL Server's coordinate system catalogue (sys.spatial_reference_systems)
    // contains only geodetic systems. However, SQL Server allows geometries
    // to have coordinate systems not in this catalogue. In this case, the only info
    // on these coordinate systems, that can be retrieved from the RDBMS, is the SRID
    // (usually an EPSG number). The provider also needs the WKT 
    // so it can give enough information about the coordinate system, via 
    // FdoIGetSpatialContexts, to applications such as Map and MapGuide.
    //
    // The WKT's for non-catalogued coordinate systems can be specified in a file called
    // extendedCoordSys.txt", in the "com" subdirectory of the directory where the provider DLL
    // resides. If this file is found, 
    // this function loads the extended coordinate systems from it.    
    void LoadExtendedCoordinateSystems();

    FdoSmPhSqsSchemasP mSchemas;

    FdoSmPhCoordinateSystemsP mExtendedCoordinateSystems;

    FdoInt32 mIsRdbObjNameAscii7;
    bool mFdoMetadataLoaded;
    FdoInt32 mTableFlags;
};

typedef FdoPtr<FdoSmPhSqsOwner> FdoSmPhSqsOwnerP;

#endif
