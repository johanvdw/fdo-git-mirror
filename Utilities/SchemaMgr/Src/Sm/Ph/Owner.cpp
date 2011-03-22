/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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

#include "stdafx.h"
#include <math.h>
#include <Sm/Ph/Owner.h>
#include <Sm/Error.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/Rd/ColumnReader.h>
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/ViewReader.h>
#include <Sm/Ph/Rd/CoordSysReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/Rd/FkeyReader.h>
#include <Sm/Ph/Rd/IndexReader.h>
#include <Sm/Ph/Rd/PkeyReader.h>
#include <Sm/Ph/Rd/SpatialContextReader.h>
#include <Sm/Ph/IndexLoader.h>
#include <Sm/Ph/SynonymBaseLoader.h>
#include <Sm/Ph/Rd/TableJoin.h>
#include <Sm/Ph/OptionsReader.h>
#include <Sm/Ph/SchemaReader.h>

FdoString* FdoSmPhOwner::NOT_CLASSIFIED = L"nc";
FdoString* FdoSmPhOwner::NOT_EXIST = L"ne";
FdoString* FdoSmPhOwner::CLASSIFIED = L"c";

class FdoSmPhSynonym;

FdoSmPhOwner::FdoSmPhOwner(
    FdoStringP name, 
    bool hasMetaSchema,
    const FdoSmPhDatabase* pDatabase,
	FdoSchemaElementState elementState
) : 
    FdoSmPhDbElement(name, (FdoSmPhMgr*) NULL, pDatabase, elementState ),
    mDbObjectsCached(false), mDbComponentsCached(false),
    mIndexLoader(NULL),
    mSynonymBaseLoader(NULL)
{
    SetHasMetaSchema( hasMetaSchema );
    mLtMode = NoLtLock;
    mLckMode = NoLtLock;

    // Load LT and Lock settings from datastore only for existing owners.
    mLtLckLoaded = (elementState == FdoSchemaElementState_Added);

    mSchemaVersion = 0.0;
    // Schema Info is handled a bit differently. We want to trip a load
    // after a new owner is committed, so that the right schema version
    // is retrieved.
    mSchemaInfoLoaded = false;
	SetIsSystem(false);

    mNotClassifiedObjects = FdoDictionary::Create();

    mReservedDbObjectNames = FdoStringCollection::Create();

    // Add commonly accessed MetaSchema tables to fetch candidates list. This
    // causes them to be fetched in one reader.
    mCandDbObjects = FdoDictionary::Create();
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_associationdefinition") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_attributedefinition") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_attributedependencies") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_classdefinition") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_dbopen") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_sad") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_lockname") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_options") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_schemainfo") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_schemaoptions") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_spatialcontext") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_spatialcontextgeom") );
    AddCandDbObject( GetManager()->GetDcDbObjectName(L"f_spatialcontextgroup") );

    mNextBaseCandIdx = 0;

    mSpatialContextsLoaded = false;

    mCoordinateSystemsLoaded = false;

    mBulkLoadPkeys = false;
    mBulkLoadFkeys = false;
}

FdoSmPhOwner::~FdoSmPhOwner(void)
{
    FDO_SAFE_RELEASE( mIndexLoader );
    FDO_SAFE_RELEASE( mSynonymBaseLoader );
}

void FdoSmPhOwner::SetPassword( FdoStringP password )
{
    mPassword = password;
}

FdoStringP FdoSmPhOwner::GetPassword()
{
    return mPassword;
}

void FdoSmPhOwner::SetIsSystem( bool isSystem )
{
	mIsSystem = isSystem;
}

bool FdoSmPhOwner::GetIsSystem()
{
	return mIsSystem;
}

void FdoSmPhOwner::SetHasMetaSchema( bool hasMetaSchema )
{
    mHasMetaSchema = hasMetaSchema;
}

bool FdoSmPhOwner::GetHasMetaSchema()
{
    return mHasMetaSchema;
}

double FdoSmPhOwner::GetSchemaVersion()
{
    LoadSchemaInfo();

    return mSchemaVersion;
}

FdoString* FdoSmPhOwner::GetDescription() const
{
    ((FdoSmPhOwner*) this)->LoadSchemaInfo();

    return FdoSmPhDbElement::GetDescription();
}

void FdoSmPhOwner::SetLtMode( FdoLtLockModeType LtMode )
{
    LoadLtLck();

	mLtMode = LtMode;
}

FdoLtLockModeType FdoSmPhOwner::GetLtMode() const
{
    ((FdoSmPhOwner*)this)->LoadLtLck();

	return mLtMode;
}

void FdoSmPhOwner::SetLckMode( FdoLtLockModeType LckMode )
{
    LoadLtLck();

	mLckMode = LckMode;
}

FdoLtLockModeType FdoSmPhOwner::GetLckMode() const
{
    ((FdoSmPhOwner*)this)->LoadLtLck();

	return mLckMode;
}

const FdoLockType* FdoSmPhOwner::GetLockTypes( FdoLtLockModeType lckMode, FdoInt32& size)
{
    // Inltialze lock lists
    LoadLtLck();

    // Get entry for the given lock mode.
    const FdoSmPhLockTypes* lockTypes = GetLockTypesCollection()->RefItem( lckMode );
    if ( !lockTypes ) 
        // Default to the NoLock mode if mode not supported.
        lockTypes = mLockTypes->RefItem( NoLtLock );

    if ( !lockTypes ) {
        // No lock types even for default mode so return nothing
        size = 0;
        return (FdoLockType*) NULL;
    }

    return lockTypes->GetLockTypes( size );
}

const FdoSmPhDbObject* FdoSmPhOwner::RefDbObject(FdoStringP dbObject) const
{
    return (FdoSmPhDbObject*) ((FdoSmPhOwner*) this)->FindDbObject(dbObject);
}

FdoSmPhDbObjectP FdoSmPhOwner::FindDbObject(FdoStringP dbObject)
{
    // If Db Object name is too short or too long then we know right away that it
    // does not exist.
    if ( (dbObject == L"") || (dbObject.GetLength() > GetManager()->DbObjectNameMaxLen()) )
        return (FdoSmPhDbObject*) NULL;

    // Check cache for database object
    FdoSmPhDbObjectP pDbObject = GetDbObjects()->FindItem(dbObject);

    if ( ((!pDbObject) || !(pDbObject->ColumnsLoaded())) && (dbObject != L"")) {
        // Object is not in the cache or its columns have not been cached. 
        // If it is in the fetch candidate list then fetch
        // it along with some other candidates. Some other candidates
        // are selected to help performance since these will likely be
        // asked for later.
        pDbObject = CacheCandDbObjects( dbObject );
    }

    if ( !pDbObject ) {
        // Not a candidate either. Check if previously fetched but not classified.
        if ( mNotClassifiedObjects->IndexOf( dbObject ) >= 0 ) 
            return pDbObject;

        // Not in cache so read it in.
        FdoSmPhRdDbObjectReaderP reader = CreateDbObjectReader(dbObject);
   
        // default reason for not being able to classify (create a DbObject for) the object;
        // it doesn't exist.
        FdoString* reason = NOT_EXIST;
        if ( reader->ReadNext() )
        {
            // object exists, change reason to not classified.
            reason = NOT_CLASSIFIED;
            pDbObject = CacheDbObject( reader );
        }

        if ( (!pDbObject) && (dbObject != L"")) {
            // Not in RDBMS so add to not found list (avoids multiple RDBMS fetches when this
            // object is asked for repeatedly).
            // reason indicates whether object was not found or was found but not classified.
            FdoDictionaryElementP elem = FdoDictionaryElement::Create( dbObject, reason );
            mNotClassifiedObjects->Add( elem );
        }
    }
   
    return(pDbObject);
}

FdoSmPhDbObjectP FdoSmPhOwner::GetDbObject(FdoStringP dbObject)
{
    FdoSmPhDbObjectP pDbObject = FindDbObject(dbObject);

    if ( !pDbObject ) 
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_4),
                (FdoString*) GetQName(),
				(FdoString*) dbObject
            )
        );

    return(pDbObject);
}

FdoSmPhDbObjectP FdoSmPhOwner::FindReferencedDbObject(FdoStringP dbObject, FdoStringP owner, FdoStringP database )
{   
    FdoSmPhDbObjectP pDbObject;

    // Look for referenced object in the cache for its owner.
    FdoSmPhOwnerP refOwner = GetManager()->FindOwner( owner, database );

    if ( refOwner ) {
        pDbObject = refOwner->GetDbObjects()->FindItem( dbObject );

        if ( !pDbObject || (!pDbObject->ColumnsLoaded()) ) {
            // Not fully cached: not in cache or columns have not been cached. 
            // Set up base objects for bulk loading
            LoadBaseObjectCands();

            // Find the object. This causes the bulk loading of it and some other
            // objects into the schema cache.
            pDbObject = refOwner->FindDbObject( dbObject );
        }
    }

    return pDbObject;
}

FdoSmPhDbObjectP FdoSmPhOwner::GetCachedDbObject(FdoInt32 idx)
{
    FdoSmPhDbObjectP dbObject;
    FdoSmPhDbObjectsP dbObjects = GetDbObjects();

    if ( (idx >= 0) && (idx < dbObjects->GetCount()) ) 
        dbObject = dbObjects->GetItem( idx );

    return dbObject;
}

FdoSmPhSpatialContextsP FdoSmPhOwner::GetSpatialContexts()
{
    LoadSpatialContexts();

    return mSpatialContexts;
}

FdoSmPhSpatialContextP FdoSmPhOwner::FindSpatialContext( FdoInt64 scId )
{
    FdoSmPhSpatialContextP sc;

    if ( mSpatialContexts ) 
        sc = mSpatialContexts->FindItemById( scId );

    if ( !sc ) {
        LoadSpatialContexts();
        sc = mSpatialContexts->FindItemById( scId );
    }

    return sc;
}

FdoSmPhSpatialContextGeomsP FdoSmPhOwner::GetSpatialContextGeoms()
{
    LoadSpatialContexts();

    return mSpatialContextGeoms;
}

FdoSmPhSpatialContextGeomP FdoSmPhOwner::FindSpatialContextGeom( FdoStringP dbObjectName, FdoStringP columnName )
{
    FdoStringP scGeomName = FdoSmPhSpatialContextGeom::MakeName(dbObjectName, columnName);
    FdoSmPhSpatialContextGeomP scGeom;

    // Check if required association already cached.
    if ( mSpatialContextGeoms )
        scGeom = mSpatialContextGeoms->FindItem(scGeomName);

    if ( !scGeom ) {
        // Not in cache, so load the associations for the given db object and
        // try again.
        LoadSpatialContexts( dbObjectName );
        scGeom = mSpatialContextGeoms->FindItem(scGeomName);
    }

    return scGeom;
}

FdoSmPhCoordinateSystemP FdoSmPhOwner::FindCoordinateSystem( FdoInt64 srid )
{
    FdoSmPhCoordinateSystemP coordSys;

    // Check the cache first
    if ( mCoordinateSystems ) 
        coordSys = mCoordinateSystems->FindItemById( srid );

    if ( !coordSys && !mCoordinateSystemsLoaded ) {
        // Not in the cache, load from the RDBMS and try again.
        LoadCoordinateSystems( CreateCoordSysReader(srid) );
        coordSys = mCoordinateSystems->FindItemById( srid );
    }

    return coordSys;
}

FdoSmPhCoordinateSystemP FdoSmPhOwner::FindCoordinateSystem( FdoStringP csName )
{
    FdoSmPhCoordinateSystemP coordSys;

    // Check the cache first
    if ( mCoordinateSystems ) 
        coordSys = mCoordinateSystems->FindItem( csName );

    if ( !coordSys && !mCoordinateSystemsLoaded ) {
        // Not in the cache, load from the RDBMS and try again.
        LoadCoordinateSystems( CreateCoordSysReader(csName) );
        coordSys = mCoordinateSystems->FindItem( csName );
    }

    return coordSys;
}

FdoSmPhCoordinateSystemP FdoSmPhOwner::FindCoordinateSystemByWkt( FdoStringP wkt )
{
    FdoSmPhCoordinateSystemP coordSys;

    // Check the cache first
    if ( mCoordinateSystems ) 
        coordSys = mCoordinateSystems->FindItemByWkt( wkt );

    if ( !coordSys && !mCoordinateSystemsLoaded ) {
        // Not in the cache, load from the RDBMS and try again.
        mCoordinateSystemsLoaded = true;
        LoadCoordinateSystems( CreateCoordSysReader() );
        coordSys = mCoordinateSystems->FindItemByWkt( wkt );
    }

    return coordSys;
}

void FdoSmPhOwner::CacheCoordinateSystem( FdoSmPhCoordinateSystemP coordSys )
{
    FdoInt32 index = -1;

    if ( !mCoordinateSystems ) 
        mCoordinateSystems = new FdoSmPhCoordinateSystemCollection();
    else
        index = mCoordinateSystems->IndexOf(coordSys->GetName());

    // Add coordinate system only if not already in collection.
    if ( index < 0 ) 
        mCoordinateSystems->Add(coordSys);
}

FdoStringP FdoSmPhOwner::GetBestSchemaName() const
{
    return FdoSmPhMgr::RdSchemaPrefix + GetName();
}

FdoStringP FdoSmPhOwner::UniqueDbObjectName( FdoStringP objectName )
{
	// Censor out characters not allowed.
    // Also convert name to default case. 
    // LogicalPhysical Schema Manager does not support creating 
    // objects with names in other cases.
	FdoStringP workName = GetManager()->GetDcDbObjectName( GetManager()->CensorDbObjectName(objectName) );
	FdoStringP outName = workName;
    FdoSize tableMaxSize = GetManager()->TableNameMaxLen();      

	// Adjust the name if it is too long, is a reserved name or the name of an existing
	// table.
	//
	// Do length check against char* version since this is what goes into the database.

	if ( ((int)strlen(outName) > tableMaxSize) ||
		 GetManager()->IsDbObjectNameReserved(outName) ||
		 IsDbObjectNameReserved( outName )
	) {

		for ( int i = 1; ; i++ ) {
			// Truncate and tack on a unique number
			outName = FdoStringP::Format( L"%ls%d",
							(FdoString*) GetManager()->GetDcDbObjectName( 
                                workName.Mid( 
                                    0, 
                                    tableMaxSize - ((int) log10((double)i)) - 1, 
                                    !(GetManager()->IsRdbUnicode()) 
                                ) 
                            ),
							i
					);

			if ( (!IsDbObjectNameReserved(outName)) && 
                 (!GetManager()->IsDbObjectNameReserved(outName)) )
				// Name not in use, we're done
				break;

			// Otherwise, increment unique number and try again.
		}
	}

	ReserveDbObjectName(outName, false);

	return(outName);
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhOwner::CreateDbObjectReader( FdoStringsP objectNames ) const
{
    return (FdoSmPhRdDbObjectReader*) NULL;
}
 
FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhOwner::CreateDerivedObjectReader( FdoStringP objectName ) const
{
    return (FdoSmPhRdDbObjectReader*) NULL;
}
 
FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhOwner::CreateDerivedObjectReader( FdoStringsP objectNames ) const
{
    return (FdoSmPhRdDbObjectReader*) NULL;
}
 
FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhOwner::CreateDbObjectReader( FdoSmPhRdTableJoinP join ) const
{
    return (FdoSmPhRdDbObjectReader*) NULL;
}

FdoPtr<FdoSmPhRdViewReader> FdoSmPhOwner::CreateViewReader() const
{
    return (FdoSmPhRdViewReader*) NULL;
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhOwner::CreateConstraintReader( FdoStringsP objectNames, FdoStringP constraintType ) const
{
    return (FdoSmPhRdConstraintReader*) NULL;
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhOwner::CreateConstraintReader( FdoSmPhRdTableJoinP join, FdoStringP constraintType ) const
{
    return (FdoSmPhRdConstraintReader*) NULL;
}

FdoPtr<FdoSmPhRdCoordSysReader> FdoSmPhOwner::CreateCoordSysReader( FdoStringP csysName) const
{
    return new FdoSmPhRdCoordSysReader();
}

FdoPtr<FdoSmPhRdCoordSysReader> FdoSmPhOwner::CreateCoordSysReader( FdoInt64 srid ) const
{
    return new FdoSmPhRdCoordSysReader();
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhOwner::CreateFkeyReader() const
{
    return (FdoSmPhRdFkeyReader*) NULL;
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhOwner::CreateFkeyReader(  FdoStringsP objectNames ) const
{
    return (FdoSmPhRdFkeyReader*) NULL;
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhOwner::CreateIndexReader() const
{
    return (FdoSmPhRdIndexReader*) NULL;
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhOwner::CreateIndexReader( FdoStringsP objectNames ) const
{
    return (FdoSmPhRdIndexReader*) NULL;
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhOwner::CreatePkeyReader() const
{
    return (FdoSmPhRdPkeyReader*) NULL;
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhOwner::CreatePkeyReader(  FdoStringsP objectNames ) const
{
    return (FdoSmPhRdPkeyReader*) NULL;
}


FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhOwner::CreatePkeyReader( FdoPtr<FdoSmPhRdTableJoin> join ) const
{
    return (FdoSmPhRdPkeyReader*) NULL;
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhOwner::CreateColumnReader() const
{
    return (FdoSmPhRdColumnReader*) NULL;
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhOwner::CreateColumnReader( FdoStringsP objectNames ) const
{
    return (FdoSmPhRdColumnReader*) NULL;
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhOwner::CreateColumnReader( FdoSmPhRdTableJoinP join ) const
{
    return (FdoSmPhRdColumnReader*) NULL;
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhOwner::CreateBaseObjectReader() const
{
    return (FdoSmPhRdBaseObjectReader*) NULL;
}

FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhOwner::CreateBaseObjectReader( FdoStringsP objectNames ) const
{
    return (FdoSmPhRdBaseObjectReader*) NULL;
}

FdoPtr<FdoSmPhRdSynonymReader> FdoSmPhOwner::CreateSynonymReader() const
{
    return (FdoSmPhRdSynonymReader*) NULL;
}

FdoPtr<FdoSmPhRdSynonymReader> FdoSmPhOwner::CreateSynonymReader( FdoStringP synonymName) const
{
    return (FdoSmPhRdSynonymReader*) NULL;
}

FdoPtr<FdoSmPhRdSynonymReader> FdoSmPhOwner::CreateSynonymReader( FdoStringsP synonymNames) const
{
    return (FdoSmPhRdSynonymReader*) NULL;
}

FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhOwner::CreateRdSpatialContextReader()
{
    return new FdoSmPhRdSpatialContextReader(FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhOwner::CreateRdSpatialContextReader( FdoStringP dbObjectName )
{
    return new FdoSmPhRdSpatialContextReader(FDO_SAFE_ADDREF(this) );
}

FdoPtr<FdoSmPhIndexLoader> FdoSmPhOwner::CreateIndexLoader(
    FdoSmPhDbObjectsP dbObjects 
)
{
    return new FdoSmPhIndexLoader( FDO_SAFE_ADDREF(this), dbObjects );
}

FdoSmPhTableP FdoSmPhOwner::CreateTable(
    FdoStringP tableName,
    FdoStringP pkeyName
)
{
    if ( FindDbObject( tableName ) )
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_210),
				(FdoString*) GetQName(), 
                (FdoString*) tableName
			)
		);

    // TODO: set pkey name.
    FdoSmPhDbObjectP table = NewTable( tableName, FdoSchemaElementState_Added, NULL );
    GetDbObjects()->Add(table);

    return( table->SmartCast<FdoSmPhTable>() );
}

FdoSmPhViewP FdoSmPhOwner::CreateView(
    FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName
)
{
    if ( FindDbObject( viewName ) )
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_210),
				(FdoString*) GetQName(), 
                (FdoString*) viewName
			)
		);

    FdoSmPhDbObjectP view = NewView( 
        viewName, 
        rootDatabase,
        rootOwner,
        rootObjectName,
        FdoSchemaElementState_Added,
        NULL
    );
    
    GetDbObjects()->Add(view);

    return( view->SmartCast<FdoSmPhView>() );
}

FdoSmPhSynonymP FdoSmPhOwner::CreateSynonym(
    FdoStringP synonymName,
    FdoSmPhDbObjectP rootObject
)
{
    if ( FindDbObject( GetManager()->GetDcDbObjectName(synonymName) ) )
        throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_428),
				"Cannot create synonym %1$ls.%2$ls, an object of that name already exists.", 
                (FdoString*) GetQName(),
                (FdoString*) synonymName
            )
		);

    FdoSmPhDbObjectP synonym = NewSynonym( synonymName, rootObject, FdoSchemaElementState_Added, NULL );
    GetDbObjects()->Add(synonym);

    return( synonym->SmartCast<FdoSmPhSynonym>() );
}

FdoSmPhDbObjectsP FdoSmPhOwner::CacheDbObjects( bool cacheComponents )
{
    // skip if all objects already cached.
    if ( !mDbObjectsCached ) {
        mDbObjectsCached = true;
        mDbComponentsCached = cacheComponents;

        ReadAndCacheDbObjects(cacheComponents);

    }
    else if (cacheComponents && !mDbComponentsCached) {
        mDbComponentsCached = true;

        ReadAndCacheDbObjects(cacheComponents);
    }


    return GetDbObjects();
}

void FdoSmPhOwner::ReadAndCacheDbObjects(bool cacheComponents)
{
    // No need for fetch candidates since this function caches all object for this owner.
    mCandDbObjects->Clear();

    FdoSmPhRdDbObjectReaderP objReader;
    FdoSmPhRdViewReaderP viewReader;
    FdoSmPhRdColumnReaderP columnReader;
    FdoSmPhRdBaseObjectReaderP baseObjectReader;
    FdoSmPhRdConstraintReaderP ukeyReader;
    FdoSmPhRdConstraintReaderP ckeyReader;
    FdoSmPhRdFkeyReaderP fkeyReader;
    FdoSmPhRdPkeyReaderP pkeyReader;

   // Create reader for owner's db objects
    objReader = CreateDbObjectReader();

    if ( !objReader )
        return;

    if ( cacheComponents ) {
        // Caching db object components so create readers for components.
        // This function does interleaved fetches from each reader so all readers
        // (including dbObject reader) must return rows ordered by dbObject name.
        //
        // Doing a single query per owner for each component is more efficient than
        // a query per dbObject.
        viewReader = CreateViewReader();
        columnReader = CreateColumnReader();
        baseObjectReader = CreateBaseObjectReader();
        ukeyReader = CreateConstraintReader( L"", L"U" );
        ckeyReader = CreateConstraintReader( L"", L"C" );
        fkeyReader = CreateFkeyReader();
        pkeyReader = CreatePkeyReader();
    }

    while ( objReader->ReadNext() ) {
        // Cache the current dbObject
        FdoSmPhDbObjectP dbObject = CacheDbObject( objReader );

        if ( dbObject && cacheComponents ) {

            if ( columnReader ) 
                dbObject->CacheColumns( columnReader );

            if ( baseObjectReader ) 
                dbObject->CacheBaseObjects( baseObjectReader );

            if ( fkeyReader ) 
                dbObject->CacheFkeys( fkeyReader );

            if ( pkeyReader ) 
                dbObject->CachePkeys( pkeyReader );

            // Load the components into the db object.
            FdoSmPhTableP table = dbObject->SmartCast<FdoSmPhTable>();

            if ( table ) {
                if ( ukeyReader ) 
                    table->CacheUkeys( ukeyReader );

                if ( ckeyReader ) 
                    table->CacheCkeys( ckeyReader );
            }

            // Load the components into the db object.
            FdoSmPhViewP view = dbObject->SmartCast<FdoSmPhView>();

            if ( view ) {
                if ( viewReader ) 
                    view->CacheView( viewReader );
            }

            // The current object may have already been in the cache, but now its
            // components have been added. In this case, the lazy
            // loaders may have already visited this object and skipped it for 
            // bulk loading . Reset the lazy
            // loaders, so that they will revisit this object. Now that it has its
            // components, it might be a bulk load candidate.

            if ( mIndexLoader ) 
                mIndexLoader->Reset();
            if ( mSynonymBaseLoader ) 
                mSynonymBaseLoader->Reset();
            mNextBaseCandIdx = 0;
        }
    }

    // At this point, all geometric columns have been bulk loaded so need
    // to bulk load spatial contexts as well.
    GetManager()->SetBulkLoadSpatialContexts(true);
}

FdoSmPhDbObjectP FdoSmPhOwner::CacheDbObject(
    FdoPtr<FdoSmPhRdDbObjectReader> reader
)
{
    FdoStringP objName = reader->GetString(L"", L"name");
    FdoSmPhDbObjectP pDbObject = GetDbObjects()->FindItem( objName );

    if ( !pDbObject ) {
        pDbObject = NewDbObject( objName, FdoSchemaElementState_Unchanged, reader);

        if ( pDbObject ) {
            // Database object found, add it the the cache.
            GetDbObjects()->Add( pDbObject );

            // Now that it's cached, it no longer needs to be a fetch candidate
            RemoveCandDbObject( pDbObject->GetName() );
        }
    }


    return pDbObject;
}

FdoSmPhDbObjectP FdoSmPhOwner::NewDbObject(
    FdoStringP objName,
    FdoSchemaElementState elementState,
    FdoPtr<FdoSmPhRdDbObjectReader> reader
)
{
    FdoSmPhDbObjectP pDbObject;

    // Instantiate the right type of database object
    switch( reader->GetType() ) {
    case FdoSmPhDbObjType_Table:
        pDbObject = NewTable(objName, FdoSchemaElementState_Unchanged, reader);
        break;

    case FdoSmPhDbObjType_View:
        // TODO: Find out if there is a way to get the root table info.
        pDbObject = NewView(objName, L"", L"", L"", FdoSchemaElementState_Unchanged, reader);
        break;

    case FdoSmPhDbObjType_Synonym:
        pDbObject = NewSynonym(objName, (FdoSmPhDbObject*) NULL, FdoSchemaElementState_Unchanged, reader);
        break;
    }

    return pDbObject;
}

FdoSmPhDbObjectP FdoSmPhOwner::NewSynonym(
    FdoStringP synonymName,
    FdoSmPhDbObjectP rootObject,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
)
{
    return (FdoSmPhDbObject*) NULL;
}

void FdoSmPhOwner::DiscardDbObject( FdoSmPhDbObject* dbObject )
{
    if ( mDbObjects )
        mDbObjects->Remove( dbObject );
}

void FdoSmPhOwner::AddCandDbObject( FdoStringP objectName )
{
    // No need for fetch candidates when all objects for owner are cached. 
    // Bulk fetching candidates is pointless when fetch size is 1.
    if ( (!mDbComponentsCached) && (GetCandFetchSize() > 1) ) {
	    FdoSmPhDbObjectP pDbObject = GetDbObjects()->FindItem(objectName);

        if ( !pDbObject || !(pDbObject->ColumnsLoaded()) ) {
            // Object not fully cached, add to candidates list.
            FdoDictionaryElementP elem = mCandDbObjects->FindItem( objectName );
            
            if ( !elem ) {
                elem = FdoDictionaryElement::Create( objectName, L"" );
                mCandDbObjects->Add( elem );
            }
        }
    }
}

void FdoSmPhOwner::RemoveCandDbObject( FdoStringP objectName )
{
    FdoInt32 ix = mCandDbObjects->IndexOf(objectName);
    if ( ix >= 0 ) 
        mCandDbObjects->RemoveAt( ix );

}

void FdoSmPhOwner::CacheSynonymBases( FdoStringP synonymName )
{
    if ( !mSynonymBaseLoader ) 
        mSynonymBaseLoader = new FdoSmPhSynonymBaseLoader( FDO_SAFE_ADDREF(this), GetDbObjects() );

    mSynonymBaseLoader->Load( synonymName, !GetAreAllDbObjectsCached(), GetCandFetchSize() );
}

FdoSchemaExceptionP FdoSmPhOwner::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmPhDbElement::Errors2Exception(pFirstException);

    if ( mDbObjects ) {
    	// Add errors for the owner's database objects.
	    for ( int i = 0; i < mDbObjects->GetCount(); i++ )
    		pException = mDbObjects->RefItem(i)->Errors2Exception(pException);
    }

	return pException;
}

void FdoSmPhOwner::Discard()
{
    ((FdoSmPhDatabase*) GetParent())->DiscardOwner( this );
}

void FdoSmPhOwner::OnAfterCommit()
{
    // An object previously not found might get created on commit.
    // Therefore, clear the not found list since it is now stale. 
    mNotClassifiedObjects->Clear();
}

void FdoSmPhOwner::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<owner name=\"%s\">\n",
			(const char*) FdoStringP(GetName())
	);

	if ( mDbObjects && ref == 0 ) {
		for ( int i = 0; i < mDbObjects->GetCount(); i++ ) 
			mDbObjects->RefItem(i)->XMLSerialize(xmlFp, ref);
	}

	fprintf( xmlFp, "</owner>\n" );

}

void FdoSmPhOwner::CommitChildren( bool isBeforeParent )
{
    int i;

    if ( mDbObjects ) {
        // Handle foreign key deletes before table deletes to keep foreign 
        // keys from dangling
        if ( isBeforeParent ) 
            CommitFkeys(isBeforeParent);

        // Commit the database objects. Make sure tables are deleted after other
        // objects, since these objects might depend on these tables. Similarly
        // make sure tables are added before other objects. 

        for ( i = (mDbObjects->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem(i);
            FdoSmPhTableP table = dbObject.p->SmartCast<FdoSmPhTable>();
            if ( isBeforeParent == (table == NULL) ) 
                dbObject->Commit( true, isBeforeParent );
        }

        for ( i = (mDbObjects->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem(i);
            FdoSmPhTableP table = dbObject.p->SmartCast<FdoSmPhTable>();
            if ( isBeforeParent == (table != NULL) ) 
                dbObject->Commit( true, isBeforeParent );
        }

        // Handle foreign key adds before table adds to ensure all referenced tables
        // and columns are present.
        if ( !isBeforeParent ) 
            CommitFkeys(isBeforeParent);
    }
}

void FdoSmPhOwner::CommitFkeys( bool isBeforeParent )
{
    int i;

    if ( mDbObjects ) {
        for ( i = (mDbObjects->GetCount() - 1); i >= 0; i-- ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem(i);
            dbObject->CommitFkeys( isBeforeParent );
        }
    }
}

void FdoSmPhOwner::ReserveDbObjectName( FdoStringP objectName, bool bCheckReserved  )
{
	if ( !bCheckReserved || !IsDbObjectNameReserved(objectName) ) 
		mReservedDbObjectNames->Add( objectName );
}

bool FdoSmPhOwner::IsDbObjectNameReserved( FdoStringP objectName )
{
	int i;
	bool bReserved = false;

	// Check the in-memory reserved list first. Implementing this list as a
	// collection seems to provide reasonable performance. However, we might
	// have to change it to a map if this changes in the future.

	for ( i = 0; i < mReservedDbObjectNames->GetCount(); i++ ) {
		if ( objectName.ICompare( mReservedDbObjectNames->GetString(i) ) == 0 ) {
			bReserved = true;
			break;
		}
	}

	// If not in reserved list, check if object exists.

	if ( !bReserved && FindDbObject(objectName) )
		bReserved = true;

    if ( !bReserved ) 
    {
        // Object might exist but was not classified. Check the not classified objects list
        // to see if this was the case. If it exists, then its name is reserved.
        FdoDictionaryElementP elem = mNotClassifiedObjects->FindItem(objectName);
        if ( elem && (wcscmp(elem->GetValue(), NOT_CLASSIFIED) == 0) )
            bReserved = true;
    }

    // The rest of the checks are unnecessary if this datastore does not yet exist.
    if ( GetElementState() != FdoSchemaElementState_Added ) {
        // Also check if this name is used by a constraint
        FdoSmPhRdConstraintReaderP conRdr = CreateConstraintReader(objectName);
        if ( conRdr->ReadNext() ) 
            bReserved = true;

	    if ( !bReserved ) {
		    // Still not found. Check if it is referenced by the metaschema.

            FdoSmPhDbObjectP classDef = FindDbObject( GetManager()->GetDcDbObjectName(L"f_classdefinition") );
            FdoSmPhDbObjectP attDef = FindDbObject( GetManager()->GetDcDbObjectName(L"f_attributedefinition") );
            FdoStringP localObjectName = GetManager()->DbObject2MetaSchemaName(objectName);

            if ( classDef && attDef ) {
		        FdoStringP statement = 
			        FdoStringP::Format( 
				        L"select 1 from %ls where tablename in ( %ls, %ls ) union select 1 from %ls where tablename in ( %ls, %ls )",
				        (FdoString*)(GetManager()->GetDcDbObjectName(L"f_classdefinition")),
                        (FdoString*) GetManager()->FormatSQLVal(objectName, FdoSmPhColType_String),
                        (FdoString*) GetManager()->FormatSQLVal(localObjectName, FdoSmPhColType_String),
                        (FdoString*)(GetManager()->GetDcDbObjectName(L"f_attributedefinition")),
				        (FdoString*) GetManager()->FormatSQLVal(objectName, FdoSmPhColType_String),
				        (FdoString*) GetManager()->FormatSQLVal(localObjectName, FdoSmPhColType_String)
			        );

		        // Running a query for each table is not the most efficient way to do 
		        // things but this query is only run when schemas are updated. 

                FdoSmPhRowP row = new FdoSmPhRow( GetManager(), L"findtable" );
                FdoSmPhColumnP column = FdoSmPhDbObjectP(row->GetDbObject())->CreateColumnInt16( L"one", true );
                FdoSmPhFieldP field = new FdoSmPhField( row, L"one", column, L"", false );
		        FdoSmPhRdQueryReaderP tableRef = GetManager()->CreateQueryReader( row, statement );

		        if ( tableRef->ReadNext() ) {
			        // Table is referenced by metaschema
			        bReserved = true;
			        mReservedDbObjectNames->Add(objectName);
		        }
            }
	    }
    }

	return(bReserved);
}

FdoSmPhDbObjectP FdoSmPhOwner::CacheCandDbObjects( FdoStringP objectName)
{
    FdoSmPhDbObjectP retDbObject;
    FdoDictionaryP candDbObjects = FdoDictionary::Create();
    FdoInt32 fetchSize = GetCandFetchSize();
    FdoInt32 ix = mCandDbObjects->IndexOf( objectName );

    // No candidate fetching if object not a candidate or fetch size too small.
    if ( (ix < 0) || (fetchSize < 2) ) 
        return retDbObject;

    // Fetch some of the other candidates. Get the ones in the neighbourhood of the given object.
    FdoInt32 start = ix - (fetchSize/2);
    if ( start < 0 ) 
        start = 0;

    // Build the candidates list.
    FdoStringsP cands = FdoStringCollection::Create();
    FdoInt32 end;

    for ( end = start; (end < mCandDbObjects->GetCount()) && (cands->GetCount() < fetchSize); end++ ) {
        FdoDictionaryElementP elem = mCandDbObjects->GetItem(end);
        cands->Add( elem->GetName() );
    }

    // Pad out list with empty object names. 
    // Candidate fetches are done by binding candidate names into selects. Selects can be re-used
    // if number of bind variables stays consistent
    while ( cands->GetCount() < fetchSize ) 
        cands->Add( L"" );

    // Remove candidates from candidate list.
    // Put in temporary list to track which ones were not found.
    for  ( ix = (end - 1); ix >= start; ix-- ) {
        FdoDictionaryElementP elem = mCandDbObjects->GetItem( ix );
        candDbObjects->Add( elem );
        mCandDbObjects->RemoveAt(ix);
    }

    // Read the candidates.
    FdoSmPhRdDbObjectReaderP objReader;
    FdoSmPhRdPkeyReaderP pkeyReader;
    FdoSmPhRdFkeyReaderP fkeyReader;
    FdoSmPhRdConstraintReaderP ukeyReader;
    FdoSmPhRdConstraintReaderP ckeyReader;
    FdoSmPhRdColumnReaderP columnReader;
    FdoSmPhRdBaseObjectReaderP baseObjectReader;

    // Create reader for candidate db objects.
    objReader = CreateDbObjectReader( cands );
    if ( !objReader ) 
        return retDbObject;

    bool first = true;

    while ( objReader && objReader->ReadNext() ) {
        // Caching db object components so create readers for components.
        // This function does interleaved fetches from each reader so all readers
        // (including dbObject reader) must return rows ordered by dbObject name.
        //
        // For datastores with MetaSchema, only columns and constraints need to be bulk fetched.
        // Primary and Foreign keys, and indexes are never fetched.
        //
        // Doing a single query per owner for each component is more efficient than
        // a query per dbObject.
        // The join is used to limit results to those needed for this schema.

        if ( first ) {
            if ( GetBulkLoadPkeys() ) {
                pkeyReader = CreatePkeyReader( cands);
                if ( !pkeyReader ) 
                    pkeyReader = CreatePkeyReader();
            }
                    
            if ( GetBulkLoadFkeys() ) {
                fkeyReader = CreateFkeyReader( cands );
                if ( !fkeyReader ) 
                    fkeyReader = CreateFkeyReader();
            }

            if ( GetManager()->GetBulkLoadConstraints() ) {
                ukeyReader = CreateConstraintReader( cands, L"U" );
                ckeyReader = CreateConstraintReader( cands, L"C" );
            }

            columnReader = CreateColumnReader( cands );

            baseObjectReader = CreateBaseObjectReader(cands);
 
            first = false;
        }

        // Cache the current dbObject
        FdoSmPhDbObjectP dbObject = CacheDbObject( objReader );
        FdoDictionaryElementP elem = candDbObjects->FindItem( dbObject ? dbObject->GetName() : (FdoString*)(objReader->GetString(L"",L"name")) );
        if ( elem )
        {
            if ( dbObject ) 
                // Object was cached, mark it has having been read and classified.
                elem->SetValue(CLASSIFIED);
            else
                // Object not cached, mark it has having been read but not classified.
                elem->SetValue(NOT_CLASSIFIED);
        }

        if ( dbObject ) {
            if ( objectName == dbObject->GetName() ) 
                // Keep track of the object asked for so it can be returned.
                retDbObject = dbObject;

            // Load the components into the db object.
            FdoSmPhTableP table = dbObject->SmartCast<FdoSmPhTable>();

            if ( columnReader ) 
                dbObject->CacheColumns( columnReader );

            if ( pkeyReader ) 
                dbObject->CachePkeys( pkeyReader );

            if ( fkeyReader ) 
                dbObject->CacheFkeys( fkeyReader );

            if ( table ) {
                if ( ukeyReader ) 
                    table->CacheUkeys( ukeyReader );

                if ( ckeyReader ) 
                    table->CacheCkeys( ckeyReader );
            }

            if ( baseObjectReader ) 
                dbObject->CacheBaseObjects( baseObjectReader );

            // The current object may have already been in the cache, but now its
            // components have been added. In this case, the lazy
            // loaders may have already visited this object and skipped it for 
            // bulk loading. Reset the lazy
            // loaders, so that they will revisit this object. Now that it has its
            // components, it might be a bulk load candidate.

            if ( mIndexLoader ) 
                mIndexLoader->Reset();
            if ( mSynonymBaseLoader ) 
                mSynonymBaseLoader->Reset();
            mNextBaseCandIdx = 0;
        }
    }

    // Add any candidates not fetched to the not found list.
    for  ( ix = 0; ix < candDbObjects->GetCount(); ix++ ) {
        FdoDictionaryElementP elem = candDbObjects->GetItem( ix );

        // If no reason set yet then object was not found.
        if ( wcslen(elem->GetValue()) == 0 )
            elem->SetValue( NOT_EXIST );

        // If object not classifed or not found, add it to the not classified list.
        if ( wcscmp(elem->GetValue(), CLASSIFIED) != 0 ) 
            mNotClassifiedObjects->Add( elem );
    }

    return retDbObject;
}

void FdoSmPhOwner::CacheCandIndexes( FdoStringP objectName )
{
    FdoPtr<FdoSmPhIndexLoader> indexLoader;

    if ( !mIndexLoader ) {
        indexLoader = CreateIndexLoader( GetDbObjects() );
        mIndexLoader = FDO_SAFE_ADDREF( indexLoader.p );
    }
    else
    {
        indexLoader = FDO_SAFE_ADDREF(mIndexLoader);
    }

    indexLoader->Load( objectName, !mDbObjectsCached, GetCandFetchSize() );
}

bool FdoSmPhOwner::GetBulkLoadPkeys()
{
    return mBulkLoadPkeys;
}

void FdoSmPhOwner::SetBulkLoadPkeys( bool bulkLoad )
{
    mBulkLoadPkeys = bulkLoad;
}

bool FdoSmPhOwner::GetBulkLoadFkeys()
{
    return mBulkLoadFkeys;
}

void FdoSmPhOwner::SetBulkLoadFkeys( bool bulkLoad )
{
    mBulkLoadFkeys = bulkLoad;
}

bool FdoSmPhOwner::GetAreAllDbObjectsCached()
{
    return mDbObjectsCached;
}

void FdoSmPhOwner::LoadBaseObjectCands()
{
    int idx1;
    int idx2;

    if ( mDbObjects ) {
        long nextBaseCandIdx = mNextBaseCandIdx;
        mNextBaseCandIdx = mDbObjects->GetCount();

        // Check each dbObject not yet checked
        for ( idx1 = nextBaseCandIdx; idx1 < mDbObjects->GetCount(); idx1++ ) {
            FdoSmPhDbObjectP dbObject = mDbObjects->GetItem(idx1);

            // Skip objects whose columns have not been loaded. Loading
            // these causes column loads for each individual object.
            // (which is slow, and nullifies the advantage of bulk loading their 
            // base objects).
            if ( dbObject->ColumnsLoaded() ) 
            {
                FdoSmPhBaseObjectsP baseObjects = dbObject->GetBaseObjects();

                // Add each base object to it's owner's candidates list.
                for ( idx2 = 0; idx2 < baseObjects->GetCount(); idx2++ ) {
                    FdoSmPhBaseObjectP baseObject = baseObjects->GetItem(idx2);

                    FdoSmPhOwnerP baseOwner = GetManager()->FindOwner( baseObject->GetOwnerName(), baseObject->GetDatabaseName() );

                    if ( baseOwner ) {
                        baseOwner->AddCandDbObject( baseObject->GetObjectName() );
                        // Need primary keys of base objects (to determine view identity properties)
                        // so bulk load them.
                        baseOwner->SetBulkLoadPkeys(true);
                    }
                }

                dbObject->LoadFkeyRefCands();
            }
        }
    }
}

FdoInt32 FdoSmPhOwner::GetCandFetchSize()
{
    // 50 bound objects seems to give optimal performance for Oracle. 
    return 50;
}

void FdoSmPhOwner::LoadLtLck()
{
    // Reading options for owners in other database instances is not yet supported.
    if ( (!mLtLckLoaded) && (wcslen(GetParent()->GetName()) == 0) && this->GetHasMetaSchema() ) {
        mLtLckLoaded = true;
        FdoSmPhOptionsReaderP optRdr = GetManager()->CreateOptionsReader( GetName() );

        // read each option, looking for the long transaction and locking options.
        while ( optRdr->ReadNext() ) {
            FdoStringP optName = optRdr->GetName();

            if ( optName == L"LT_MODE" )
                mLtMode = (FdoLtLockModeType)(optRdr->GetValue().ToLong());
            else if ( optName == L"LOCKING_MODE" ) 
                mLckMode = (FdoLtLockModeType)(optRdr->GetValue().ToLong());
        }
    }

    LoadLckTypes(mLckMode);
}

void FdoSmPhOwner::LoadLckTypes( FdoLtLockModeType lockMode )
{
    FdoLockType* noLockTypes = new FdoLockType[1];
    noLockTypes[0] = FdoLockType_Transaction;
    FdoSmPhLockTypesP lockTypes = new FdoSmPhLockTypes( NoLtLock, 1, noLockTypes );
    GetLockTypesCollection()->Add( lockTypes );

    if ( lockMode == FdoMode ) {
        FdoLockType* fdoLockTypes = new FdoLockType[3];
        fdoLockTypes[0] = FdoLockType_Transaction;
        fdoLockTypes[1] = FdoLockType_Exclusive;
        fdoLockTypes[2] = FdoLockType_LongTransactionExclusive;

        lockTypes = new FdoSmPhLockTypes( FdoMode, 3, fdoLockTypes );
        GetLockTypesCollection()->Add( lockTypes );
    }
}
   
void FdoSmPhOwner::LoadSchemaInfo()
{
    // Load only if this is an existing owner.
    if ( (!mSchemaInfoLoaded) && 
            ((GetElementState() == FdoSchemaElementState_Unchanged) ||
             (GetElementState() == FdoSchemaElementState_Modified)
            ) 
    ) {
        mSchemaInfoLoaded = true;

        // Read the schema info record for the owner (datastore).
        FdoSmPhSchemaReaderP schemaRdr = new FdoSmPhSchemaReader( FDO_SAFE_ADDREF(this), true );

        if ( schemaRdr->ReadNext() ) {
            SetDescription( schemaRdr->GetDescription() );
            mSchemaVersion = schemaRdr->GetVersion();
        }
    }
}

FdoSmPhDbObjectsP FdoSmPhOwner::GetDbObjects()
{
    if ( !mDbObjects ) 
        // Created object cache.
        mDbObjects = new FdoSmPhDbObjectCollection(NULL);

    return mDbObjects;
}

FdoSmPhLockTypesCollection* FdoSmPhOwner::GetLockTypesCollection()
{
    if ( !mLockTypes ) 
        mLockTypes = new FdoSmPhLockTypesCollection();

    return mLockTypes;
}

void FdoSmPhOwner::LoadSpatialContexts( FdoStringP dbObjectName )
{
    FdoStringP  scInfoTable = GetManager()->GetRealDbObjectName( FdoSmPhMgr::ScInfoNoMetaTable );

    if ( !mSpatialContexts ) {
        mSpatialContexts = new FdoSmPhSpatialContextCollection();
        mSpatialContextGeoms = new FdoSmPhSpatialContextGeomCollection();
        // Make sure ScInfo table is loaded first, since real spatial 
        // context names can be determined from the columns in this table.
        DoLoadSpatialContexts( FdoSmPhMgr::ScInfoNoMetaTable );
    }

    if ( GetManager()->GetBulkLoadSpatialContexts() || (dbObjectName != scInfoTable) ) 
        DoLoadSpatialContexts( GetManager()->GetBulkLoadSpatialContexts() ? FdoStringP() : dbObjectName );
}

void FdoSmPhOwner::DoLoadSpatialContexts( FdoStringP dbObjectName )
{        
    if ( GetElementState() == FdoSchemaElementState_Added ) 
        return;

    if ( !mSpatialContextsLoaded ) {
        FdoInt32 firstScGeom = mSpatialContextGeoms->GetCount();

        // reverse-engineering. The PH schema object will return the appropiate reader or
	    // the default one.
        FdoSmPhRdSpatialContextReaderP scReader;
        
        if ( dbObjectName == L"") {
            // We're either asked to or forced to load all spatial contexts.
            scReader = CreateRdSpatialContextReader();
            mSpatialContextsLoaded = true;
        }
        else {
            // Incremental loading (SC's associated with given dbObject).
            scReader = CreateRdSpatialContextReader(dbObjectName);
        }
    
        bool scFound = false;

        while (scReader->ReadNext())
        {
            scFound = true;

            // Generate physical spatial context from current SpatialContextGeom
            FdoPtr<FdoByteArray> scExtent = scReader->GetExtent();
            FdoSmPhSpatialContextP sc = new FdoSmPhSpatialContext(
                GetManager(),
                scReader->GetSrid(),
                scReader->GetGeomColumnName(),
                scReader->GetDescription(),
                scReader->GetCoordinateSystem(),
                scReader->GetCoordinateSystemWkt(),
                scReader->GetExtentType(),
                scExtent,
                scReader->GetXYTolerance(),
                scReader->GetZTolerance()
            );

            if (NULL == sc.p)
    		    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

            // Create Spatial context geometry object and associate it with this scId
	        FdoSmPhSpatialContextGeomP  scgeom = new FdoSmPhSpatialContextGeom(
                                                            this,
														    scReader->GetGeomTableName(),
														    scReader->GetGeomColumnName(),
														    scReader->GetHasElevation(),
                                                            scReader->GetHasMeasure(),
                                                            scReader->GetGeometryType(),
                                                            scReader->IsDerived(),
                                                            sc,
                                                            mSpatialContexts
            );

            if (NULL == scgeom.p)
			    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

            if ( mSpatialContextGeoms->IndexOf(scgeom->GetName()) < 0 ) 
            {
                mSpatialContextGeoms->Add( scgeom );	
                // For derived SCGeoms, we hit the corresponding table
                // or view that contains the geometry column.
                // Bulk load these tables and views for efficiency.
                if ( scReader->IsDerived() ) 
                    AddCandDbObject(scReader->GetGeomTableName());
            }
        }

        // Get spatial contexts for derived objects. A derived object does not have
        // an explicit correspondence to its columns (it is implicitly related to the
        // columns of its base object). Synonyms are usually derived objects.
        // This means the above won't pick up spatial contexts for geometric columns
        // for derived objects, meaning that a spatial context can be missed for a 
        // synonym that references a table in another datastore. The following finds
        // spatial contexts for these derived objects.

        // If loading spatial contexts for a single dbObject and spatial contexts
        // were found then the dbObject was not derived. Therefore we can skip the 
        // derived object check.
        if ( (dbObjectName == L"") || !scFound ) 
        {
            FdoStringsP derivedObjects = FdoStringCollection::Create();
            FdoSmPhRdDbObjectReaderP objReader = CreateDerivedObjectReader(dbObjectName);
            
            if ( objReader ) 
            {
                //Read and cache all derived object before looking at their columns.
                //This causes their base objects (which have the columns) to be bulk loaded.
                while ( objReader->ReadNext() )
                {
                    CacheDbObject(objReader);
                    derivedObjects->Add( objReader->GetString(L"", L"name") );
                }


                // Check each derived object for geometric column
                for ( int i = 0; i < derivedObjects->GetCount(); i++ ) 
                {
                    FdoSmPhDbObjectP dbObject = FindDbObject(derivedObjects->GetString(i));

                    if ( dbObject ) 
                    {
                        FdoSmPhColumnsP columns = dbObject->GetColumns();

                        for ( int j = 0; j < columns->GetCount(); j++ )
                        {
                            FdoSmPhColumnP column = columns->GetItem(j);
                            FdoSmPhColumnGeomP geomColumn = column->SmartCast<FdoSmPhColumnGeom>();

                            if ( geomColumn )
                            {
                                // Found a geometric column
                                FdoStringP scGeomName = FdoSmPhSpatialContextGeom::MakeName(dbObject->GetName(), geomColumn->GetName());

                                if ( mSpatialContextGeoms->IndexOf(scGeomName) == -1 )
                                {
                                    // Its spatial context info has not yet been cached.
                                    // Cache a dummy spatial context geometry. When the
                                    // scGeoms are resolved (see below) the base column
                                    // is retrieved and its spatial context is added to the 
                                    // cache.
                                    FdoSmPhSpatialContextGeomP  scgeom = new FdoSmPhSpatialContextGeom(
                                                                                    this,
													                                dbObject->GetName(),
													                                geomColumn->GetName(),
													                                false,
                                                                                    false,
                                                                                    0,
                                                                                    true,
                                                                                    NULL,
                                                                                    mSpatialContexts
                                    );
                    
                                    mSpatialContextGeoms->Add( scgeom );	
                                }
                            }
                        }
                    }
                }
            }
        }
        

        // Resolve the SCGeoms that were loaded to their spatial contexts. This is triggered
        // by doing a GetSpatialContext on each SCGeom. This does the following:
        //  - coalesce and cache spatial contexts in mSpatialContexts
        //  - associates each derived SCGeom to the associated spatial context of its
        //    root SCGeom. For example, this is done for SQLServerSpatial to associate
        //    geometry columns in views to their spatial contexts, by associating to the 
        //    spatial context for the root column.

        FdoInt32 lastScGeom = mSpatialContextGeoms->GetCount() - 1;
        for ( FdoInt32 i = firstScGeom; i <= lastScGeom; i++ )
        {
            FdoSmPhSpatialContextGeomP  scgeom = mSpatialContextGeoms->GetItem(i);
            FdoSmPhSpatialContextP sc = scgeom->GetSpatialContext();
        }

        // If an scgeom for a derived object does not get resolve them it is dangling
        // (null spatial context). Remove these scgeoms.
        for ( FdoInt32 i = lastScGeom; i >= firstScGeom; i-- )
        {
            FdoSmPhSpatialContextGeomP  scgeom = mSpatialContextGeoms->GetItem(i);
            if ( !scgeom->GetSpatialContext() )
                mSpatialContextGeoms->RemoveAt(i);
        }
    }
}

void FdoSmPhOwner::LoadCoordinateSystems( FdoSmPhRdCoordSysReaderP rdr )
{
    if ( !mCoordinateSystems ) 
        mCoordinateSystems = new FdoSmPhCoordinateSystemCollection();

    while ( rdr->ReadNext() ) {
        FdoSmPhCoordinateSystemP coordSys = new FdoSmPhCoordinateSystem(
            GetManager(),
            rdr->GetString(L"", L"name"),
            L"",
            rdr->GetInt64(L"", L"srid"),
            rdr->GetString(L"", L"wktext")
        );

        // Add the coordinate system to the cache if not already present.
        if ( mCoordinateSystems->IndexOf(coordSys->GetName()) < 0 )
            mCoordinateSystems->Add( coordSys );
    }
}
