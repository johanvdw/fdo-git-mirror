#ifndef _BYTEVALUE_H_
#define _BYTEVALUE_H_
// 

//
// Copyright (C) 2004-2006  Autodesk, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <Fdo/Expression/DataValue.h>
#include <Fdo/Schema/DataType.h>

/// \brief
/// The FdoByteValue class derives from FdoDataValue and represents a literal
/// byte value.
class FdoByteValue : public FdoDataValue
{
    friend class FdoStringValue;
    friend class FdoDataValue;
/// \cond DOXYGEN-IGNORE
protected:
    /// \brief
    /// Constructs a default instance of an FdoByteValue with a value of null.
    /// \return
    /// Returns nothing
    /// 
    FdoByteValue();

    /// \brief
    /// Constructs a default instance of an FdoByteValue using the specified arguments.
    /// \param value 
    /// Input a byte
    /// 
    /// \return
    /// Returns nothing
    /// 
    FdoByteValue(FdoByte value);

    /// \brief
    /// Default destructor for FdoByteValue.
    virtual ~FdoByteValue();

    virtual void Dispose();
/// \endcond

public:
    /// \brief
    /// Constructs a default instance of an FdoByteValue with a value of null.
    /// 
    /// \return
    /// Returns the created FdoByteValue
    /// 
    FDO_API static FdoByteValue* Create();

    /// \brief
    /// Constructs a default instance of an FdoByteValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte
    /// 
    /// \return
    /// Returns the created FdoByteValue
    /// 
    FDO_API static FdoByteValue* Create(FdoByte value);

    /// \brief
    /// Constructs an instance of an FdoByteValue from another FdoDataValue.
    /// 
    /// \param src 
    /// Input the other FdoDataValue. Must be of one of the following types:
    ///     FdoDataType_Boolean
    ///     FdoDataType_Byte
    ///     FdoDataType_Decimal
    ///     FdoDataType_Double
    ///     FdoDataType_Int16
    ///     FdoDataType_Int32
    ///     FdoDataType_Int64
    ///     FdoDataType_Single
    ///     FdoDataType_String
    ///         - value must be numeric.
    ///
    /// In all other cases, the src type is considered incompatible with this type.
    /// \param nullIfIncompatible 
    /// Input will determine what to do if the source value cannot be converted to 
    /// this type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \param shift 
    /// Input determines whether non integer values can be converted:
    ///     true - convert values by rounding them.
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \param truncate 
    /// Input determines what to do if source value is outside the FdoByte range
    //  ( 0 to 255 ):
    ///     true - convert values less than 0 to 0, convert values greater than 255 to 255
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \return
    /// Returns an FdoByteValue, whose value is converted from the src value. 
    /// If src is an FdoBooleanValue:
    ///     false is converted to 0
    ///     true is converted to 1
    ///
    FDO_API static FdoByteValue* Create(
        FdoDataValue* src, 
        FdoBoolean nullIfIncompatible = false,
        FdoBoolean shift = true, 
        FdoBoolean truncate = false 
    );

    /// \brief
    /// Gets the data type of the FdoByteValue.
    /// 
    /// \return
    /// Returns an FdoDataType
    /// 
    FDO_API FdoDataType GetDataType();

    /// \brief
    /// Gets the FdoByteValue.
    /// 
    /// \return
    /// Returns a byte
    /// 
    FDO_API FdoByte GetByte();

    /// \brief
    /// Sets the byte value.
    /// 
    /// \param value 
    /// Input a byte
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetByte(FdoByte value);

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoByteValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param p 
    /// Input an FdoIExpressionProcessor
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void Process(FdoIExpressionProcessor* p);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a text string
    /// 
    FDO_API FdoString* ToString();

    /// \brief
    /// A cast operator to get the byte value.
    /// 
    /// \return
    /// Returns a byte
    /// 
    FDO_API operator FdoByte()
    {
        return m_data;
    }

/// \cond DOXYGEN-IGNORE
protected:
    // See FdoDataValue::DoCompare()
    virtual FdoCompareType DoCompare( FdoDataValue* other );

    FdoByte m_data;
/// \endcond
};
#endif



