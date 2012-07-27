#ifndef FDOSMPHSQSCOLUMNGEOM_H
#define FDOSMPHSQSCOLUMNGEOM_H      1
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

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnGeom.h>
#include "Column.h"
#include "Owner.h"

// Represents a SqlServer geometric type column.
class FdoSmPhSqsColumnGeom :
    public FdoSmPhColumnGeom, public FdoSmPhSqsColumn
{
public:
    FdoSmPhSqsColumnGeom(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        FdoSmPhScInfoP AssociatedSCInfo,
        bool bNullable = true,
        bool bHasElevation = true,
        bool bHasMeasure = false,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL
    );

    virtual ~FdoSmPhSqsColumnGeom(void);

    virtual int GetRdbType()
    {
        return RDBI_GEOMETRY;
    }

    virtual FdoInt64 GetSRID();

    virtual void SetElementState(FdoSchemaElementState elementState);

    // Creates a spatial index for this column. Throws an exception if column
    // already indexed.
    FdoPtr<FdoSmPhSpatialIndex> CreateSpatialIndex( 
        FdoStringP indexName = L"" 
            // Spatial index name. If L"", a name is autogenerated.
    );

    // Regenerate spatial index by dropping and re-creating it.
    virtual void    RegenSpatialIndex();


protected:
    // unused constructor needed only to build on Linux
    FdoSmPhSqsColumnGeom() {}

    virtual bool Add(); 

    // Encodes SRID into name for spatial index.
    virtual FdoStringP UniqueIndexName();

    // Parses SRID out of spatial index name. Returns -1 if SRID was not encoded into name.
    FdoInt64 IndexName2Srid();

    virtual void PostFinalize();

private:
  	FdoInt64 mSRID;
};

typedef FdoPtr<FdoSmPhSqsColumnGeom> FdoSmPhSqsColumnGeomP;

#endif
