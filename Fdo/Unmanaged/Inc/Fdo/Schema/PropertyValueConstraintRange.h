#ifndef _PROPERTYVALUECONSTRAINTRANGE_H_
#define _PROPERTYVALUECONSTRAINTRANGE_H_

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
#include <Fdo/Schema/PropertyValueConstraint.h>
#include <Fdo/Expression/DataValue.h>

/// \brief
/// FdoPropertyValueConstraintRange is used to specify minimum and / or maximum allowed values for a particular property. 
/// It can be used for all data property types except for Boolean, BLOB, or CLOB.
/// One or the other or both of MinValue and MaxValue must be specified. If both are specified, then MaxValue must be greater 
/// than or equal to MinValue and if either MinInclusive or MaxInclusive are false, then MaxValue must be greater than MinValue. 
/// MinValue and MaxValue if specified must be valid values for the property type. E.g. if the property is decimal(4,0), 
/// then the maximum possible MaxValue is 9999.
/// If the data property definition includes a non-null default value, then this constraint is applied to that value as well.
/// If the data property definition allows nulls, a null value is considered as being valid regardless of the range constraint.
class FdoPropertyValueConstraintRange : public FdoPropertyValueConstraint
{
protected:
    /// Constructs a default instance of a FdoPropertyValueConstraintRange.
    FdoPropertyValueConstraintRange();

    /// Constructs an instance of a FdoPropertyValueConstraintRange using the specified
    /// arguments.
    FdoPropertyValueConstraintRange(FdoDataValue  *minValue, FdoDataValue* maxValue);

    virtual ~FdoPropertyValueConstraintRange();

	virtual void Dispose()
    {
        delete this;
    }
public:
    
    /// \brief
    /// Constructs an empty instance of an FdoPropertyValueConstraintRange.
    /// 
    /// \return
    /// Returns an FdoPropertyValueConstraintRange
    /// 
    FDO_API static FdoPropertyValueConstraintRange* Create( );

    /// \brief
    /// Constructs and populates an instance of an FdoPropertyValueConstraintRange.
    /// 
    /// \param minValue 
    /// Minimum allowed value
    /// \param maxValue 
    /// Maximum allowed value
    /// 
    /// \return
    /// Returns an FdoPropertyValueConstraintRange
    /// 
    FDO_API static FdoPropertyValueConstraintRange* Create( FdoDataValue  *minValue, FdoDataValue* maxValue );

    /// \brief
    /// Returns FdoPropertyValueConstraintType_Range type.
    /// 
    /// \return
    /// Returns the constraint type
    /// 
    FDO_API virtual FdoPropertyValueConstraintType GetConstraintType();

    /// \brief
    /// Get the minimum allowed value. The type of this is the same as the type of the property. 
    /// E.g. if the property is int32, then this value is int32.
    /// 
    /// \return
    /// Returns the minimum value
    /// 
	FDO_API FdoDataValue* GetMinValue();

    /// \brief
    /// Set the minimum allowed value.
    /// 
    /// \param value 
    /// Minimum allowed value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetMinValue(FdoDataValue* value);

    /// \brief
    /// Returns a bool to indicate if the minimum value is inclusive or exclusive. This is the difference between >= and >. 
    /// This is a boolean type where true means inclusive.
    /// 
    /// \return
    /// Returns true if the value is inclusive. false otherwise
    /// 
    FDO_API bool GetMinInclusive();

    /// \brief
    /// Set the minimum value to inclusive or exclusive. This is the difference between >= and >. 
    /// 
    /// \param value 
    /// This is a boolean type where true means inclusive.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetMinInclusive(bool value);

    /// \brief
    /// Get the maximum allowed value. The type of this is the same as the type of the property. 
    /// E.g. if the property is int32, then this value is int32.
    /// 
    /// \return
    /// Returns the maximum value
    /// 
    FDO_API FdoDataValue* GetMaxValue();

    /// \brief
    /// Set the maximum allowed value.
    /// 
    /// \param value 
    /// Maximum allowed value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetMaxValue(FdoDataValue* value);

    /// \brief
    /// Returns a bool to indicate if the maximum value is inclusive or exclusive. This is the difference between <= and <. 
    /// This is a boolean type where true means inclusive.
    /// 
    /// \return
    /// Returns true if the value is inclusive. false otherwise
    /// 
    FDO_API bool GetMaxInclusive();

    /// \brief
    /// Returns a bool to indicate if the maximum value is inclusive or exclusive. This is the difference between <= and <. 
    /// This is a boolean type where true means inclusive.
    /// 
    /// \return
    /// Returns true if the value is inclusive. false otherwise
    /// 
    FDO_API void SetMaxInclusive(bool value);

private:
	FdoDataValue*	m_minValue;
	FdoDataValue*	m_maxValue;
	bool			m_isMinInclusive;
	bool			m_isMaxInclusive;
};

#endif


