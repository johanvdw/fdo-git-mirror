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

#include "FDO\Filter\mgBinaryLogicalOperations.h"

class FdoFilter;

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
public __gc __interface IFilterProcessor;;

/// \brief
/// The FdoFilter abstract class is a base class for condition and operator classes
/// that can occur in a filter expression tree. The FdoFilter class contains
/// operations for converting between the text and expression tree representation
/// of a filter.
public __gc class Filter : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Static operation that parses the well defined text representation and
    /// returns a Filter instance.
    /// 
    /// \param filterText 
    /// Input well defined text string
    /// 
    /// \return
    /// Returns root node of parse tree
    /// 
	 static NAMESPACE_OSGEO_FDO_FILTER::Filter* Parse(System::String* filterText);

    /// \brief
    /// Abstract operation that takes an FdoIFilterProcessor as an argument.
    /// Concrete filter subclasses must override this method and pass themselves
    /// as an argument to the appropriate filter processor operation.
    /// 
    /// \param processor 
    /// Input filter processor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor* processor);

    /// \brief
    /// Converts the filter expression to its well
    /// defined text representation.
    /// 
    /// \return
    /// Returns well defined text string
    /// 
	System::String* ToString();
	
    /// \brief
    /// Static operation that combines two filters using the specified binary
    /// logical operation.
    /// 
    /// \param lhs 
    /// Input left hand expression as filter expression
    /// \param operation 
    /// Input binary logical operation
    /// \param rhs 
    /// Input right hand expression as filter expression
    /// 
    /// \return
    /// Returns new filter node
    /// 
	NAMESPACE_OSGEO_FDO_FILTER::Filter* Combine( NAMESPACE_OSGEO_FDO_FILTER::Filter* leftFilter, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, NAMESPACE_OSGEO_FDO_FILTER::Filter* rightFilter);

    /// \brief
    /// Static operation that combines two filters using the specified binary
    /// logical operation.
    /// 
    /// \param leftFilter 
    /// Input left hand expression as well defined text string
    /// \param operation 
    /// Input binary logical operation
    /// \param rightFilter 
    /// Input right hand expression as filter expression
    /// 
    /// \return
    /// Returns new filter node
    /// 
	NAMESPACE_OSGEO_FDO_FILTER::Filter* Combine(System::String* leftFilter, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, NAMESPACE_OSGEO_FDO_FILTER::Filter* rightFilter);

    /// \brief
    /// Static operation that combines two filters using the specified binary
    /// logical operation.
    /// 
    /// \param lhs 
    /// Input left hand expression as filter expression
    /// \param operation 
    /// Input binary logical operation
    /// \param rhs 
    /// Input right hand expression as well defined text string
    /// 
    /// \return
    /// Returns new filter node
    /// 
	NAMESPACE_OSGEO_FDO_FILTER::Filter* Combine(NAMESPACE_OSGEO_FDO_FILTER::Filter* leftFilter, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, System::String* rightFilter);

    /// \brief
    /// Static operation that combines two filters using the specified binary
    /// logical operation.
    /// 
    /// \param lhs 
    /// Input left hand expression as well defined text string
    /// \param operation 
    /// Input binary logical operation
    /// \param rhs 
    /// Input right hand expression as well defined text string
    /// 
    /// \return
    /// Returns new filter node
    /// 
	NAMESPACE_OSGEO_FDO_FILTER::Filter* Combine(System::String* leftFilter, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperations operation, System::String* rightFilter);

	Filter(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoFilter* GetImpObj();

protected:
	System::Void ReleaseUnmanagedObject();
};

END_NAMESPACE_OSGEO_FDO_FILTER


