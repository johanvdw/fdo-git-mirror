// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "ShpScrollableFeatureReader.h"
#include "ShpConnection.h"


ShpScrollableFeatureReader::ShpScrollableFeatureReader(ShpConnection* connection, FdoString* className, FdoFilter* filter, SortElementDef *sortedTable, unsigned int tableSize, PropertyStub* propStubs, int numProps  )
: ShpFeatureReader(connection, className, NULL, NULL)
{
	m_Filter = filter;
    m_SortedTable = sortedTable;  // Transfer the ownership of this array; need to be freed by this class
	m_TableSize = tableSize;
	m_PropStubs = propStubs;
	m_NumProps = numProps;

	m_CurrentIndex = -1;
 }

ShpScrollableFeatureReader::~ShpScrollableFeatureReader()
{
	// Clean up
	if ( m_SortedTable && m_SortedTable->ctx )
	{
		SortContextDef	*ctx = m_SortedTable->ctx;

		for( int i = 0; i < ctx->propCount; i++) {
			delete[] ctx->names[i];
		}
		if ( ctx->names )
			delete[] ctx->names;
		if ( ctx->options )
			delete[] ctx->options;
		if ( ctx->featIds )
			delete[] ctx->featIds;

		if ( ctx->propCount > 0 )
		{
			for( int i = 0; i < (int)m_TableSize; i++ )
			{
				for( int j = 0; j < ctx->propCount; j++ )
				{
					if( ctx->propCache[i][j].type == FdoDataType_String && ctx->propCache[i][j].value.strVal != NULL )
						delete[] ctx->propCache[i][j].value.strVal;
					else if ( ctx->propCache[i][j].type == FdoDataType_DateTime && ctx->propCache[i][j].value.dateVal != NULL )
						delete ctx->propCache[i][j].value.dateVal;
				}
				delete[] ctx->propCache[i];
			}
			delete[] ctx->propCache;
		}
		delete ctx;
	}

	if( m_SortedTable != NULL )
		delete[] m_SortedTable;

	if ( m_PropStubs )
	{
//		for ( int i = 0; i < m_NumProps; i++ )
//			delete [] m_PropStubs->m_name;

//		delete[] m_PropStubs;
	}
}

void ShpScrollableFeatureReader::Dispose()
{
    delete this;
}

void ShpScrollableFeatureReader::GetData()
{
    if ( mData )
		delete mData;

    if ( mShape )
		delete mShape;

	mData = NULL;
	mShape = NULL;

    mFileSet->GetObjectAt (&mData, mType, &mShape, mFeatureNumber);
}

bool ShpScrollableFeatureReader::ReadNext()
{
	m_CurrentIndex++;
	if( m_CurrentIndex >= (int)m_TableSize )
	{
		m_CurrentIndex = -1;
		return false;
	}

	FdoInt32 index = m_SortedTable [m_CurrentIndex].index;
	mFeatureNumber = m_SortedTable->ctx->featIds[index];

	GetData();

	return true;
}

int ShpScrollableFeatureReader::Count()
{
	return m_TableSize;
}

bool ShpScrollableFeatureReader::ReadFirst()
{
	m_CurrentIndex = 0;

	FdoInt32 index = m_SortedTable [m_CurrentIndex].index;
	mFeatureNumber = m_SortedTable->ctx->featIds[index];

	GetData();

	return true;
}

bool ShpScrollableFeatureReader::ReadLast()
{
	m_CurrentIndex = m_TableSize - 1;

	FdoInt32 index = m_SortedTable [m_CurrentIndex].index;
	mFeatureNumber = m_SortedTable->ctx->featIds[index];

	GetData();

	return true;
}

bool ShpScrollableFeatureReader::ReadPrevious()
{
	m_CurrentIndex--;
	if( m_CurrentIndex < 0 )
	{
		m_CurrentIndex = -1;
		return false;
	}

	FdoInt32 index = m_SortedTable [m_CurrentIndex].index;
	mFeatureNumber = m_SortedTable->ctx->featIds[index];

    GetData();

	return true;
}

bool ShpScrollableFeatureReader::ReadAt(FdoPropertyValueCollection* key)
{
	unsigned int recordindex = IndexOf( key );
	if( recordindex == 0 )
		return false;

	return ReadAtIndex( recordindex );
}

bool  ShpScrollableFeatureReader::ReadAtIndex( unsigned int recordindex )
{
	if( recordindex > m_TableSize || recordindex < 1 )
		return false;

	m_CurrentIndex = recordindex - 1;  // One-based

	FdoInt32 index = m_SortedTable [m_CurrentIndex].index;
	mFeatureNumber = m_SortedTable->ctx->featIds[index];

    GetData();

	return true;
}

unsigned int ShpScrollableFeatureReader::IndexOf(FdoPropertyValueCollection* keyVals)
{
	REC_NO		rowid = 0;
	bool		found = false;
	int			k;

	// Special case: no ordering property or the properties passed in do not match the
	// ordering ones (one or more)

	if ( m_NumProps == 0 )
	{
		// If the property requested is the Featid, then we are in luck.
		if ( keyVals->GetCount() == 1 && wcscmp(keyVals->GetItem(0)->GetName()->GetText(), L"FeatId" ) == 0 ) 
		{
			FdoPtr<FdoValueExpression> valueExpr = keyVals->GetItem(0)->GetValue();
			FdoInt32Value * typedValue = (FdoInt32Value *)(valueExpr.p);
			FdoInt32		featid = typedValue->GetInt32() - 1; // one-based

			rowid = FindRowidByFeatNum( featid );
		}
		else
		{
			// The sorted table cannot be used, create another one.
			rowid = SearchNewSortedTable( keyVals );
		}
	}
	else
	{
		// If the property requested is the Featid, then we are in luck. The ctx->featIds[] is already sorted.
		if ( keyVals->GetCount() == 1 && wcscmp(keyVals->GetItem(0)->GetName()->GetText(), L"FeatId" ) == 0 ) 
		{
			FdoPtr<FdoValueExpression> valueExpr = keyVals->GetItem(0)->GetValue();
			FdoInt32Value * typedValue = (FdoInt32Value *)(valueExpr.p);
			FdoInt32		featid = typedValue->GetInt32() - 1; // one-based

			rowid = FindRowidByFeatNum( featid );
		}
		else
		{
			// Check and see if ALL the keys passed in are among the ordering properties.
			for ( int i = 0; i < keyVals->GetCount(); i++ )
			{
				FdoStringP	keyName = keyVals->GetItem(i)->GetName()->GetText();
				found = false;
				for ( int j = 0; j < m_NumProps && !found; j++ )
				{
					found = ( wcscmp(keyName, m_PropStubs[j].m_name ) == 0 );
				}
				if ( !found )
					throw FdoCommandException::Create(NlsMsgGet(SHP_INVALID_INDEXOF_PARAM, 
							"Only ordering properties or the identity property are allowed as IndexOf() argument"));
			}
			rowid = SearchSortedTable( m_SortedTable, m_TableSize, m_PropStubs, m_NumProps, keyVals);
		}

		found = false;
		for ( k = 0; k < (int)m_TableSize && !found; k++ )
		{
			found = ( (rowid -1 ) == m_SortedTable[k].index);
		}
		rowid = k; // it is the current row index + 1. 	
	}

	return rowid;
}

REC_NO ShpScrollableFeatureReader::FindRowidByFeatNum( REC_NO featid )
{
	REC_NO		rowid = 0;
	bool		found;
	int			k;

	// When the featids are in order (no deletes) then rowid is matching
	if ( m_SortedTable->ctx->featIds[featid] == featid )
	{
		rowid = featid + 1; // One-based
	}
	else // Read backwards
	{
		found = false;
		for ( k = featid - 1; k >=0 && !found; k-- )
		{
			found = (m_SortedTable->ctx->featIds[k] == featid);
		}
		rowid = k + 1; // it is the current row index + 1. 
	}
	
	return rowid;
}

REC_NO ShpScrollableFeatureReader::SearchSortedTable( SortElementDef *sortedTable, unsigned int tableSize, PropertyStub* propStubs, int numProps, FdoPropertyValueCollection* keyVal)
{
	REC_NO		rowid = 0;
	bool		found;

	// Create an element in order to pass it Compare()
	SortElementDef		elem;
	elem.index = 0;

	SortContextDef		ctx; 

	ctx.propCount = numProps;
	ctx.propCache = new DataPropertyDef*[1];
	ctx.propCache[0] = new DataPropertyDef [numProps];

	elem.ctx = &ctx;

	// Use the property stubs to match the key properties
	for ( int i = 0; i < numProps ; i++ )
	{
		FdoPtr<FdoPropertyValue>	propVal;

		PropertyStub	prop = propStubs[i];

		found = false;
		for ( int j = 0; j < keyVal->GetCount() && !found; j++ )
		{
			propVal = keyVal->GetItem(j);
			FdoStringP	name = propVal->GetName()->GetText();

			found = ( wcscmp(name, prop.m_name) == 0 );
		}

		// Invalidate a property not selected. Compare() will skip it.
		ctx.propCache[0][i].type = found? prop.m_dataType : -1;

		if ( found )
		{
			FdoPtr<FdoValueExpression> valueExpr = propVal->GetValue();

			// Copy the search key value
		    switch( prop.m_dataType )
		    {
				case FdoDataType_Boolean : 
				{
					FdoBooleanValue * typedValue = (FdoBooleanValue *)(valueExpr.p);
					ctx.propCache[0][i].value.intVal = typedValue->GetBoolean();
					break;
				}
				case FdoDataType_DateTime :
				{
					ctx.propCache[0][i].value.dateVal = new FdoDateTime();
					FdoDateTimeValue * typedValue = (FdoDateTimeValue *)(valueExpr.p);
					*ctx.propCache[0][i].value.dateVal = typedValue->GetDateTime();
					break;
				}
				case FdoDataType_Decimal :
				{
					FdoDecimalValue * typedValue = (FdoDecimalValue *)(valueExpr.p);
					ctx.propCache[0][i].value.dblVal = typedValue->GetDecimal();
					break;
				}
				case FdoDataType_Int32 : 
				{
					FdoInt32Value * typedValue = (FdoInt32Value *)(valueExpr.p);
					ctx.propCache[0][i].value.intVal = typedValue->GetInt32();
					break;
				}
				case FdoDataType_String : 
				{
					FdoStringValue * typedValue = (FdoStringValue *)(valueExpr.p);
					FdoString	*tmpStr = typedValue->GetString();
					ctx.propCache[0][i].value.strVal = new wchar_t[wcslen(tmpStr)+1];
					wcscpy( ctx.propCache[0][i].value.strVal, tmpStr );
					break;
				}
				default:
					throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_71_DATA_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoDataTypeToString((FdoDataType)ctx.propCache[i]->type)));
					break;
			}
		}		
	} // For each property in the key

	// Bsearch the sorted table for the 1st record matching the (partial) criteria
	SortElementDef	*elem1 = (SortElementDef *)bsearch( (void *)&elem, (void *)sortedTable, tableSize, sizeof(SortElementDef), compare );

	rowid = elem1->index;
	
	// Clean up
	for ( int i = 0; i < numProps ; i++ )
	{
		if ( ctx.propCache[0][i].type == FdoDataType_DateTime )
			delete ctx.propCache[0][i].value.dateVal;
		else if ( ctx.propCache[0][i].type == FdoDataType_String )
			delete [] ctx.propCache[0][i].value.strVal;
	}
	delete [] ctx.propCache[0];
	delete ctx.propCache;

	return rowid + 1; // Zero-Based, adjust
}


REC_NO ShpScrollableFeatureReader::SearchNewSortedTable( FdoPropertyValueCollection *keyVals )
{
	int		i = 0;

	// Do regular select on the selected featids in order to collect data
    FdoPtr<FdoISelect> sel = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
    sel->SetFeatureClassName (mClassName);
	sel->SetFilter(m_Filter);

	FdoPtr<ShpFeatureReader>	reader = (ShpFeatureReader*)sel->Execute();

	// Set a flag on the reader not to read the SHP file.
	reader->SetFetchGeometry( false );
	if( reader == NULL || !reader->ReadNext() )
		return 0;
	
	// TODO: Optimize memory, use reader->mFeatIdFilterExecutor to get the size of the featid list
	int maxsize = reader->mFileSet->GetShapeIndexFile ()->GetNumObjects ();
	
	PropertyStub		*propStubs = NULL;
	int					numProps = keyVals->GetCount();

	// Create and initialize a sorting context.
	SortContextDef		*ctx = new SortContextDef;
	memset (ctx, '\0', sizeof(SortContextDef));

	// Use the same comparison handler
	ctx->compareHandler = m_SortedTable->ctx->compareHandler;
	ctx->propCount = numProps;
	ctx->options = new FdoOrderingOption[ctx->propCount];
	ctx->names = new FdoString*[ctx->propCount];

	// Create properties stubs to save processing time 
	ColumnInfo* infoCols = reader->mData->GetColumnInfo ();
	int	numCols = infoCols->GetNumColumns ();

	// Get the names of the ordered properties. Create properties stubs.
	propStubs = new PropertyStub[ctx->propCount];

	for( i = 0; i < ctx->propCount; i++ )
	{		
		FdoPtr<FdoPropertyValue>	propVal = keyVals->GetItem(i);
		FdoString					*propName = propVal->GetName()->GetText();

		// Need the property type. Get it from the reader.
		propStubs[i].m_name = new wchar_t[wcslen(propName)+1];
		wcscpy( (wchar_t*)propStubs[i].m_name, propName );

		if ( wcscmp( propName, reader->mLogicalIdentityPropertyName ) == 0 )
		{
			propStubs[i].m_dataType = FdoDataType_Int32;
		}
		else
		{
			for ( int j = 0; j < numCols; j++ )
			{
				if ( wcscmp( infoCols->GetColumnNameAt(j), propName ) == 0 )
				{
					propStubs[i].m_dataType = ShpSchemaUtilities::DbfTypeToFdoType(infoCols->GetColumnTypeAt(j));
					break;
				}
			}
		}
		
		// Since the user did not specify one, use ascending 
		ctx->options[i] = FdoOrderingOption_Ascending;
		ctx->names[i] = new wchar_t[wcslen(propName)+1];
		wcscpy( (wchar_t*)ctx->names[i], propName );
	}		
	
	// initialize the ordering property cache
	ctx->propCache = new DataPropertyDef*[maxsize];

	// Populate the array to be sorted with actual data. 
	ctx->featIds = new REC_NO[maxsize];
	SortElementDef  *sortedTable = new SortElementDef[maxsize];

	// First read done above
	i = 0;
	bool	hasMore = true;

	while( hasMore )
	{
		REC_NO featid = reader->mFeatureNumber; // Zero-based

	    sortedTable[i].index = i;
	    sortedTable[i].ctx = ctx;
	    ctx->featIds[i]= featid;

		ctx->propCache[i] = new DataPropertyDef[ctx->propCount];

		// This is skipped if !doSorting
	    for( int j = 0; j < ctx->propCount; j++ )
	    {
		    PropertyStub  ps = propStubs[j];

		    ctx->propCache[i][j].type = ps.m_dataType;
			FdoString *pName = ps.m_name;

			if ( reader->IsNull( pName ) )
			{
				ctx->propCache[i][j].type = -1;
				continue;
			}

		    switch( ps.m_dataType )
		    {
				case FdoDataType_Boolean : 
				case FdoDataType_Byte : 
					ctx->propCache[i][j].value.intVal = (int)reader->GetByte( pName );
					break;

				case FdoDataType_DateTime :
					ctx->propCache[i][j].value.dateVal = new FdoDateTime();
					*ctx->propCache[i][j].value.dateVal = reader->GetDateTime( pName );
					break;

				case FdoDataType_Decimal :		  
				case FdoDataType_Double :
					ctx->propCache[i][j].value.dblVal = reader->GetDouble( pName );
					break;

				case FdoDataType_Int16 : 
					ctx->propCache[i][j].value.intVal = reader->GetInt16( pName );
					break;

				case FdoDataType_Int32 : 
					ctx->propCache[i][j].value.intVal = reader->GetInt32( pName );
					break;

				case FdoDataType_Int64 : 
					ctx->propCache[i][j].value.int64Val = reader->GetInt64( pName );
					break;

				case FdoDataType_Single :
					ctx->propCache[i][j].value.dblVal = reader->GetSingle( pName );
					break;

				case FdoDataType_String : 
				{
					FdoString*	tmpStr = reader->GetString( pName );
					ctx->propCache[i][j].value.strVal = new wchar_t[wcslen(tmpStr)+1];
					wcscpy( ctx->propCache[i][j].value.strVal , tmpStr );
					break;
				}
			
		      default:
                  throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_71_DATA_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoDataTypeToString(ps.m_dataType)));
			      break;
		    }
	    }
        i++;

		// get the next record
		hasMore = reader->ReadNext();
	}
	maxsize = i;

	// Reset the flag.
	reader->SetFetchGeometry( true );

	// Build the sorted list
	qsort( (void*)sortedTable, maxsize, sizeof(SortElementDef), compare );
	
	// Do the search. It returns the rowid in this temporary table
	REC_NO rowid = SearchSortedTable( sortedTable, maxsize, propStubs, numProps, keyVals);

	// Clean up
	for ( int i = 0; i < numProps ; i++ )
	{
		if ( ctx->propCache[0][i].type == FdoDataType_DateTime )
			delete ctx->propCache[0][i].value.dateVal;
		else if ( ctx->propCache[0][i].type == FdoDataType_String )
			delete [] ctx->propCache[0][i].value.strVal;
	}
	delete [] ctx->propCache[0];
	delete ctx->propCache;
	delete ctx;

	return rowid;
}
