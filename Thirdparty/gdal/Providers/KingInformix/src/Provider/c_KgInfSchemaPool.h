/*
* Copyright (C) 2007  Haris Kurtagic
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

#ifndef _c_KgInfSchemaPool_h
#define _c_KgInfSchemaPool_h



#include "c_KgInfSchemaDesc.h"


typedef struct t_SchemaPoolDesc
{
  string m_ConnectionString;
  
  FdoPtr<c_KgInfSchemaDesc> m_SchemaData;
  
}t_SchemaPoolDesc;


class c_KgInfSchemaPool
{
public:
  c_KgInfSchemaPool(void);

  ~c_KgInfSchemaPool(void);

protected:  
  static vector<t_SchemaPoolDesc> g_SchemaPoolDesc;
  
  static FdoCommonThreadMutex m_Mutex;
  
public:  
  static c_KgInfSchemaDesc* GetSchemaData(c_KgInfConnection* Connection);
  
};


#endif