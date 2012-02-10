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
#include <Rdbms/Override/ODBC/OdbcOvDataPropertyDefinition.h>

FdoOdbcOvDataPropertyDefinition* FdoOdbcOvDataPropertyDefinition::Create()
{
    return new FdoOdbcOvDataPropertyDefinition();
}

FdoOdbcOvDataPropertyDefinition* FdoOdbcOvDataPropertyDefinition::Create(FdoString* name)
{
    return new FdoOdbcOvDataPropertyDefinition(name);
}


FdoOdbcOvDataPropertyDefinition::FdoOdbcOvDataPropertyDefinition()
{
}

FdoOdbcOvDataPropertyDefinition::FdoOdbcOvDataPropertyDefinition(FdoString* name) :
    FdoRdbmsOvPropertyDefinition(name),
    FdoRdbmsOvDataPropertyDefinition(name),
    FdoOdbcOvPropertyDefinition(name)
{
}

FdoOdbcOvDataPropertyDefinition::~FdoOdbcOvDataPropertyDefinition()
{
}

FdoOdbcOvColumn* FdoOdbcOvDataPropertyDefinition::GetColumn()
{
    return (FdoOdbcOvColumn*) FdoRdbmsOvDataPropertyDefinition::GetColumn();
}

void FdoOdbcOvDataPropertyDefinition::SetColumn(FdoOdbcOvColumn *column)
{
    FdoRdbmsOvDataPropertyDefinition::SetColumn(column);
}

void FdoOdbcOvDataPropertyDefinition::Dispose()
{
    delete this;
}
