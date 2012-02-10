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
#include <Sm/Ph/Rd/IndexReader.h>

FdoSmPhRdIndexReader::FdoSmPhRdIndexReader(
    FdoSmPhReaderP reader
) :
	FdoSmPhRdReader(reader)
{
}


FdoSmPhRdIndexReader::FdoSmPhRdIndexReader(
    FdoSmPhMgrP         mgr,
    FdoSmPhRowsP        rows
) :
	FdoSmPhRdReader(mgr, rows)
{
}


FdoSmPhRdIndexReader::~FdoSmPhRdIndexReader(void)
{
}

FdoSmPhIndexType FdoSmPhRdIndexReader::GetIndexType()
{
    return FdoSmPhIndexType_Scalar;
}

FdoSmPhRowsP FdoSmPhRdIndexReader::MakeRows( FdoSmPhMgrP mgr )
{
    FdoSmPhRowsP rows = new FdoSmPhRowCollection();

    // Single row, no joins
    FdoSmPhRowP row = new FdoSmPhRow( mgr, L"IndexColumns" );
    rows->Add(row);
    
    // Each field adds itself to the row.
    FdoSmPhFieldP field = new FdoSmPhField(
        row, 
        L"index_name",
        row->CreateColumnDbObject(L"index_name",false)
    );

    field = new FdoSmPhField(
        row, 
        L"table_name",
        row->CreateColumnDbObject(L"table_name",false)
    );

    field = new FdoSmPhField(
        row, 
        L"column_name",
        row->CreateColumnDbObject(L"column_name",false)
    );

    field = new FdoSmPhField(
        row, 
        L"uniqueness",
        row->CreateColumnBool(L"uniqueness",false)
    );

    field = new FdoSmPhField(
        row, 
        L"index_type",
        row->CreateColumnBool(L"index_type",false)
    );

    return( rows );
}

