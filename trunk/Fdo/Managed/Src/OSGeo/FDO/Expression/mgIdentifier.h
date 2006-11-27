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

#include "FDO\Expression\mgExpression.h"

class FdoIdentifier;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The FdoIdentifier class derives from FdoExpression and represents an 
/// identifier, such as a class name or property name. Identifiers can be just a name 
/// or they can be scoped to a particular context. For example, consider a Person class 
/// that has a DataProperty called "Name" and a ClassProperty called "Address". 
/// The Address class has DataProperties "Street", "City", and "Zip Code". When manipulating 
/// instances of a Person, the "Name" property can be referred to by the property name "Name". 
/// The "City" property is scoped inside the "Address" property, hence must be referred to by the 
/// property name "Address.City".
public __gc class Identifier : public NAMESPACE_OSGEO_FDO_EXPRESSION::Expression
{
public:
    /// \brief
    /// Constructs a default instance of an identifier.
    /// 
    /// \return
    /// Returns the FdoIdentifier
    /// 
	Identifier();

    /// \brief
    /// Constructs an instance of an identifier using the specified arguments.
    /// 
    /// \param text 
    /// Input identifier text
    /// 
    /// \return
    /// Returns the FdoIdentifier
    /// 
	Identifier(System::String* text);

    /// \brief
    /// Gets the full text of the identifier.
    /// 
    /// \return
    /// Returns the identifier text
    /// 
	__property System::String* get_Text();

    /// \brief
    /// Sets the full text of the identifier.
    /// 
    /// \param value 
    /// Input identifier text
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Text(System::String* value);

    /// \brief
    /// Gets just the name of the identifier, with any scope stripped off.
    /// 
    /// \return
    /// Returns the name of the identifier
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Gets the scope of the identifier as an array of strings.
    /// 
    /// \param length 
    /// Output number of strings
    /// 
    /// \return
    /// Returns scope as array of strings
    /// 
	__property System::String* get_Scope() [];

    /// \brief
    /// Gets the schema name part of the identifier.
    /// 
    /// \return
    /// Returns the schema name or an empty string if the schema is not part of the identifier
    /// 
	__property System::String* get_SchemaName();

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoIdentifier to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input expression processor interface
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns the well defined text string
    /// 
	System::String* ToString();

	Identifier(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoIdentifier* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


