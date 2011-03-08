/*
* Copyright (C) 2006  SL-King d.o.o
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

#ifndef _c_Oci_ColumnData_h
#define _c_Oci_ColumnData_h



#ifndef OCI_ORACLE
#include <oci.h>
#include <oci1.h>
#endif

#include "OCI_SDO_GEOM_TYPES.h"


class c_Oci_Connection;



class c_Oci_ColumnData
{
public:
    enum e_OciDataBufferType
    {
      e_Undefined=0,
      e_MemBuffer, // it means oci data type has not been recognised
      e_OciNumber,
      e_OciDateTime,
      e_OciSdoGeometry,
      e_OciSdoDimArray,
      e_OciString,
      e_OciLongRaw,
      e_OciBlob,
      e_OciClob,
    };


  c_Oci_ColumnData();
  ~c_Oci_ColumnData();
  void Set( c_Oci_Connection*OciConn,int ColumnNumber,int OciDataType,const wchar_t*TypeName,int ColumnSize,int DataArraySize );

  
  void* GetDataDefineBuffer();
  void* GetDataIndDefineBuffer();
  
  int GetDataDefineType();
  long GetDataDefineSize();

  OCIType* GetDataOciType() { return m_OciType; }
  
  
  void NextRow()
  {
    m_CurrentRow++;
    if( m_CurrentRow>= m_DataArraySize ) m_CurrentRow=0;
    
    //if( m_CurrentRow == 0 ) m_CurrentPtr_ScalarInd = m_ScalarInd;
  }
  void StartRow()
  {
    m_CurrentRow = -1;    
  }
    
  bool IsNull();
  OCINumber* GetOciNumber();
  OCIDate* GetOciDate();
  const wchar_t* GetString();
  SDO_GEOMETRY_TYPE* GetSdoGeom(SDO_GEOMETRY_ind** GeomInd=NULL);
  OCIArray* GetSdoDimArray();
  unsigned char* GetLongRaw();
  long GetLongRawLength();
  void* GetDataRealLengthBuffer();
  void GetLobData(unsigned long& BuffSize,void* BuffPtr);
  bool IsClob();
  bool IsBlob();
protected:
  int m_CurrentRow;
  sb2* m_CurrentPtr_ScalarInd;

  c_Oci_Connection*m_OciConn;
  OCIType *m_OciType;

  int m_ColumnNumber;
  int m_OciDataType;
  long m_ColSize;
  
  int m_DataArraySize;
  
  unsigned char* m_LobBuff; // when data type is Lob it is used to allocate memory to get data from loblocator
                            // it is valid only for one GetLongRaw ( next GetLongRaw will deallocate previous one)
  unsigned long m_LobBuffSize; // how much was allocated                            
  
  e_OciDataBufferType m_DataBufferType;
  
  union {
    char** m_MemBuffPtrArray;
    
    
    OCINumber* m_DataNumber;
    
    OCIDate* m_DataDateTime;
    OCILobLocator** m_DataLobLocator;
    
    //wchar_t** m_DataStringPtrArray;
    wchar_t* m_DataStringPtr;
    
    SDO_GEOMETRY_TYPE** m_DataSdoGeom;
    
    OCIArray** m_DataSdoDimArray;
    
    //OCILobLocator** m_DataLob;
    ub1* m_DataLongRaw;
    
  };
  
  ub2* m_DataLength;
  
  union 
  {
    sb2 * m_ScalarInd;
    SDO_GEOMETRY_ind** m_SdoGeomInd;
    sb2** m_SdoDimArrayInd;
  };
};


#endif
