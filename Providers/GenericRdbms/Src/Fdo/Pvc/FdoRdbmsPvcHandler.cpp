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
#include "Fdo/Pvc/FdoRdbmsPvcHandler.h"
#include <Sm/Ph/Column.h>
#include <math.h>

void FdoRdbmsPvcHandler::SetGeomOrdinateBindValue( 
    char* buffer, 
    double doubleValue, 
    const FdoSmPhColumn* columnDefinition 
)
{
    FdoSmPhColType colType = columnDefinition ? columnDefinition->GetType() : FdoSmPhColType_Double;
    int scale = 8;
    
    switch ( colType ) {
    case FdoSmPhColType_Decimal:
        // Round decimals to defined scale
        scale = columnDefinition->GetScale();
        sprintf(buffer, "%.*f", scale, doubleValue);
        break;
    case FdoSmPhColType_Single:
        sprintf(buffer, "%.8g", doubleValue);
        break;
    case FdoSmPhColType_Byte:
    case FdoSmPhColType_Int16:
    case FdoSmPhColType_Int32:
    case FdoSmPhColType_Int64:
        // Integral type columns possible, though Byte and In16 would be rather imprecise for geometry ordinates.
        // Round to integral value. Leave it to RDBMS to catch out of range values. 
        sprintf(buffer, "%.0f", doubleValue);
        break;
    default:
        // Double or Unknown type. Assume double precision.
        sprintf(buffer, "%.16g", doubleValue);
        break;
    }
}

FdoPtr<FdoDataValue> FdoRdbmsPvcHandler::GetGeomOrdinateBindValue( 
    double doubleValue, 
    const FdoSmPhColumn* columnDefinition 
)
{
    FdoSmPhColType colType = columnDefinition ? columnDefinition->GetType() : FdoSmPhColType_Double;

    FdoPtr<FdoDataValue> ret = FdoDoubleValue::Create(doubleValue);

    int scale = 8;

    switch ( colType ) {
    case FdoSmPhColType_Decimal:
        // Round decimals to defined scale
        {
            scale = columnDefinition->GetScale();
            double shift = (doubleValue < 0) ? -0.5 : 0.5;
            double mult = pow((double)10, (double)scale);
            ret = FdoDoubleValue::Create( ((FdoDouble)(FdoInt64)(doubleValue * mult + shift)) / mult);
        }
        break;
    case FdoSmPhColType_Single:
        ret = FdoSingleValue::Create(ret);
        break;
    case FdoSmPhColType_Byte:
        ret = FdoByteValue::Create(ret);
        break;
    case FdoSmPhColType_Int16:
        ret = FdoInt16Value::Create(ret);
        break;
    case FdoSmPhColType_Int32:
        ret = FdoInt32Value::Create(ret);
        break;
    case FdoSmPhColType_Int64:
        ret = FdoInt64Value::Create(ret);
        break;
    default:
        // Double or unknown type, keep it as Double.
        break;
    }

    return ret;
}
