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

#include "stdafx.h"
#include "Index.h"

FdoSmPhSqsIndex::FdoSmPhSqsIndex(
        FdoStringP name,
        const FdoSmPhTable* pTable,
        bool isUnique,
        FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdIndex( name, pTable, isUnique, elementState ),
    FdoSmPhSqsDbObject( name, (const FdoSmPhOwner*) pTable->GetParent() , reader),
    FdoSmPhDbObject( name, (const FdoSmPhOwner*) pTable->GetParent(), elementState )
{
    //TODO; //TODO: handle index-specific stuff from pDbObjectReader ?
}

FdoSmPhSqsIndex::~FdoSmPhSqsIndex(void)
{
}

// FdoSmPhSqsDbObject::GetDbName overridden since 
// index names in SQL statements must not be qualified
// by user name. 
FdoStringP FdoSmPhSqsIndex::GetDbName() const
{
    FdoStringP dbName = FdoStringP(L"\"") + GetName() + L"\"";

    return dbName;
}



FdoPtr<FdoSmPhRdColumnReader> FdoSmPhSqsIndex::CreateColumnReader()
{
    return FdoSmPhIndex::CreateColumnReader();
}

bool FdoSmPhSqsIndex::Add()
{
    const FdoSmPhTable* table = RefTable();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create %lsindex %ls on %ls ( %ls )",
        GetIsUnique() ? L"unique " : L"",
        (FdoString*) GetDbName(),
        (FdoString*) table->GetDbQName(),
        (FdoString*) GetKeyColsSql(GetColumns())->ToString( L", " )
    );

    // SqlServer does not allow qualified name for index to create.
    // Therefore, must switch to the index's owning database before creating
    // it.
    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

bool FdoSmPhSqsIndex::Delete()
{
    const FdoSmPhTable* table = RefTable();

    // Index name must be qualified by table name.
    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop index %ls.%ls",
        (FdoString*) ((FdoSmPhTable*)table)->GetDbName(),
        (FdoString*) GetDbName()
    );

    // SqlServer does not allow qualified name for index to create.
    // Therefore, must switch to the index's owning database before deleting
    // it.
    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

