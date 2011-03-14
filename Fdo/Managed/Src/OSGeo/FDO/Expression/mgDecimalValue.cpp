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
#include <Fdo\Expression\DecimalValue.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgDecimalValue.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::DecimalValue(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::DecimalValue() : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDecimalValue::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::DecimalValue(System::Double value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDecimalValue::Create(value)), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::DecimalValue(
    DataValue^ src, 
    System::Boolean nullIfIncompatible,
    System::Boolean shift,
    System::Boolean truncate
) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDecimalValue::Create(src->GetImpObj(), nullIfIncompatible, shift, truncate)), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::DecimalValue(
    DataValue^ src
) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDecimalValue::Create(src->GetImpObj())), true))
}

FdoDecimalValue* NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::GetImpObj()
{
	return static_cast<FdoDecimalValue*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::operator System::Double( NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue^ value )
{
	return (value->GetImpObj())->operator FdoDouble();
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::DataType::get()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Double NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::Decimal::get()
{
	System::Double unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDecimal())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::Decimal::set(System::Double value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDecimal(value))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
