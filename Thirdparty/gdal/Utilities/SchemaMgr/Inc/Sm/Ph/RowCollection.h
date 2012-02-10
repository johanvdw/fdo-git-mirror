#ifndef FDOSMPHROWCOLLECTION_H
#define FDOSMPHROWCOLLECTION_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/../NamedCollection.h>
#include <Sm/Ph/Row.h>

// Represents a collection of Row Objects.

class FdoSmPhRowCollection : public FdoSmNamedCollection<FdoSmPhRow>
{
public:
	FdoSmPhRowCollection(void) :
		FdoSmNamedCollection<FdoSmPhRow>(NULL, false)
	{}
	~FdoSmPhRowCollection(void) 
    {
    }

    FdoSmPhFieldP GetField( FdoStringP tableName, FdoStringP fieldName )
    {
        FdoSmPhFieldP field;
        FdoInt32 cidx;

        if ( tableName.GetLength() > 0 ) {
            FdoSmPhRowP row = GetItem( tableName );
			if ( row )
			{
				FdoSmPhFieldsP fields = row->GetFields();
				field = fields->GetItem( fieldName );
			}
        }
        else {
            for ( cidx = 0; cidx < GetCount(); cidx++ ) {
                FdoSmPhRowP row = GetItem(cidx);
				FdoSmPhFieldsP fields = row->GetFields();
                field = fields->FindItem( fieldName );
                if ( field ) 
                    break;
            }
        }

        return( field );
    }

    // Clears the field values for all rows in this collection.
    void Clear()
    {
        FdoInt32 cidx;
        for ( cidx = 0; cidx < GetCount(); cidx++ ) {
            FdoSmPhRowP row = GetItem(cidx);
            row->Clear();
        }
    }

};

typedef FdoPtr<FdoSmPhRowCollection> FdoSmPhRowsP;

#endif


