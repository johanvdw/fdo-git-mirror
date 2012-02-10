/*
 * Copyright (C) 2006  Haris Kurtagic
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
#ifndef FdoKgOraClassDefinition_H
#define FdoKgOraClassDefinition_H

#ifdef _WIN32
#pragma once
#endif //_WIN32



class c_KgMssOverClassDefinition : public FdoPhysicalClassMapping
{
	typedef FdoPhysicalClassMapping BaseType;

public:
    FDOKGMSS_API static c_KgMssOverClassDefinition* Create();

public:
    FDOKGMSS_API c_KgMssOverPropertyDefinitionCollection* GetProperties() const;
    
    
    FDOKGMSS_API bool GetIsSqlGeomTable() const
    {
      return m_SqlGeomFullName.GetLength() > 0;
    }
    
    FDOKGMSS_API FdoString* GetSqlQueryFullName() const { return m_SqlQueryFullName; }
    FDOKGMSS_API FdoString* GetSqlGeomFullName() const 
    { 
      if( m_SqlGeomFullName.GetLength() > 0 )
        return m_SqlGeomFullName; 
      else
        return m_SqlQueryFullName;
    }
    
    FDOKGMSS_API void SetSqlQueryFullName(FdoString * SqlQueryFullName) { m_SqlQueryFullName=SqlQueryFullName; };
    FDOKGMSS_API void SetSqlGeomFullName(FdoString * SqlGeomFullName) { m_SqlGeomFullName=SqlGeomFullName; };
    
    FDOKGMSS_API void SetSqlQueryFullName(FdoString* Schema,FdoString* Table) 
    { 
      FdoStringP f1;
      m_SqlQueryFullName = "";
      f1 = Schema;
      if( f1.GetLength() > 0 )
      {
        //m_SqlQueryFullName = m_SqlQueryFullName + L"[" + f1 + L"].";
        m_SqlQueryFullName = m_SqlQueryFullName + f1 + L".";
      }
      f1 = Table;
      //m_SqlQueryFullName = m_SqlQueryFullName + L"[" + f1 + L"]";            
      m_SqlQueryFullName = m_SqlQueryFullName + f1;
    };
    // set geometry table full name without "[" because I need to sewnd to 
    // ST.FilterQuery ( and ither) name without [ ]
    FDOKGMSS_API void SetSqlGeomFullName(FdoString* Schema,FdoString* Table) 
    { 
      FdoStringP f1;
      m_SqlGeomFullName = "";
      f1 = Schema;
      if( f1.GetLength() > 0 )
      {
        //m_SqlGeomFullName = m_SqlGeomFullName + L"[" + f1 + L"].";
        m_SqlGeomFullName = m_SqlGeomFullName + f1 + L".";
      }
      f1 = Table;
      //m_SqlGeomFullName = m_SqlGeomFullName + L"[" + f1 + L"]";            
      m_SqlGeomFullName = m_SqlGeomFullName + f1 ;            
    };
    
    FDOKGMSS_API void SetUseSequenceForIdentity(FdoString * TableSequence) { m_UseSequenceForIdentity=TableSequence; };
    FDOKGMSS_API FdoString* GetUseSequenceForIdentity() const { return m_UseSequenceForIdentity; }
    
    FDOKGMSS_API void SetOraTableAliasNum(int AliasNum) { char tbuff[16]; sprintf(tbuff,"a%d",AliasNum);  m_OraTableAlias=tbuff; };
    FDOKGMSS_API const char* GetOraTableAlias() { return  m_OraTableAlias; };
	

    FDOKGMSS_API void SetPointGeometry(FdoString *GeomPropertyName, FdoString *X_OraColumn, FdoString *Y_OraColumn, FdoString *Z_OraColumn )
    {
      m_IsPointGeometry = true;  
      m_PoinGeometry_PropertyName = GeomPropertyName;
      m_PoinGeometry_X_OraColumn = X_OraColumn;
      m_PoinGeometry_Y_OraColumn = Y_OraColumn;
      m_PoinGeometry_Z_OraColumn = Z_OraColumn;
    }
    FDOKGMSS_API void SetIsPointGeometry(bool IsPointGeometry)
    {
      m_IsPointGeometry = IsPointGeometry;  
    }
    FDOKGMSS_API bool GetIsPointGeometry() { return m_IsPointGeometry; }
    FDOKGMSS_API FdoString* GetPoinGeometryPropertyName() { return m_PoinGeometry_PropertyName; };
    FDOKGMSS_API FdoString* GetPointXOraColumn() { return m_PoinGeometry_X_OraColumn; };
    FDOKGMSS_API FdoString* GetPointYOraColumn() { return m_PoinGeometry_Y_OraColumn; };
    FDOKGMSS_API FdoString* GetPointZOraColumn() { return m_PoinGeometry_Z_OraColumn; };
    
    	

    FDOKGMSS_API virtual void InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs);
    FDOKGMSS_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs);
    FDOKGMSS_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGMSS_API virtual void _writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags);

    /// find property with column name
    FDOKGMSS_API c_KgMssOverPropertyDefinition* FindByColumnName(FdoString* ColumnName);

protected:
    c_KgMssOverClassDefinition(void);
    virtual ~c_KgMssOverClassDefinition(void);
    virtual void Dispose(void);

private:
	  FdoStringP m_SqlQueryFullName; // complete table name in Oracle ( [owner].[table] ) for fdo class
	  FdoStringP m_SqlGeomFullName; // complete table name in Oracle ( [owner].[table] ) for fdo class
	  
	  FdoStringP m_OraTableAlias; // table allias used for in SQL (generated in describe schema)
	  FdoStringP m_UseSequenceForIdentity; // if this
	  
	  // Class can have geometry property defined not in standard way as column in oracle table (SDO_GEOMETRY)
	  // but as point which is created from numeric colimns X,Y and/or Z
	  bool m_IsPointGeometry; // true if geometry property is created
	  FdoStringP m_PoinGeometry_PropertyName; // name of geometry property in fdo class- must be same as
	  FdoStringP m_PoinGeometry_X_OraColumn;  // name of oracle column (number) which is used for X coordinate of point
	  FdoStringP m_PoinGeometry_Y_OraColumn;  // name of oracle column (number) which is used for Y coordinate of point
	  FdoStringP m_PoinGeometry_Z_OraColumn;  // name of oracle column (number) which is used for Z coordinate of point - can be empty for 2D points
	  
    FdoPtr<c_KgMssOverPropertyDefinitionCollection> m_Properties;
};



#endif // FdoKgOraClassDefinition_H


