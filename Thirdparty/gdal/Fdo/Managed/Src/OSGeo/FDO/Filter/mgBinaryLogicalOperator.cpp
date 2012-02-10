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
#include <Fdo\Filter\BinaryLogicalOperator.h>
#include <Fdo\Filter\BinaryLogicalOperations.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Filter\mgBinaryLogicalOperator.h"
#include "FDO\Filter\mgBinaryLogicalOperations.h"
#include "FDO\Filter\mgFilter.h"
#include "FDO\Filter\mgIFilterProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::BinaryLogicalOperator(System::IntPtr unmanaged, System::Boolean autoDelete) : LogicalOperator(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::BinaryLogicalOperator() : LogicalOperator(IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(IntPtr(FdoBinaryLogicalOperator::Create()), true))
}

NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::BinaryLogicalOperator(NAMESPACE_OSGEO_FDO_FILTER::Filter^ leftOperand, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, NAMESPACE_OSGEO_FDO_FILTER::Filter^ rightOperand) : LogicalOperator(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoBinaryLogicalOperator::Create(leftOperand->GetImpObj(), static_cast<FdoBinaryLogicalOperations>(operation), rightOperand->GetImpObj())), true))
}

FdoBinaryLogicalOperator* NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::GetImpObj()
{
	return static_cast<FdoBinaryLogicalOperator*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::Operation::get()
{
	FdoBinaryLogicalOperations unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetOperation())
	return static_cast<BinaryLogicalOperations>(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::Operation::set(NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOperation(static_cast<FdoBinaryLogicalOperations>(value)))
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::LeftOperand::get()
{
	FdoFilter* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetLeftOperand())
	return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(ObjectFactory::CreateFilter(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::LeftOperand::set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLeftOperand((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::RightOperand::get()
{
	FdoFilter* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetRightOperand())
	return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(ObjectFactory::CreateFilter(IntPtr(result), true));
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::RightOperand::set(NAMESPACE_OSGEO_FDO_FILTER::Filter^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetRightOperand((value == nullptr ? nullptr : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator::Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessorImp^>(processor))->GetImpObj()))
}
