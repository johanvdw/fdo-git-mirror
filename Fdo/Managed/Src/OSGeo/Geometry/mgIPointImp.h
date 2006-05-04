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

#pragma once
#include "mgIGeometryImp.h"
#include "mgIPoint.h"

class FdoIPoint;
class FdoPointCollection;
BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc __interface IDirectPosition;
END_NAMESPACE_OSGEO_GEOMETRY

BEGIN_NAMESPACE_OSGEO_GEOMETRY

/// \brief
/// The IPointImp class is a point Geometry type.
private __gc class IPointImp : 
	public NAMESPACE_OSGEO_GEOMETRY::IGeometryImp, public NAMESPACE_OSGEO_GEOMETRY::IPoint
{
public:
	IPointImp(IntPtr unmanaged, Boolean autoDelete);

public private:
	FdoIPoint *GetImpObj();

public:
    /// \brief
    /// Gets the position of this point Geometry.
    /// 
    /// \return
    /// Returns the position
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IDirectPosition *get_Position();
	
    /// \brief
    /// Gets the position of this point Geometry, by values of its member data.
    /// This is in aid of higher performance for any implementation that
    /// does not internally use FdoIDirectPosition objects for storage, or for
    /// an application seeking to avoid overhead of accessor methods.
    /// 
    /// \param x 
    /// Output X ordinate value.
    /// \param y 
    /// Output Y ordinate value.
    /// \param z 
    /// Output Z ordinate value.
    /// \param m 
    /// Output M ordinate value.
    /// \param dimensionality 
    /// Output Dimensionality of ordinates in this position.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void GetPositionByMembers(
		System::Double &coordinateX,
		System::Double &coordinateY,
		System::Double &coordinateZ,
		System::Double &coordinateM,
		System::Int32 &dimensionality);
};

/// \brief
///  The PointCollection class is a collection of Point objects.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __gc __sealed class PointCollection 
	: public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList
{
public:
	PointCollection(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	FdoPointCollection *GetImpObj();

protected:
	__sealed System::Void Dispose(System::Boolean disposing);

private:

	__gc class Enumerator : public System::Collections::IEnumerator
	{
		PointCollection *m_pCol;
		System::Int32 m_nIdx;
	public:
		Enumerator(PointCollection *col)
			: m_pCol(col), m_nIdx(-1)
		{}

		__property System::Object *get_Current();
		System::Boolean MoveNext();
		System::Void Reset();
	};
public:
    /// \brief
    ///  Creates an instance of PointCollection with no contained elements.
    /// 
    /// \return
    /// Returns an empty collection
    /// 
	PointCollection();

private:
    // System::Collections::ICollection
	System::Void System::Collections::ICollection::CopyTo(System::Array* array,System::Int32 index);
	__property System::Object* System::Collections::ICollection::get_SyncRoot(System::Void);
	__property System::Boolean System::Collections::ICollection::get_IsSynchronized(System::Void);
    // System::Collections::IList
	__property System::Boolean System::Collections::IList::get_IsFixedSize(System::Void);
	__property System::Boolean System::Collections::IList::get_IsReadOnly(System::Void);
	System::Int32 System::Collections::IList::Add(Object* value);
	System::Boolean System::Collections::IList::Contains(Object* value);
	System::Int32 System::Collections::IList::IndexOf(Object* value);
	System::Void System::Collections::IList::Insert(System::Int32 index, Object* value);
	System::Void System::Collections::IList::Remove(Object* value);
	__property Object* System::Collections::IList::get_Item(System::Int32 index);
	__property System::Void  System::Collections::IList::set_Item(System::Int32 index, Object* value);

public:
    /// \brief
    ///    Gets the count of items in collection.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    ///     Removes the index-th IPoint from this collection.
    /// 
    /// \param index 
    /// Input index of the element to remove.
    /// 
	System::Void RemoveAt(System::Int32 index);
    /// \brief
    ///     Removes all elements from the collection.
    /// 
	System::Void  Clear();

public:
    /// \brief
    ///     Adds a IPoint object into the collection.
    /// 
    /// \param value 
    /// Input the IPoint object to add.
    /// 
    /// \return
    /// 		The position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_GEOMETRY::IPoint* value);
    /// \brief
    ///     Determines the index of a specific IPoint object.
    /// 
    /// \param value 
    /// Input the IPoint object to locate in the collection.
    /// 
    /// \return
    /// 		The index of value if found in the collection; otherwise, -1.
    /// 
    System::Int32 IndexOf(NAMESPACE_OSGEO_GEOMETRY::IPoint* value);
    /// \brief
    ///     Inserts an IPoint object to the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the IPoint object to insert.
    /// 
    System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint* value); 
    /// \brief
    ///     Removes the first occurrence of a specific IPoint object.
    /// 
    /// \param value 
    /// Input the IPoint object to remove from the collection.
    /// 
    System::Void Remove(NAMESPACE_OSGEO_GEOMETRY::IPoint* value);
    /// \brief
    ///     Determines whether the collection contains a specific IPoint object.
    /// 
    /// \param value 
    /// Input The IPoint object to locate in the collection.
    /// 
    /// \return
    /// 		True if the value is found in the collection; otherwise, false.
    /// 
    System::Boolean Contains(NAMESPACE_OSGEO_GEOMETRY::IPoint* value);
    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
    System::Void CopyTo(NAMESPACE_OSGEO_GEOMETRY::IPoint* array[],System::Int32 startAt);

    /// \brief
    ///     Gets or sets an IPoint in the collection.
    /// 
    /// \param index 
    /// Input index of the IPoint to retrieve or set (System::Int32).
    /// 
	__property NAMESPACE_OSGEO_GEOMETRY::IPoint *get_RealTypeItem(System::Int32 index);
    __property System::Void set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint *value);

	__property NAMESPACE_OSGEO_GEOMETRY::IPoint *get_Item(System::Int32 index);
    __property System::Void set_Item(System::Int32 index, NAMESPACE_OSGEO_GEOMETRY::IPoint *value);
};

END_NAMESPACE_OSGEO_GEOMETRY


