//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"
#include "PostGisProvider.h"
#include "PgUtility.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/algorithm/string/predicate.hpp>

namespace
{
    const int PgVarHeaderSize = 4;
}

namespace fdo { namespace postgis { namespace details {

FdoDataType FdoTypeFromPgTypeName(std::string const& typeName)
{
    using boost::algorithm::iequals;
    
    FdoDataType fdoType;

    if (iequals(typeName, "bool"))
    {
        fdoType = FdoDataType_Boolean;
    }
    else if (iequals(typeName, "int2"))
    {
        fdoType = FdoDataType_Int16;
    }
    else if (iequals(typeName, "int4"))
    {
        fdoType = FdoDataType_Int32;
    }
    else if (iequals(typeName, "int8"))
    {
        fdoType = FdoDataType_Int64;
    }
    else if (iequals(typeName, "float4"))
    {
        fdoType = FdoDataType_Single;
    }
    else if (iequals(typeName, "float8"))
    {
        fdoType = FdoDataType_Double;
    }
    else if (iequals(typeName, "numeric"))
    {
        fdoType = FdoDataType_Decimal;
    }
    else if (iequals(typeName, "char") || iequals(typeName, "bpchar")
        || iequals(typeName, "varchar") || iequals(typeName, "text")
        || iequals(typeName, "cstring"))
    {
        fdoType = FdoDataType_String;
    }
    else if (iequals(typeName, "abstime") || iequals(typeName, "reltime")
        || iequals(typeName, "date") || iequals(typeName, "time")
        || iequals(typeName, "timestamp") || iequals(typeName, "timestampz") 
        || iequals(typeName, "timetz"))
    {
        fdoType = FdoDataType_DateTime;
    }
    else if (iequals(typeName, "bytea"))
    {
        assert(!"TYPE MAPPING TO BE REVIEWED");
        fdoType = FdoDataType_BLOB;
    }
    else if (iequals(typeName, "oid"))
    {
        assert(!"TYPE MAPPING TO BE REVIEWED");
        fdoType = FdoDataType_Int32;
    }
    else
    {
        if (iequals(typeName, "geometry"))
        {
            assert(!"GEOMETRY DOES NOT MAP TO FdoDataType");
        }
        
        // TODO: Add mapping of FdoDataType_Byte and FdoDataType_CLOB
        
        FdoStringP tmp(typeName.c_str());
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_UNSUPPORTED_DATATYPE,
            "The '%1$ls' data type is not supported by PostGIS provider.",
            static_cast<FdoString*>(tmp)));
    }

    return fdoType;
}

int GetTypeMaxSize(int const& modifier)
{
    return (modifier - PgVarHeaderSize);
}

int GetTypePrecision(int const& modifier)
{
    return ((modifier - PgVarHeaderSize) >> 16);
}

int GetTypeScale(int const& modifier)
{
    return ((modifier - PgVarHeaderSize) & 0xffff);
}

}}} // namespace fdo::postgis::details
