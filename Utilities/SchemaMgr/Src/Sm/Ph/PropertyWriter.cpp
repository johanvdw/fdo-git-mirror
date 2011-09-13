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

#include "stdafx.h"
#include <Sm/Ph/PropertyWriter.h>

FdoSmPhPropertyWriter::FdoSmPhPropertyWriter( FdoSmPhMgrP mgr) : 
    FdoSmPhWriter( MakeWriter(mgr) )
{
}

FdoSmPhPropertyWriter::~FdoSmPhPropertyWriter(void)
{
}

FdoStringP FdoSmPhPropertyWriter::GetName()
{
	return(GetString(L"", L"attributename"));
}

FdoStringP FdoSmPhPropertyWriter::GetColumnName()
{
	return(GetString(L"", L"columnname"));
}

FdoStringP FdoSmPhPropertyWriter::GetColumnType()
{
	return(GetString(L"", L"columntype"));
}

bool FdoSmPhPropertyWriter::GetIsFixedColumn()
{
	return(GetBoolean(L"", L"isfixedcolumn"));
}

bool FdoSmPhPropertyWriter::GetIsColumnCreator()
{
	return(GetBoolean(L"", L"iscolumncreator"));
}

FdoStringP FdoSmPhPropertyWriter::GetDescription()
{
	return(GetString(L"", L"description"));
}

FdoInt64 FdoSmPhPropertyWriter::GetClassId()
{
	return(GetInt64(L"", L"classid"));
}


FdoStringP FdoSmPhPropertyWriter::GetDataType()
{
	return(GetString(L"", L"attributetype"));
}

FdoStringP FdoSmPhPropertyWriter::GetGeometryType()
{
	return(GetString(L"", L"geometrytype"));
}

int FdoSmPhPropertyWriter::GetIdPosition()
{
	return((int) GetInteger(L"", L"idposition"));
}

bool FdoSmPhPropertyWriter::GetIsNullable()
{
	return(GetBoolean(L"", L"isnullable"));
}

bool FdoSmPhPropertyWriter::GetIsReadOnly()
{
	return(GetBoolean(L"", L"isreadonly"));
}

bool FdoSmPhPropertyWriter::GetIsFeatId()
{
	return(GetBoolean(L"", L"isfeatid"));
}

bool FdoSmPhPropertyWriter::GetIsSystem()
{
	return(GetBoolean(L"", L"issystem"));
}

int FdoSmPhPropertyWriter::GetLength()
{
	return((int) GetLong(L"", L"columnsize"));
}

int FdoSmPhPropertyWriter::GetScale()
{
	return((int) GetLong(L"", L"columnscale"));
}

FdoStringP FdoSmPhPropertyWriter::GetDefaultValue()
{
	// default values are not supported
	return(L"");
}

FdoStringP FdoSmPhPropertyWriter::GetTableName()
{
	return(GetString(L"", L"tablename"));
}

FdoStringP FdoSmPhPropertyWriter::GetRootObjectName()
{
    FdoSmPhField* pField = (FdoSmPhField*) GetField( L"", L"rootobjectname" );

    if ( pField ) 
        return(GetString(L"", L"rootobjectname"));
    else
        return(GetString(L"", L"roottablename"));
}

FdoStringP FdoSmPhPropertyWriter::GetUser()
{
	return(GetString(L"", L"owner"));
}

bool FdoSmPhPropertyWriter::GetHasElevation()
{
	return(GetBoolean(L"", L"haselevation"));
}

bool FdoSmPhPropertyWriter::GetHasMeasure()
{
	return(GetBoolean(L"", L"hasmeasure"));
}

bool FdoSmPhPropertyWriter::GetIsAutoGenerated()
{
	return(GetBoolean(L"", L"isautogenerated"));
}

bool FdoSmPhPropertyWriter::GetIsRevisionNumber()
{
    return(GetBoolean(L"", L"isrevisionnumber"));
}

FdoStringP FdoSmPhPropertyWriter::GetSequenceName()
{
	return(GetString(L"", L"sequencename"));
}

void FdoSmPhPropertyWriter::SetName( FdoStringP sValue )
{
	SetString(L"", L"attributename", sValue);
}

void FdoSmPhPropertyWriter::SetColumnName( FdoStringP sValue )
{
	SetString(L"", L"columnname", sValue);
}

void FdoSmPhPropertyWriter::SetColumnType( FdoStringP sValue )
{
	SetString(L"", L"columntype", sValue);
}

void FdoSmPhPropertyWriter::SetIsFixedColumn( bool bValue )
{
	SetBoolean(L"", L"isfixedcolumn", bValue);
}

void FdoSmPhPropertyWriter::SetIsColumnCreator( bool bValue )
{
    FdoSmPhFieldP pField = GetField( L"", L"iscolumncreator" );

    if ( pField && pField->GetColumn() ) 
    	SetBoolean(L"", L"iscolumncreator", bValue);
}

void FdoSmPhPropertyWriter::SetDescription( FdoStringP sValue )
{
	SetString(L"", L"description", sValue);
}

void FdoSmPhPropertyWriter::SetClassId( FdoInt64 lValue )
{
	SetInt64(L"", L"classid", lValue);
}

void FdoSmPhPropertyWriter::SetDataType( FdoStringP sValue )
{
	SetString(L"", L"attributetype", sValue);
}

void FdoSmPhPropertyWriter::SetGeometryType( FdoStringP sValue )
{
    FdoSmPhFieldP pField = GetField( L"", L"geometrytype" );
 
    if ( pField && pField->GetColumn() ) 
        SetString(L"", L"geometrytype", sValue);
    else
     	SetString(L"", L"geometrytype", L"");
}

void FdoSmPhPropertyWriter::SetIdPosition( int iValue )
{
	SetInteger(L"", L"idposition", iValue);
}

void FdoSmPhPropertyWriter::SetIsNullable( bool bValue )
{
	SetBoolean(L"", L"isnullable", bValue);
}

void FdoSmPhPropertyWriter::SetIsReadOnly( bool bValue )
{
	SetBoolean(L"", L"isreadonly", bValue);
}

void FdoSmPhPropertyWriter::SetIsFeatId( bool bValue )
{
	SetBoolean(L"", L"isfeatid", bValue);
}

void FdoSmPhPropertyWriter::SetIsSystem( bool bValue )
{
	SetBoolean(L"", L"issystem", bValue);
}

void FdoSmPhPropertyWriter::SetLength( int iValue )
{
	SetLong(L"", L"columnsize", iValue);
}

void FdoSmPhPropertyWriter::SetScale( int iValue )
{
	SetLong(L"", L"columnscale", iValue);
}

void FdoSmPhPropertyWriter::SetTableName( FdoStringP sValue )
{
	SetString(L"", L"tablename", GetManager()->DbObject2MetaSchemaName(sValue));
}

void FdoSmPhPropertyWriter::SetRootObjectName( FdoStringP sValue )
{
    FdoSmPhFieldP pField = GetField( L"", L"rootobjectname" );

    if ( pField && pField->GetColumn() ) 
    	SetString(L"", L"rootobjectname", GetManager()->DbObject2MetaSchemaName(sValue));
    else
    	SetString(L"", L"roottablename", GetManager()->DbObject2MetaSchemaName(sValue));
}

void FdoSmPhPropertyWriter::SetUser( FdoStringP sValue )
{
	SetString(L"", L"owner", sValue);
}

void FdoSmPhPropertyWriter::SetHasElevation( bool bValue )
{
	SetBoolean(L"", L"haselevation", bValue);
}

void FdoSmPhPropertyWriter::SetHasMeasure( bool bValue )
{
	SetBoolean(L"", L"hasmeasure", bValue);
}


void FdoSmPhPropertyWriter::SetIsAutoGenerated( bool bValue )
{
    FdoSmPhFieldP pField = GetField( L"", L"isautogenerated" );

    if ( (!GetIsFeatId()) || (pField && pField->GetColumn()) ) 
    	SetBoolean(L"", L"isautogenerated", bValue);
}

void FdoSmPhPropertyWriter::SetSequenceName( FdoStringP bValue )
{
    SetString(L"", L"sequencename", bValue);
}

void FdoSmPhPropertyWriter::SetIsRevisionNumber( bool bValue )
{
    FdoSmPhFieldP pField = GetField( L"", L"isrevisionnumber" );

    if (pField && pField->GetColumn())
        SetBoolean(L"",  L"isrevisionnumber" , bValue);
}
       
void FdoSmPhPropertyWriter::Add()
{
    FdoSmPhWriter::Add();
}

void FdoSmPhPropertyWriter::Modify( FdoInt64 classId, FdoStringP sName )
{
    FdoSmPhWriter::Modify(
        FdoStringP::Format(
#ifdef _WIN32
            L"where classid = %I64d and attributename = %ls",
#else
            L"where classid = %lld and attributename = %ls",
#endif
			classId,
			(FdoString*) GetManager()->FormatSQLVal( sName, FdoSmPhColType_String )
		)
	);
}

void FdoSmPhPropertyWriter::Delete( FdoInt64 classId, FdoStringP sName )
{
    FdoSmPhWriter::Delete(
        FdoStringP::Format( 
#ifdef _WIN32
            L"where classid = %I64d and attributename = %ls",
#else
            L"where classid = %lld and attributename = %ls",
#endif
			classId,
			(FdoString*) GetManager()->FormatSQLVal( sName, FdoSmPhColType_String )
		)
    );
}

FdoSmPhRowP FdoSmPhPropertyWriter::MakeRow( FdoSmPhMgrP mgr )
{
    bool hasMs = FdoSmPhOwnerP(mgr->GetOwner())->GetHasMetaSchema();
    FdoStringP attDefTable = mgr->GetDcDbObjectName(L"f_attributedefinition");

    FdoSmPhRowP row = new FdoSmPhRow( 
        mgr, 
        L"f_attributedefinition", 
        hasMs ? mgr->FindDbObject(attDefTable) : FdoSmPhDbObjectP() 
    );

    // Each field adds itself to the row.
    FdoSmPhFieldP field = new FdoSmPhField( row, L"classid" );
    field = new FdoSmPhField( row, L"tablename" );
    field = new FdoSmPhField( row, L"attributename" );
    field = new FdoSmPhField( row, L"idposition", (FdoSmPhColumn*) NULL, L"0" );
    field = new FdoSmPhField( row, L"columnname" );
    field = new FdoSmPhField( row, L"description" );
    field = new FdoSmPhField( row, L"attributetype" );
    field = new FdoSmPhField( row, L"columntype" );
    field = new FdoSmPhField( row, L"columnsize", (FdoSmPhColumn*) NULL, L"0" );
    field = new FdoSmPhField( row, L"columnscale", (FdoSmPhColumn*) NULL, L"0" );
    field = new FdoSmPhField( row, L"isnullable" );
    field = new FdoSmPhField( row, L"isreadonly" );
    field = new FdoSmPhField( row, L"isfeatid" );
    field = new FdoSmPhField( row, L"issystem" );
    field = new FdoSmPhField( row, L"owner" );

    field = new FdoSmPhField(
        row, 
        L"rootobjectname", 
        row->CreateColumnDbObject(L"rootobjectname",true)
    );

    field = new FdoSmPhField(
        row, 
        L"roottablename",
        row->CreateColumnDbObject(L"roottablename",true)
    );

    field = new FdoSmPhField(
        row, 
        L"isfixedcolumn",
        row->CreateColumnBool(L"isfixedcolumn",true)
    );

    field = new FdoSmPhField(
        row, 
        L"iscolumncreator", 
        row->CreateColumnBool(L"iscolumncreator",true)
    );

    field = new FdoSmPhField(
        row, 
        L"haselevation",
        row->CreateColumnBool(L"haselevation",true)
    );

    field = new FdoSmPhField(
        row, 
        L"hasmeasure",
        row->CreateColumnBool(L"hasmeasure",true)
    );

    field = new FdoSmPhField(
        row, 
        L"isautogenerated",
        row->CreateColumnBool(L"isautogenerated",true)
    );

    field = new FdoSmPhField(
        row, 
        L"isrevisionnumber",
        row->CreateColumnBool(L"isrevisionnumber",true)
    );

    field = new FdoSmPhField(
        row, 
        L"sequencename",
        row->CreateColumnDbObject(L"sequencename",true)
    );

    field = new FdoSmPhField(
        row, 
        L"geometrytype", 
        row->CreateColumnChar(L"geometrytype",true,255)
    );

    return( row );
}

FdoSmPhWriterP FdoSmPhPropertyWriter::MakeWriter( FdoSmPhMgrP mgr )
{
    FdoSmPhCommandWriterP subWriter = mgr->CreateCommandWriter( MakeRow(mgr) );

    return subWriter.p->SmartCast<FdoSmPhWriter>();
}
