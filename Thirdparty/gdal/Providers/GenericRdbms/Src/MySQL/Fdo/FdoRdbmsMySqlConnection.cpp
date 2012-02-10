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

#include <malloc.h>

#include "FdoRdbmsMySqlConnectionInfo.h"

#include "FdoRdbmsMySqlConnection.h"
#include "FdoRdbmsMySqlCreateDataStore.h"
#include "FdoRdbmsMySqlDeleteDataStore.h"
#include "FdoRdbmsMySqlFilterProcessor.h"
#include "../SchemaMgr/SchemaManager.h"
#include "../SchemaMgr/Ph/Mgr.h"
#include "FdoRdbmsMySqlGeometryCapabilities.h"
#include "FdoRdbmsMySqlSchemaCapabilities.h"
#include "FdoRdbmsMySqlConnectionCapabilities.h"
#include "FdoRdbmsMySqlCommandCapabilities.h"
#include "FdoRdbmsMySqlFilterCapabilities.h"
#include "Rdbms/FdoRdbmsCommandType.h"

#include "DbiConnection.h"
#include <Inc/Rdbi/proto.h>
extern initializer mysql_rdbi_init;

wchar_t* getComDir (); // in MySql.cpp
#define OD_MAX_RETRY_COUNT 10

FdoRdbmsMySqlConnection::FdoRdbmsMySqlConnection():
mFilterProcessor( NULL ),
mConnectionInfo(NULL)
{
}

FdoRdbmsMySqlConnection::~FdoRdbmsMySqlConnection ()
{
    if( mFilterProcessor )
        delete mFilterProcessor;

    FDO_SAFE_RELEASE(mConnectionInfo);
}

FdoRdbmsMySqlConnection* FdoRdbmsMySqlConnection::Create()
{
    FdoRdbmsMySqlConnection   *conn = new FdoRdbmsMySqlConnection();
    conn->GetDbiConnection()->InitRdbi( mysql_rdbi_init );
	return conn;
}

FdoICommand *FdoRdbmsMySqlConnection::CreateCommand (FdoInt32 commandType)
{
    FdoICommand* ret;

    switch (commandType)
    {
        case FdoCommandType_CreateDataStore:
            ret = new FdoRdbmsMySqlCreateDataStore (this);
            break;

        case FdoCommandType_DestroyDataStore:
             ret = new FdoRdbmsMySqlDeleteDataStore (this);
             break;

		case FdoCommandType_ActivateSpatialContext:
		case FdoCommandType_AcquireLock:
		case FdoCommandType_GetLockInfo:
		case FdoCommandType_GetLockedObjects:
		case FdoCommandType_GetLockOwners:
		case FdoCommandType_ReleaseLock:
		case FdoCommandType_ActivateLongTransaction:
		case FdoCommandType_DeactivateLongTransaction:
		case FdoCommandType_CommitLongTransaction:
		case FdoCommandType_CreateLongTransaction:
		case FdoCommandType_GetLongTransactions:
		case FdoCommandType_RollbackLongTransaction:
        case FdoRdbmsCommandType_CreateSpatialIndex:
        case FdoRdbmsCommandType_DestroySpatialIndex:
        case FdoRdbmsCommandType_GetSpatialIndexes:
			throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_10, "Command not supported"));
			break;

		 default:
             return FdoRdbmsConnection::CreateCommand( commandType );
    }
    return (ret);
}

FdoRdbmsFilterProcessor* FdoRdbmsMySqlConnection::GetFilterProcessor()
{
    if( mFilterProcessor == NULL )
        mFilterProcessor = new FdoRdbmsMySqlFilterProcessor( this );

    return FDO_SAFE_ADDREF(mFilterProcessor);
}

FdoIConnectionInfo* FdoRdbmsMySqlConnection::GetConnectionInfo()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new FdoRdbmsMySqlConnectionInfo(this);
    FDO_SAFE_ADDREF(mConnectionInfo);
    return mConnectionInfo;
}

FdoIConnectionCapabilities* FdoRdbmsMySqlConnection::GetConnectionCapabilities()
{
    if (mConnectionCapabilities == NULL)
        mConnectionCapabilities = new FdoRdbmsMySqlConnectionCapabilities();
    FDO_SAFE_ADDREF(mConnectionCapabilities);
    return mConnectionCapabilities;
}

/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* FdoRdbmsMySqlConnection::GetGeometryCapabilities()
{
    if( mGeometryCapabilities == NULL )
        mGeometryCapabilities = new FdoRdbmsMySqlGeometryCapabilities();

    return FDO_SAFE_ADDREF(mGeometryCapabilities);
}

/// Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.
FdoISchemaCapabilities* FdoRdbmsMySqlConnection::GetSchemaCapabilities()
{
    if (mSchemaCapabilities == NULL)
        mSchemaCapabilities = new FdoRdbmsMySqlSchemaCapabilities();
    FDO_SAFE_ADDREF(mSchemaCapabilities);
    return mSchemaCapabilities;
}

FdoSchemaManagerP FdoRdbmsMySqlConnection::NewSchemaManager(
    GdbiConnection* gdbiConnection, 
    FdoStringP currUser,
    FdoStringP currSchema
)
{
    FdoSchemaManagerP schMgr = new FdoMySqlSchemaManager(
        gdbiConnection,
        currSchema
    );

    FdoSmPhMySqlMgrP physMgr = schMgr->GetPhysicalSchema()->SmartCast<FdoSmPhMySqlMgr>();

    // Pass down the location of the COM directory for MetaSchema creation scripts.
	physMgr->SetHomeDir( FdoStringP(getComDir()) );

    return schMgr;
}

 //
// Converts a MySql string date of a specific format to a FdoDateTime (time_t) format.
FdoDateTime  FdoRdbmsMySqlConnection::DbiToFdoTime( const char* timeStr )
{
    FdoDateTime fdoTime;
    int year, month, day, hour, minute, seconds;
    year = month = day = hour = minute = seconds = 0;

    if( timeStr != NULL && *timeStr != '\0' )
    {
        int count = sscanf(timeStr,"%4d-%02d-%02d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &seconds);     
        if( count != 6 )
            count = sscanf(timeStr,"%4d-%02d-%02d",&year, &month, &day);
    }
    fdoTime.year = (FdoInt16)year;
    fdoTime.month = (FdoByte)month;
    fdoTime.day = (FdoByte)day;
    fdoTime.hour = (FdoByte)hour;
    fdoTime.minute = (FdoByte)minute;
    fdoTime.seconds = (float)seconds;
    return fdoTime;
}

FdoDateTime  FdoRdbmsMySqlConnection::DbiToFdoTime( const wchar_t* timeStr )
{
    FdoDateTime fdoTime;
    int year, month, day, hour, minute, seconds;
    year = month = day = hour = minute = seconds = 0;

    if( timeStr != NULL && *timeStr != '\0' )
    {
        int count = swscanf(timeStr, L"%4d-%02d-%02d %02d:%02d:%02d", &year, &month, &day, &hour, &minute, &seconds);     
        if( count != 6 )
            count = swscanf(timeStr, L"%4d-%02d-%02d",&year, &month, &day);
    }
    fdoTime.year = (FdoInt16)year;
    fdoTime.month = (FdoByte)month;
    fdoTime.day = (FdoByte)day;
    fdoTime.hour = (FdoByte)hour;
    fdoTime.minute = (FdoByte)minute;
    fdoTime.seconds = (float)seconds;
    return fdoTime;
}

//
// Convert time_t( FdoDateTime ) to a MySql string date of the form.
// It return a statically allocated storage that can be overwritten by subsequent call to this or other methods.
const char* FdoRdbmsMySqlConnection::FdoToDbiTime( FdoDateTime  when )
{
    char *ret = GetDbiConnection()->GetUtility()->newCharP();

	bool isDateSupplied = ((when.year != -1) || (when.month != -1) || (when.day != -1));
    bool isValidDate    = isDateSupplied && ((when.year != -1) && (when.month != -1) && (when.day != -1));

	bool isTimeSupplied = ((when.hour != -1) || (when.minute != -1));
    bool isValidTime    = isTimeSupplied && ((when.hour != -1) && (when.minute != -1));

	if ((isDateSupplied  && !isValidDate)    ||
        (isTimeSupplied  && !isValidTime)    ||
        (!isDateSupplied && !isTimeSupplied)    )
		 throw FdoException::Create(NlsMsgGet(FDORDBMS_480,
                                              "Incomplete date/time setting."));

    if (isDateSupplied) 
	{
		if (!isTimeSupplied)
		{
			// "1979-11-30"
			sprintf (ret, "%4d-%02d-%02d",
				when.year,
				when.month,
				when.day);
		}
		else
        {
            // "1979-11-30 00:00:00"
            sprintf (ret, "%4d-%02d-%02d %02d:%02d:%02.2f",
                when.year,
                when.month,
                when.day,
                when.hour,
                when.minute,
                when.seconds);
        }
    }
    else
    {
		// MySQL accepts time only dates, but it needs the date fields too,
		// otherwise time could be interpreted as date '2010-45-45' which is 
		// illegal and therefore changed to '0000-00-00'.

		// "0000-00-00 10:45:05"
		sprintf (ret, "0000-00-00 %02d:%02d:%02.2f",
				when.hour,
				when.minute,
				when.seconds);
    }

    return (ret);
}

FdoConnectionState FdoRdbmsMySqlConnection::Open()
{
 	if( GetConnectionState() != FdoConnectionState_Open )
	{
  	    FdoRdbmsConnection::Open();
    }

    return GetConnectionState();
}

// Creates a Long Transaction Manager and its corresponding Long Transaction
// Manager Service.
FdoRdbmsLongTransactionManager *FdoRdbmsMySqlConnection::CreateLongTransactionManager()
{

    // There is no long transaction support in MySQL.

    return NULL;

}

FdoIDataStorePropertyDictionary*  FdoRdbmsMySqlConnection::CreateDataStoreProperties( int action )
{
    FdoCommonDataStorePropDictionary* mDataStorePropertyDictionary = new FdoCommonDataStorePropDictionary(this);
	
    FdoPtr<ConnectionProperty> newProp;
	if ( action == FDO_RDBMS_DATASTORE_FOR_READ )
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", false, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
        
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_DESCRIPTION, NlsMsgGet(FDORDBMS_448, "Description"), L"", false, false, false, false, false, false, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
		
    	wchar_t **LtModeValues = new wchar_t*[2];
        LtModeValues[0] = new wchar_t[4];
        LtModeValues[1] = new wchar_t[5];
        wcscpy(LtModeValues[0], L"FDO");
        wcscpy(LtModeValues[1], L"NONE");
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_LTMODE, NlsMsgGet(FDORDBMS_449, "LtMode"), L"FDO", false, false, true, false, false, false, false, 2, (const wchar_t**)LtModeValues);
        mDataStorePropertyDictionary->AddProperty(newProp);
		
    	wchar_t **LockModeValues = new wchar_t*[2];
        LockModeValues[0] = new wchar_t[4];
        LockModeValues[1] = new wchar_t[5];
        wcscpy(LockModeValues[0], L"FDO");
        wcscpy(LockModeValues[1], L"NONE");
        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_LOCKMODE, NlsMsgGet(FDORDBMS_450, "LockMode"), L"FDO", false, false, true, false, false, false, false, 2, (const wchar_t**)LockModeValues);
        mDataStorePropertyDictionary->AddProperty(newProp);
	}
	else if ( action == FDO_RDBMS_DATASTORE_FOR_CREATE )
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", true, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);

        newProp = new ConnectionProperty (FDO_RDBMS_DATASTORE_DESCRIPTION, NlsMsgGet(FDORDBMS_448, "Description"), L"", false, false, false, false, false, false, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
	}
	else if ( action == FDO_RDBMS_DATASTORE_FOR_DELETE )
	{
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", true, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
	}

    return mDataStorePropertyDictionary;
}
FdoRdbmsLockManager *FdoRdbmsMySqlConnection::CreateLockManager()
{
	return NULL;
}

FdoICommandCapabilities* FdoRdbmsMySqlConnection::GetCommandCapabilities()
{
    if (mCommandCapabilities == NULL)
        mCommandCapabilities = new FdoRdbmsMySqlCommandCapabilities();
    FDO_SAFE_ADDREF(mCommandCapabilities);
    return mCommandCapabilities;
}

FdoIFilterCapabilities *FdoRdbmsMySqlConnection::GetFilterCapabilities()
{
    if (mFilterCapabilities == NULL)
        mFilterCapabilities = new FdoRdbmsMysqlFilterCapabilities();
    FDO_SAFE_ADDREF(mFilterCapabilities);
    return mFilterCapabilities;	
}
