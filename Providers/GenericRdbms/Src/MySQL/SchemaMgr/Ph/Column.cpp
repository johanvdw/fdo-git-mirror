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
#include "Column.h"
#include "DbObject.h"


FdoSmPhMySqlColumn::FdoSmPhMySqlColumn(FdoSmPhRdColumnReader* reader)
{
    if (NULL != reader)
    {
        ; //TODO: read MySQL-specific column attributes from given reader
    }
}



FdoSmPhMySqlColumn::~FdoSmPhMySqlColumn(void)
{
}


FdoStringP FdoSmPhMySqlColumn::GetAddSql()
{
    // Get basic name/type/nullability SQL:
    FdoStringP addSql = FdoSmPhColumn::GetAddSql();

    // Add auto-increment column information:
    const FdoSmPhMySqlDbObject* parent = dynamic_cast<const FdoSmPhMySqlDbObject*>(this->GetParent());
    FdoStringP idColumn = parent->GetAutoIncrementColumnName();
    if (0==wcscmp((FdoString*)idColumn, this->GetName()))
        addSql += L" AUTO_INCREMENT ";

    return addSql;
}

FdoStringP FdoSmPhMySqlColumn::GetAutoincrementSql()
{
    if ( FdoSmPhColumn::GetAutoincrement() )
        return L" AUTO_INCREMENT ";
    else
        return L"";
}
