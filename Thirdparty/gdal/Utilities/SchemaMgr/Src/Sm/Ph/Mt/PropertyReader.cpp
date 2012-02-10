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
#include <Sm/Error.h>
#include <Sm/Ph/Mt/PropertyReader.h>

FdoSmPhMtPropertyReader::FdoSmPhMtPropertyReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr, FdoStringCollection* classNames) : 
    FdoSmPhReader( MakeReader(froms, schemaName, mgr, classNames) )
{
}

FdoSmPhMtPropertyReader::~FdoSmPhMtPropertyReader(void)
{
}

FdoSmPhReaderP FdoSmPhMtPropertyReader::MakeReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr, FdoStringCollection* classNames)
{
    FdoSmPhRowP from = froms->GetItem(0);
    FdoSmPhRowP fromB = froms->GetItem(1);
    FdoSmPhOwner* owner = static_cast<FdoSmPhOwner*>((FdoSmSchemaElement*)(FdoSmPhDbObjectP(from->GetDbObject())->GetParent()));
    double schemaVersion = owner->GetSchemaVersion();
    FdoStringP classidRow;

    // The following error can happen when a field is added to the property reader's select list without
    // adding it to the fake row (see below).
    // Although the fake row is only added for older datastores, trap this error for all datastores,
    // so that it will be caught by the unit tests. 
    // When this error occurs, you must change the expected counts on the next line to match the 
    // new field counts for the property reader and adjust the select columns in the classidrow
    // clause below to be consistent with the property reader fields.
    if ( (from->GetFields()->GetCount() != 25) || (fromB->GetFields()->GetCount() != 1) ) {
		throw FdoSchemaException::Create (
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_419) 
			)
		);
    }

    if ( schemaVersion > 0.0 && schemaVersion < 3.0 ) {
        // This block will only be executed for the Oracle Provider, since MetaSchema
        // versions 1 and 2 were Oracle Provider only.

        // Version 3 added a property for ClassId and the Rdbms providers will not 
        // find classid columns if this property is not present. In Versions 1 and 2
        // of the Oracle Provider, classid column name was hard-coded; there was
        // no corresponding property in the MetaSchema.

        // The following adds a fake f_attributedefinition row for classid to make
        // a pre-version 3 MetaSchema look more like version 3. This allows more recent
        // versions of the Oracle Provider to work with these older schemas.

        // The fields for the fake row must be in the same order as the fields setup
        // by FdoSmPhPropertyReader::MakeRows().
        classidRow = FdoStringP::Format(
            L" union select classid, tablename, 'ClassId', 0, 'CLASSID', null, 'int64', 'number', 20, 0, 0, 0, 0, 1, '%ls', null, null, 1, 1, 0, 0, 0, 0, null, null, 'Feature' from %ls.f_classdefinition where schemaname = 'F_MetaClass' and classname = 'Feature' ",
            owner->GetName(),
            owner->GetName()
        );
    }
    FdoStringP sWhere;
    int cntCls = (classNames == NULL) ? 0 : classNames->GetCount();
    if (cntCls == 0)
    {
        // Generate the where clause
        sWhere = FdoStringP::Format( 
            L"where f_attributedefinition.classid = f_classdefinition.classid and schemaname = %ls %ls order by %ls, %ls", 
    	    (FdoString*) mgr->FormatSQLVal(schemaName, FdoSmPhColType_String),
            (FdoString*) classidRow,
            (FdoString*) mgr->FormatOrderCol(L"classname", FdoSmPhColType_String),
            (FdoString*) mgr->FormatOrderCol(L"attributename", FdoSmPhColType_String)
        );
    }
    else
    {
        FdoStringP clsInNames;
        for (int i = 0; i < cntCls; i++)
        {
            if (i)
                clsInNames += L",";
            clsInNames += mgr->FormatSQLVal(classNames->GetString(i), FdoSmPhColType_String);
        }
        // Generate the where clause
        sWhere = FdoStringP::Format( 
            L"where f_attributedefinition.classid = f_classdefinition.classid and classname IN(%ls) and schemaname = %ls %ls order by %ls, %ls", 
            (FdoString*) clsInNames,
	        (FdoString*) mgr->FormatSQLVal(schemaName, FdoSmPhColType_String),
            (FdoString*) classidRow,
            (FdoString*) mgr->FormatOrderCol(L"classname", FdoSmPhColType_String),
            (FdoString*) mgr->FormatOrderCol(L"attributename", FdoSmPhColType_String)
        );
    }

    // Create a query reader to wrap around
    FdoSmPhRdQueryReaderP pSubReader = mgr->CreateQueryReader( froms, sWhere );

    return FDO_SAFE_ADDREF( (FdoSmPhRdQueryReader*) pSubReader );
}

