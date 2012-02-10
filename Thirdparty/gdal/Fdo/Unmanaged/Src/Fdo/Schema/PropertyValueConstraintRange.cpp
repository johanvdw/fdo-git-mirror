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
//

#include <Fdo/Schema/PropertyValueConstraintRange.h>
#include <Fdo/Expression/DateTimeValue.h>
#include "XmlContext.h"
#include "StringUtility.h"
#include "../Expression/ExpressionInternal.h"


// Constructs a default instance of a FdoPropertyValueConstraintRange.
FdoPropertyValueConstraintRange::FdoPropertyValueConstraintRange():
m_minValue( NULL ),
m_maxValue( NULL ),
m_isMinInclusive( false ),
m_isMaxInclusive( false )
{
}

// Constructs an instance of a FdoPropertyValueConstraintRange using the specified
// arguments.
FdoPropertyValueConstraintRange::FdoPropertyValueConstraintRange(FdoDataValue  *minValue, FdoDataValue* maxValue):
m_minValue( minValue ),
m_maxValue( maxValue ),
m_isMinInclusive( false ),
m_isMaxInclusive( false )
{
	FDO_SAFE_ADDREF( m_minValue );
	FDO_SAFE_ADDREF( m_maxValue );
}

FdoPropertyValueConstraintRange::~FdoPropertyValueConstraintRange()
{
	FDO_SAFE_RELEASE( m_minValue );
	FDO_SAFE_RELEASE( m_maxValue );
}


///<summary>Constructs an empty instance of an FdoPropertyValueConstraintRange.</summary>
/// <returns>Returns an FdoPropertyValueConstraintRange</returns> 
FdoPropertyValueConstraintRange* FdoPropertyValueConstraintRange::Create( )
{
	return new FdoPropertyValueConstraintRange( );
}

///<summary>Constructs and populates an instance of an FdoPropertyValueConstraintRange.</summary>
/// <param name="minValue">Minimum allowed value</param> 
/// <param name="maxValue">Maximum allowed value</param> 
/// <returns>Returns an FdoPropertyValueConstraintRange</returns> 
FdoPropertyValueConstraintRange* FdoPropertyValueConstraintRange::Create( FdoDataValue  *minValue, FdoDataValue* maxValue )
{
	return new FdoPropertyValueConstraintRange( minValue, maxValue );
}

///<summary>Returns FdoPropertyValueConstraintType_Range type.</summary>
///<returns>Returns the constraint type</returns>
FdoPropertyValueConstraintType FdoPropertyValueConstraintRange::GetConstraintType()
{
	return FdoPropertyValueConstraintType_Range;
}

///<summary>Get the minimum allowed value. The type of this is the same as the type of the property. 
///E.g. if the property is int32, then this value is int32.</summary>
/// <returns>Returns the minimum value</returns> 
FdoDataValue* FdoPropertyValueConstraintRange::GetMinValue()
{
	FDO_SAFE_ADDREF( m_minValue );
	return m_minValue;
}

///<summary>Set the minimum allowed value.</summary>
/// <param name="value">Minimum allowed value</param> 
/// <returns>Returns nothing</returns> 
void FdoPropertyValueConstraintRange::SetMinValue(FdoDataValue* value)
{
	FDO_SAFE_RELEASE( m_minValue );
	m_minValue = value;
	FDO_SAFE_ADDREF( m_minValue );
}

///<summary>Returns a bool to indicate if the minimum value is inclusive or exclusive. This is the difference between >= and >. 
///This is a boolean type where true means inclusive.</summary>
/// <returns>Returns true if the value is inclusive. false otherwise</returns> 
bool FdoPropertyValueConstraintRange::GetMinInclusive()
{
	return m_isMinInclusive;
}

///<summary>Set the minimum value to inclusive or exclusive. This is the difference between >= and >. 
/// <param name="value">This is a boolean type where true means inclusive.</summary>
/// <returns>Returns nothing</returns> 
void FdoPropertyValueConstraintRange::SetMinInclusive(bool value)
{
	m_isMinInclusive = value;
}

///<summary>Get the maximum allowed value. The type of this is the same as the type of the property. 
///E.g. if the property is int32, then this value is int32.</summary>
/// <returns>Returns the maximum value</returns> 
FdoDataValue* FdoPropertyValueConstraintRange::GetMaxValue()
{
	FDO_SAFE_ADDREF( m_maxValue );
	return m_maxValue;
}

///<summary>Set the maximum allowed value.</summary>
/// <param name="value">Maximum allowed value</param> 
/// <returns>Returns nothing</returns> 
void FdoPropertyValueConstraintRange::SetMaxValue(FdoDataValue* value)
{
	FDO_SAFE_RELEASE( m_maxValue );
	m_maxValue = value;
	FDO_SAFE_ADDREF( m_maxValue );
}

///<summary>Returns a bool to indicate if the maximum value is inclusive or exclusive. This is the difference between <= and <. 
///This is a boolean type where true means inclusive.</summary>
/// <returns>Returns true if the value is inclusive. false otherwise</returns> 
bool FdoPropertyValueConstraintRange::GetMaxInclusive()
{
	return m_isMaxInclusive;
}

///<summary>Returns a bool to indicate if the maximum value is inclusive or exclusive. This is the difference between <= and <. 
///This is a boolean type where true means inclusive.</summary>
/// <returns>Returns true if the value is inclusive. false otherwise</returns>
void FdoPropertyValueConstraintRange::SetMaxInclusive(bool value)
{
	m_isMaxInclusive = value;
}

void FdoPropertyValueConstraintRange::Set( FdoPropertyValueConstraint* pConstraint, FdoString* parentName, FdoSchemaMergeContext* pContext )
{
    FdoPropertyValueConstraint::Set(pConstraint, parentName, pContext);

    // Base function adds error on constraint type mismatch so silently skip if 
    // types do not match.
    if ( pConstraint->GetConstraintType() == FdoPropertyValueConstraintType_Range ) {
        FdoPropertyValueConstraintRange* pRangeConstraint = (FdoPropertyValueConstraintRange*) pConstraint;

        // For the purposes of XML reading and SDF Provider ApplySchema, reusing the min value
        // is fine, but is not ok in the general case. 
        // TODO: make copy of min value
        SetMinValue( FdoPtr<FdoDataValue>(pRangeConstraint->GetMinValue()) );
        SetMinInclusive( pRangeConstraint->GetMinInclusive() );

        // TODO: make copy of max value
        SetMaxValue( FdoPtr<FdoDataValue>(pRangeConstraint->GetMaxValue()) );
        SetMaxInclusive( pRangeConstraint->GetMaxInclusive() );
    }
}

bool FdoPropertyValueConstraintRange::Equals( FdoPropertyValueConstraint* pConstraint )
{
    // Not equal if other constraint is not a range.
    if ( pConstraint->GetConstraintType() != FdoPropertyValueConstraintType_Range )
        return false;

    FdoPropertyValueConstraintRange* pRangeConstraint = (FdoPropertyValueConstraintRange*) pConstraint;

    // First, do easy part: compare min and max inclusive settings

    if ( GetMinInclusive() != pRangeConstraint->GetMinInclusive() )
        return false;

    if ( GetMaxInclusive() != pRangeConstraint->GetMaxInclusive() )
        return false;

    FdoPtr<FdoDataValue> myMinValue = GetMinValue();
    FdoPtr<FdoDataValue> theirMinValue = pRangeConstraint->GetMinValue();

    if ( FdoInternalDataValue::Compare(myMinValue, theirMinValue) != FdoCompareType_Equal ) 
        return false;

    FdoPtr<FdoDataValue> myMaxValue = GetMaxValue();
    FdoPtr<FdoDataValue> theirMaxValue = pRangeConstraint->GetMaxValue();

    if ( FdoInternalDataValue::Compare(myMaxValue, theirMaxValue) != FdoCompareType_Equal ) 
        return false;

    return true;
}

bool FdoPropertyValueConstraintRange::Contains( FdoPropertyValueConstraint* pConstraint )
{
    // Not equal if other constraint is not a range.
    if ( pConstraint->GetConstraintType() != FdoPropertyValueConstraintType_Range )
        return false;

    FdoPropertyValueConstraintRange* pRangeConstraint = (FdoPropertyValueConstraintRange*) pConstraint;

    FdoCompareType compare = CompareRangeEnd( 
        GetMinInclusive(), 
        FdoPtr<FdoDataValue>(GetMinValue()),
        pRangeConstraint->GetMinInclusive(), 
        FdoPtr<FdoDataValue>(pRangeConstraint->GetMinValue()),
        false
    );
    
    // Doesn't contain other range if this min > other min or mins can't be compared
    if ( (compare == FdoCompareType_Greater) || (compare == FdoCompareType_Undefined) ) 
        return false;

    compare = CompareRangeEnd( 
        GetMaxInclusive(), 
        FdoPtr<FdoDataValue>(GetMaxValue()),
        pRangeConstraint->GetMaxInclusive(), 
        FdoPtr<FdoDataValue>(pRangeConstraint->GetMaxValue()),
        true
    );
    
    // Doesn't contain other range if this max < other max or max's can't be compared
    if ( (compare == FdoCompareType_Less) || (compare == FdoCompareType_Undefined) ) 
        return false;

    return true;
}

bool FdoPropertyValueConstraintRange::Contains( FdoDataValue* pValue )
{
    // Null value always satisfies constraint.
    if ( (pValue == NULL) || pValue->IsNull() ) 
        return true;

    FDO_SAFE_ADDREF(pValue);
    FdoPtr<FdoDataValue> value = pValue;

    // Check the value against the minimum end of the range.
    FdoCompareType compare = CompareEnd( 
        GetMinInclusive(), 
        FdoPtr<FdoDataValue>(GetMinValue()),
        true, 
        value,
        false
    );
    
    if ( (compare == FdoCompareType_Greater) || (compare == FdoCompareType_Undefined) ) 
        // value is below the minimum (or can't be compared with minimum) so it violates the constraint. 
        return false;

    // Check the value against the maximum end of the range.
    compare = CompareEnd( 
        GetMaxInclusive(), 
        FdoPtr<FdoDataValue>(GetMaxValue()),
        true, 
        value,
        true
    );
    
    if ( (compare == FdoCompareType_Less) || (compare == FdoCompareType_Undefined) ) 
        // value is above the maximum (or can't be compared with maximum) so it violates the constraint. 
        return false;

    return true;
}

bool FdoPropertyValueConstraintRange::ValueEquals( FdoPtr<FdoDataValue> myValue, FdoPtr<FdoDataValue> theirValue )
{

    FdoStringP myValueString = ValueToString( myValue );
    FdoStringP theirValueString = ValueToString( theirValue );

    return ( myValueString == theirValueString );
}

FdoStringP FdoPropertyValueConstraintRange::ValueToString( FdoPtr<FdoDataValue> value )
{
    FdoStringP stringContainer;

    if ( value ) {
        FdoString* stringValue = value->ToString();
        stringContainer = stringValue;
    }

    return stringContainer;
}

FdoCompareType FdoPropertyValueConstraintRange::CompareRangeEnd( FdoBoolean myInclusive, FdoPtr<FdoDataValue> myValue, FdoBoolean theirInclusive, FdoPtr<FdoDataValue> theirValue, FdoBoolean isMax )
{
    FdoCompareType result = CompareEnd( myInclusive, myValue, theirInclusive, theirValue, isMax );

    // If both values are not null but have different components then Contains() must return
    // false (by having this function return Undefined). This forces a check of the data when an SDF range constraint is changed.
    // The above must also be done if the date component increased but the time component 
    // decreased or vice versa. 
    //
    // The reason for the above is that in these cases there can be existing data values that
    // are ok for the old constraint but not the new one.

    if ( myValue && !(myValue->IsNull()) && (myValue->GetDataType() == FdoDataType_DateTime) &&
         theirValue && !(theirValue->IsNull()) && (theirValue->GetDataType() == FdoDataType_DateTime)
    ) {
        FdoDateTimeValue* myDtValue = (FdoDateTimeValue*) myValue.p;
        FdoDateTimeValue* theirDtValue = (FdoDateTimeValue*) theirValue.p;
        FdoDateTime myDt = myDtValue->GetDateTime();
        FdoDateTime theirDt = theirDtValue->GetDateTime();

        if ( (myDt.IsDateTime() == theirDt.IsDateTime()) &&
             (myDt.IsDate() == theirDt.IsDate()) &&
             (myDt.IsTime() == theirDt.IsTime())
        ) {
            // Values define the same components

            if ( myDt.IsDateTime() ) {
                // Both are date time. Check if date increased but time decreased or vice versa.
                FdoDateTime myTime( myDt.hour, myDt.minute, myDt.seconds );
                FdoPtr<FdoDateTimeValue> myTimeValue = FdoDateTimeValue::Create(myTime);
                FdoDateTime theirTime( theirDt.hour, theirDt.minute, theirDt.seconds );
                FdoPtr<FdoDateTimeValue> theirTimeValue = FdoDateTimeValue::Create(theirTime);

                FdoCompareType timeCompare = FdoInternalDataValue::Compare( myTimeValue, theirTimeValue );

                if ( (timeCompare == FdoCompareType_Equal) && (myInclusive != theirInclusive) ) {
                    if ( theirInclusive ) 
                        timeCompare = isMax ? FdoCompareType_Less : FdoCompareType_Greater;

                    if ( myInclusive ) 
                        timeCompare = isMax ? FdoCompareType_Greater : FdoCompareType_Less;
                }

                if ( timeCompare != FdoCompareType_Equal ) {
                    if ( timeCompare != result ) 
                        result = FdoCompareType_Undefined;
                }
            }
        }
        else {
            // Defined components differ.
            result = FdoCompareType_Undefined;
        }

    }

    return result;
}

FdoCompareType FdoPropertyValueConstraintRange::CompareEnd( FdoBoolean myInclusive, FdoPtr<FdoDataValue> myValue, FdoBoolean theirInclusive, FdoPtr<FdoDataValue> theirValue, FdoBoolean isMax )
{
    // Equal if both values are null.
    if ( (!myValue || myValue->IsNull()) && (!theirValue || theirValue->IsNull()) ) 
        return FdoCompareType_Equal;

    if ( !theirValue || theirValue->IsNull() ) 
        // Other value null, comparison depends on whether min or max
        return isMax ? FdoCompareType_Less : FdoCompareType_Greater;

    if ( !myValue || myValue->IsNull() ) 
        // This value null, comparison depends on whether min or max
        return isMax ? FdoCompareType_Greater : FdoCompareType_Less;

    FdoCompareType result = FdoInternalDataValue::Compare( myValue, theirValue );

    if ( result == FdoCompareType_PartlyEqual ) 
        result = FdoCompareType_Equal;

    // Values are equal. However, if inclusivities not the same then they are 
    // not equal
    if ( (result == FdoCompareType_Equal) && (myInclusive != theirInclusive) ) {
        if ( theirInclusive ) 
            result = isMax ? FdoCompareType_Less : FdoCompareType_Greater;

        if ( myInclusive ) 
            result = isMax ? FdoCompareType_Greater : FdoCompareType_Less;
    }

    return result;
}

