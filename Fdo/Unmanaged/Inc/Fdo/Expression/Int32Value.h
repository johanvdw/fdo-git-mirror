#ifndef _INT32VALUE_H_
#define _INT32VALUE_H_
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
/// The FdoInt32Value class derives from FdoDataValue and represents a 32-bit signed integer value.
class FdoInt32Value : public FdoDataValue
{
/// \cond DOXYGEN-IGNORE
    friend class FdoByteValue;
    friend class FdoInt16Value;
    friend class FdoStringValue;
    friend class FdoDataValue;
protected:
    /// \brief
    /// Constructs a default instance of an FdoInt32Value with a
    /// value of null.
    /// \return
    /// Returns nothing
    /// 
    FdoInt32Value();

    /// \brief
    /// Constructs a default instance of an FdoInt32Value using the specified arguments.
    /// \param value 
    /// Input a 32 bit integer
    /// 
    /// \return
    /// Returns nothing
    /// 
    FdoInt32Value(FdoInt32 value);

    /// \brief
    /// Default destructor for FdoInt32Value.
    /// \return
    /// Returns nothing
    /// 
    virtual ~FdoInt32Value();

    virtual void Dispose();
/// \endcond

public:
    /// \brief
    /// Constructs a default instance of an FdoInt32Value with a value of null.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API static FdoInt32Value* Create();

    /// \brief
    /// Constructs a default instance of an FdoInt32Value using the specified arguments.
    /// 
    /// \param value 
    /// Input a 32 bit integer
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API static FdoInt32Value* Create(FdoInt32 value);

    /// \brief
    /// Constructs an instance of an FdoInt32Value from another FdoDataValue.
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
    /// Input determines what to do if source value is outside the FdoInt32 range
    ///  ( INT_MIN to INT_MAX ):
    ///     true - convert values less than INT_MIN to INT_MIN, convert values greater than INT_MAX to INT_MAX
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \return
    /// Returns an FdoInt32Value, whose value is converted from the src value. 
    /// If src is an FdoBooleanValue:
    ///     false is converted to 0
    ///     true is converted to 1
    ///
    FDO_API static FdoInt32Value* Create(
        FdoDataValue* src, 
        FdoBoolean nullIfIncompatible = false,
        FdoBoolean shift = true, 
        FdoBoolean truncate = false 
    );

    /// \brief
    /// Gets the data type of the FdoInt32Value.
    /// 
    /// \return
    /// Returns an FdoDataType
    /// 
    FDO_API FdoDataType GetDataType();

    /// \brief
    /// Gets the FdoInt32Value.
    /// 
    /// \return
    /// Returns a 32 bit integer
    /// 
    FDO_API FdoInt32 GetInt32();

    /// \brief
    /// Sets the FdoInt32Value.
    /// 
    /// \param value 
    /// Input a 32 bit integer
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetInt32(FdoInt32 value);

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoInt32Value to the appropriate
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
    /// Returns a character string
    /// 
    FDO_API FdoString* ToString();

    /// \brief
    /// A cast operator to get the integer value.
    /// 
    /// \return
    /// Returns a 32 bit integer
    /// 
    FDO_API operator FdoInt32()
    {
        return m_data;
    }

/// \cond DOXYGEN-IGNORE
protected:
    // See FdoDataValue::DoCompare()
    virtual FdoCompareType DoCompare( FdoDataValue* other );

    FdoInt32    m_data;
/// \endcond
};
#endif



