/*
* Copyright (C) 2009  SL-King d.o.o
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
*/

#include "stdafx.h"
#include "c_KgOraSqlParamDesc.h"
#include "c_FilterStringBuffer.h"
#include "c_FgfToSdoGeom.h"
#include "c_Ora_API2.h"
#include "c_FdoOra_API3.h"
#include "c_KgOraSqlParamDesc.h"

c_KgOraSqlParamDesc::c_KgOraSqlParamDesc() 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
}
c_KgOraSqlParamDesc::c_KgOraSqlParamDesc(FdoString* UserName) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
  
  m_ParamType=e_UserParam;m_UserParamName=UserName; 
}

c_KgOraSqlParamDesc::c_KgOraSqlParamDesc(FdoByteArray* Geom,const c_KgOraSridDesc& Srid) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_Geometry;
  m_ParamGeometry=Geom; 
  
  Geom->AddRef();
  m_OracleSrid=Srid;
}

c_KgOraSqlParamDesc::c_KgOraSqlParamDesc(FdoDataValue* DataValue) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_DataValue;
  m_ParamDataValue=DataValue; 
  
  m_ParamDataValue->AddRef();
}
c_KgOraSqlParamDesc::c_KgOraSqlParamDesc(const c_KgOraSridDesc& Srid,double MinX,double MinY,double MaxX,double MaxY) 
{ 
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;
  
  
  m_ParamType=e_OptimizedRect;
  m_OptimizedRect.m_MinX=MinX;
  m_OptimizedRect.m_MinY=MinY;
  m_OptimizedRect.m_MaxX=MaxX;
  m_OptimizedRect.m_MaxY=MaxY;
  
  m_OracleSrid=Srid;
  
  
}

c_KgOraSqlParamDesc::~c_KgOraSqlParamDesc() 
{ 
  SetNull();
}

void c_KgOraSqlParamDesc::SetNull()
{
  //if( (m_ParamType==e_Geometry) && (m_ParamGeometry) ) delete m_ParamGeometry; 
  if( (m_ParamType==e_Geometry) && (m_ParamGeometry) ) m_ParamGeometry->Release(); 
  
  if( (m_ParamType==e_DataValue) && m_ParamDataValue ) m_ParamDataValue->Release();
  
  m_ParamType=e_Uknown;
  m_ParamGeometry=NULL; 
  m_ParamDataValue=NULL;  
}
c_KgOraSqlParamDesc::e_ParamType c_KgOraSqlParamDesc::GetParamType() const 
{ 
  return m_ParamType; 
}


FdoByteArray* c_KgOraSqlParamDesc::GetGeometry() const 
{ 
  return m_ParamGeometry; 
}
void c_KgOraSqlParamDesc::SetGeometry(FdoByteArray* Geom,const c_KgOraSridDesc& Srid) 
{ 
  SetNull();
  
  m_ParamType=e_Geometry;
  m_ParamGeometry =  Geom; 
  m_OracleSrid=Srid;
  if(m_ParamGeometry) m_ParamGeometry->AddRef();
}
  
void c_KgOraSqlParamDesc::SetDataValue(FdoDataValue* DataValue) 
{ 
  SetNull();
  
  m_ParamType=c_KgOraSqlParamDesc::e_DataValue;
  m_ParamDataValue =  DataValue; 
}
  
FdoString* c_KgOraSqlParamDesc::GetUserParamName() const 
{ 
  return m_UserParamName; 
}
void c_KgOraSqlParamDesc::SetUserParamName(FdoString* Name) 
{ 
  SetNull();
  
  m_ParamType=c_KgOraSqlParamDesc::e_UserParam;
  m_UserParamName =  Name; 
}

void c_KgOraSqlParamDesc::ApplySqlParameter(c_Oci_Statement* OraStm,int SqlParamNum) 
{ 
  switch( m_ParamType )
  {
    case e_Geometry:
    {
      if( m_ParamGeometry )
      {
          
        c_SDO_GEOMETRY * sdogeom = c_SDO_GEOMETRY::Create(OraStm->m_OciConn);
        
        c_FgfToSdoGeom fgftosdo;
        
        
        
        
        if( fgftosdo.ToSdoGeom((int*)m_ParamGeometry->GetData(),m_OracleSrid.m_OraSrid,sdogeom) == c_FgfToSdoGeom::e_Ok )
        {
          OraStm->BindSdoGeomValue(SqlParamNum,sdogeom);  
        }
        else
        {
          delete sdogeom;
          OraStm->BindSdoGeomValue(SqlParamNum,NULL);  
        }
      
        #ifdef _KGORA_EXTENDED_LOG
        {
          char* buff = c_Ora_API2::SdoGeomToString(sdogeom);
          D_KGORA_ELOG_WRITE2("SQL Param %d Geometry='%s'",SqlParamNum,buff);
          delete [] buff;
        }
        #endif
        
      }
      else
      {
        
        OraStm->BindSdoGeomValue( SqlParamNum,NULL);
        #ifdef _KGORA_EXTENDED_LOG
        {
        D_KGORA_ELOG_WRITE1("SQL Param %d Geometry=NULL",SqlParamNum);
        
        }
        #endif
      }
    }
    break;
    case e_OptimizedRect:
    {
      c_SDO_GEOMETRY *sdorect = c_Ora_API2::CreateOptimizedRect(OraStm->m_OciConn,m_OracleSrid.m_IsGeodetic,m_OracleSrid.m_OraSrid,m_OptimizedRect.m_MinX,m_OptimizedRect.m_MinY,m_OptimizedRect.m_MaxX,m_OptimizedRect.m_MaxY);
      OraStm->BindSdoGeomValue(SqlParamNum,sdorect);   
      #ifdef _KGORA_EXTENDED_LOG
        {
          char* buff = c_Ora_API2::SdoGeomToString(sdorect);
          D_KGORA_ELOG_WRITE2("SQL Param %d  OptimizedRect='%s'",SqlParamNum,buff);
          delete [] buff;
        }
        #endif     
    }
    break;
    
    case e_DataValue:
    {
      
      
      if( c_FdoOra_API3::SetOracleStatementData(OraStm,SqlParamNum,m_ParamDataValue) )
      {
      }
      
      #ifdef _KGORA_EXTENDED_LOG
      {
      FdoStringP fdostr = m_ParamDataValue->ToString();
      D_KGORA_ELOG_WRITE2("SQL Param %d Data='%s'",SqlParamNum,(const char*)fdostr);
      
      }
      #endif
    }
    break;  
  }
}//end of  c_KgOraSqlParamDesc::ApplySqlParameters

void c_KgOraSqlParamDesc::ApplySqlParameter(c_Oci_Statement* OraStm,const wchar_t* SqlParamName) 
{ 
  switch( m_ParamType )
  {
  case e_Geometry:
    {
      if( m_ParamGeometry )
      {

        c_SDO_GEOMETRY * sdogeom = c_SDO_GEOMETRY::Create(OraStm->m_OciConn);

        c_FgfToSdoGeom fgftosdo;

        if( fgftosdo.ToSdoGeom((int*)m_ParamGeometry->GetData(),m_OracleSrid.m_OraSrid,sdogeom) == c_FgfToSdoGeom::e_Ok )
        {
          OraStm->BindSdoGeomValue(SqlParamName,sdogeom);  
        }
        else
        {
          delete sdogeom;
          OraStm->BindSdoGeomValue(SqlParamName,NULL);  
        }

#ifdef _KGORA_EXTENDED_LOG
        {
          char* buff = c_Ora_API2::SdoGeomToString(sdogeom);
          FdoStringP sval(SqlParamName); 
          
          D_KGORA_ELOG_WRITE2("SQL Param %s Geometry='%s'",(const char*)sval,buff);
          delete [] buff;
        }
#endif

      }
      else
      {
        
        OraStm->BindSdoGeomValue( SqlParamName,NULL );
#ifdef _KGORA_EXTENDED_LOG
        {
          FdoStringP sval(SqlParamName); 
          D_KGORA_ELOG_WRITE1("SQL Param %s Geometry=NULL",(const char*)sval);

        }
#endif
      }
    }
    break;
  case e_OptimizedRect:
    {
      c_SDO_GEOMETRY *sdorect = c_Ora_API2::CreateOptimizedRect(OraStm->m_OciConn,m_OracleSrid.m_IsGeodetic,m_OracleSrid.m_OraSrid,m_OptimizedRect.m_MinX,m_OptimizedRect.m_MinY,m_OptimizedRect.m_MaxX,m_OptimizedRect.m_MaxY);
      OraStm->BindSdoGeomValue(SqlParamName,sdorect);   
#ifdef _KGORA_EXTENDED_LOG
      {
        FdoStringP sval(SqlParamName); 
        char* buff = c_Ora_API2::SdoGeomToString(sdorect);
        D_KGORA_ELOG_WRITE2("SQL Param %s  OptimizedRect='%s'",(const char*)sval,buff);
        delete [] buff;
      }
#endif     
    }
    break;

  case e_DataValue:
    {


      if( c_FdoOra_API3::SetOracleStatementData(OraStm,SqlParamName,m_ParamDataValue) )
      {
      }

#ifdef _KGORA_EXTENDED_LOG
      {
        FdoStringP sval(SqlParamName); 
        FdoStringP fdostr = m_ParamDataValue->ToString();
        D_KGORA_ELOG_WRITE2("SQL Param %s Data='%s'",(const char*)sval,(const char*)fdostr);

      }
#endif
    }
    break;  
  }
}//end of  c_KgOraSqlParamDesc::ApplySqlParameters

