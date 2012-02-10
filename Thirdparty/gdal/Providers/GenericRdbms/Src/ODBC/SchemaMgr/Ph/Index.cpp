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
#include "Index.h"

FdoSmPhOdbcIndex::FdoSmPhOdbcIndex(
        FdoStringP name,
        FdoSmPhDbObject* pParent,
        bool isUnique,
        FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdIndex( name, pParent, isUnique, elementState ),
    FdoSmPhOdbcDbObject( name, (const FdoSmPhOwner*) pParent->GetParent() , reader),
    FdoSmPhDbObject( name, (const FdoSmPhOwner*) pParent->GetParent(), elementState )
{
    //TODO; //TODO: handle index-specific stuff from pDbObjectReader ?
}

FdoSmPhOdbcIndex::~FdoSmPhOdbcIndex(void)
{
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhOdbcIndex::CreateColumnReader()
{
    return FdoSmPhIndex::CreateColumnReader();
}

bool FdoSmPhOdbcIndex::Add()
{
    FdoSmPhDbObjectP dbObject = GetDbObject();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create %lsindex %ls on %ls ( %ls )",
        GetIsUnique() ? L"unique " : L"",
        (FdoString*) GetName(),
        (FdoString*) dbObject->GetDbQName(),
        (FdoString*) GetKeyColsSql(GetColumns())->ToString( L", " )
    );

    // Odbc does not allow qualified name for index to create.
    // Therefore, must switch to the index's owning database before creating
    // it.
    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

bool FdoSmPhOdbcIndex::Delete()
{
    FdoSmPhDbObjectP dbObject = GetDbObject();

    // Index name must be qualified by table name.
    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop index dbo.%ls.%ls",
        (FdoString*) dbObject->GetName(),
        (FdoString*) GetName()
    );

    // Odbc does not allow qualified name for index to create.
    // Therefore, must switch to the index's owning database before deleting
    // it.
    ActivateOwnerAndExecute( sqlStmt );

    return true;
}
