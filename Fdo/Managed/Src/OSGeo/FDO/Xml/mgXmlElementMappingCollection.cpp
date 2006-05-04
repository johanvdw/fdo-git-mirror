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
#include <Fdo\Xml\ClassMappingCollection.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlElementMappingCollection.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Xml\mgXmlElementMapping.h"
#include "FDO\Commands\Schema\mgPhysicalElementMapping.h"

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
			Detach();
	}
}

NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::XmlElementMappingCollection(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping* parent) : Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlElementMappingCollection::Create(parent->GetImpObj()), true))
}

FdoXmlElementMappingCollection* NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::GetImpObj()
{
    return static_cast<FdoXmlElementMappingCollection*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::ICollection::CopyTo(System::Array* array, System::Int32 index) 
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0;i<this->Count;i++)
	{
		array->set_Item(index + i, get_Item(i));
	}
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::CopyTo(NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* array[], System::Int32 index)
{
	if (NULL == array)
	{
		throw new System::ArgumentNullException();
	}

	if (index < 0)
	{
		throw new System::ArgumentOutOfRangeException();
	}
	if (array->Rank != 1 || index >= array->Length || get_Count() + index > array->Length)
	{
		throw new System::ArgumentException();
	}

	for (System::Int32 i=0;i<this->Count;i++)
	{
		array[index+i] = __try_cast<NAMESPACE_OSGEO_FDO_XML::XmlElementMapping*>(get_Item(i));
	}
}

System::Object* NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::ICollection::get_SyncRoot()
{
	return NULL;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::ICollection::get_IsSynchronized()
{
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IList::get_IsFixedSize() 
{ 
	return false;
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IList::get_IsReadOnly() 
{ 
	return false;
}

System::Object* NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IList::get_Item(System::Int32 index)
{
	return get_RealTypeItem( index );
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IList::set_Item(System::Int32 index, System::Object* value)
{
	set_RealTypeItem(index,  __try_cast<NAMESPACE_OSGEO_FDO_XML::XmlElementMapping*>(value) );
}

System::Int32 NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IList::Add(System::Object* value)
{
	return Add(__try_cast<NAMESPACE_OSGEO_FDO_XML::XmlElementMapping*>(value));
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IList::Contains(System::Object* value)
{
	return Contains(__try_cast<NAMESPACE_OSGEO_FDO_XML::XmlElementMapping*>(value));
}

System::Int32 NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IList::IndexOf(System::Object* value)
{
	return IndexOf(__try_cast<NAMESPACE_OSGEO_FDO_XML::XmlElementMapping*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IList::Insert(System::Int32 index, System::Object* value)
{
	Insert(index,__try_cast<NAMESPACE_OSGEO_FDO_XML::XmlElementMapping*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IList::Remove(System::Object* value)
{
	return Remove(__try_cast<NAMESPACE_OSGEO_FDO_XML::XmlElementMapping*>(value));
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::RemoveAt(System::Int32 index)
{
	EXCEPTION_HANDLER(GetImpObj()->RemoveAt(index))
}

System::Collections::IEnumerator* NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::GetEnumerator()
{
	return new Enumerator(this);
}

System::Int32 NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::get_Count(System::Void)
{
	FdoInt32 length;

	EXCEPTION_HANDLER(length = GetImpObj()->GetCount())

		return length;
}

System::Int32 NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::Add(NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->Add((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Int32 NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::IndexOf(NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* value)
{
	FdoInt32 index;

	EXCEPTION_HANDLER(index = GetImpObj()->IndexOf((value == NULL ? NULL : value->GetImpObj())))

		return index;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Insert(index, (value == NULL ? NULL : value->GetImpObj())))
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::Remove(NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* value)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::Contains(NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* value)
{
	FdoBoolean exist;

	EXCEPTION_HANDLER(exist = !!GetImpObj()->Contains(value == NULL ? NULL : value->GetImpObj()))

	return exist;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}

/*
Implementation for XmlElementMappingCollection::Enumerator
*/ 
System::Object* NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::Enumerator::get_Current()
{
	if (m_nIdx < 0 || m_nIdx >= m_pCol->Count)
	{
		throw new InvalidOperationException();
	}

	FdoXmlElementMapping* upElement;

	EXCEPTION_HANDLER(upElement = m_pCol->GetImpObj()->GetItem(m_nIdx))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlElementMapping(upElement, true);
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::Enumerator::MoveNext()
{
	++m_nIdx;
	return m_nIdx < m_pCol->Count;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::Enumerator::Reset()
{
	m_nIdx = -1;
}

NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::get_RealTypeItem(System::Int32 index)
{
	FdoXmlElementMapping* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(index))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlElementMapping(upElement, true);
}

NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::get_RealTypeItem(System::String* index)
{
	FdoXmlElementMapping* upElement;

	EXCEPTION_HANDLER(upElement = GetImpObj()->GetItem(StringToUni(index)))

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlElementMapping(upElement, true);
}

System::Void  NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetItem(index, (value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::get_Item(System::Int32 index)
{
	return get_RealTypeItem(index);
}

System::Void  NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection::set_Item(System::Int32 index, NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* value)
{
	set_RealTypeItem(index, value);
}