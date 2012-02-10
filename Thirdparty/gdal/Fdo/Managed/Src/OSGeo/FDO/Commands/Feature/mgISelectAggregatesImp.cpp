/*
* 
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
#include <Fdo\Commands\Feature\ISelectAggregates.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\Feature\mgISelectAggregatesImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\mgIdentifierCollection.h"
#include "FDO\Commands\\Feature\mgIDataReaderImp.h"
#include "FDO\Filter\mgFilter.h"

FdoISelectAggregates* NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::GetImpObj()
{
    return static_cast<FdoISelectAggregates*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReader^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::Execute()
{
	FdoIDataReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->Execute())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDataReader(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::Distinct::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDistinct(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::Distinct::get()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetDistinct())
	
	return result;
}

NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::Grouping::get()
{
	FdoIdentifierCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGrouping())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifierCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::GroupingFilter::set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ filter)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGroupingFilter(filter->GetImpObj()))
}
 
NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::GroupingFilter::get()
{
	FdoFilter* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGroupingFilter())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFilter(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::JoinCriteria::get()
{
	FdoJoinCriteriaCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetJoinCriteria())

	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::JoinCriteriaCollection^>(ObjectFactory::CreateJoinCriteriaCollection(IntPtr(result), true));
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::Alias::get()
{
	FdoString* unstr;
    EXCEPTION_HANDLER(unstr = GetImpObj()->GetAlias())
	return CHECK_STRING(unstr);
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp::Alias::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetAlias(StringToUni(value)))
}
