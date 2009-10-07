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
#include "Common\mgStringElement.h"
#include "Common\mgException.h"

NAMESPACE_OSGEO_COMMON::StringElement::StringElement(System::String* value) : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoStringElement::Create(FdoStringP(StringToUni(value))), true))
}

NAMESPACE_OSGEO_COMMON::StringElement::StringElement(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoStringElement* NAMESPACE_OSGEO_COMMON::StringElement::GetImpObj()
{
	return static_cast<FdoStringElement*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_COMMON::StringElement::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

System::String* NAMESPACE_OSGEO_COMMON::StringElement::get_String()
{
	FdoString *str = NULL;
	EXCEPTION_HANDLER(str = GetImpObj()->GetString())
	return str;
}
