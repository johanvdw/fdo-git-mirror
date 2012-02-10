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
#include "View.h"
#include "Mgr.h"

FdoSmPhMySqlView::FdoSmPhMySqlView(
    FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdView(viewName, rootDatabase, rootOwner, rootObjectName, pOwner, elementState),
    FdoSmPhMySqlDbObject(viewName, pOwner, reader),
    FdoSmPhDbObject(viewName, pOwner, elementState)
{
}

FdoSmPhMySqlView::~FdoSmPhMySqlView(void)
{
}

FdoStringP FdoSmPhMySqlView::GetRootNameSql()
{
    FdoStringP rootDatabase = this->GetRootDatabase();
    FdoStringP rootOwner = this->GetRootOwner();
    FdoStringP rootObject = this->GetRootObjectName();

    if ( rootDatabase != L"" ) 
        throw FdoSchemaException::Create( 
            NlsMsgGet4(
	            FDORDBMS_148,
                "Cannot create view on %1$ls.%2$ls.%3$ls; %4$ls Provider does not support views on objects on other database servers.",
                (FdoString*) rootDatabase,
                (FdoString*) rootOwner,
                (FdoString*) rootObject,
                L"MySQL"
            )
       );

    return FdoStringP::Format(
        L"%ls%ls%ls\"%ls\"",
        (rootOwner == L"" ) ? L"" : L"\"",
        (FdoString*) rootOwner,
        (rootOwner == L"" ) ? L"" : L"\".",
        (FdoString*) rootObject
    );
}
