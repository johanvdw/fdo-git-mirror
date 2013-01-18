/*
 * Copyright (C) 2010  SL-King d.o.o
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
#ifndef FdoKgKmlClassDefinition_H
#define FdoKgKmlClassDefinition_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <FdoKgKmlPropertyDefinitionCollection.h>

class FdoKgKmlClassDefinition : public FdoPhysicalClassMapping
{
	typedef FdoPhysicalClassMapping BaseType;

public:
    FDOKGKML_API static FdoKgKmlClassDefinition* Create();

public:
    FDOKGKML_API FdoKgKmlPropertyDefinitionCollection* GetProperties() const;
    
    FDOKGKML_API FdoString* GetOracleFullTableName() const { return m_OracleTableFullName; }
    FDOKGKML_API void SetOracleFullTableName(FdoString * OracleFullName) { m_OracleTableFullName=OracleFullName; };
    
    FDOKGKML_API void SetUseSequenceForIdentity(FdoString * TableSequence) { m_UseSequenceForIdentity=TableSequence; };
    FDOKGKML_API FdoString* GetUseSequenceForIdentity() const { return m_UseSequenceForIdentity; }
    
    FDOKGKML_API void SetOraTableAliasNum(int AliasNum) { char tbuff[16]; sprintf(tbuff,"a%d",AliasNum);  m_OraTableAlias=tbuff; };
    FDOKGKML_API const char* GetOraTableAlias() { return  m_OraTableAlias; };
	

    FDOKGKML_API void SetPointGeometry(FdoString *GeomPropertyName, FdoString *X_OraColumn, FdoString *Y_OraColumn, FdoString *Z_OraColumn )
    {
      m_IsPointGeometry = true;  
      m_PoinGeometry_PropertyName = GeomPropertyName;
      m_PoinGeometry_X_OraColumn = X_OraColumn;
      m_PoinGeometry_Y_OraColumn = Y_OraColumn;
      m_PoinGeometry_Z_OraColumn = Z_OraColumn;
    }
    FDOKGKML_API void SetIsPointGeometry(bool IsPointGeometry)
    {
      m_IsPointGeometry = IsPointGeometry;  
    }
    FDOKGKML_API bool GetIsPointGeometry() { return m_IsPointGeometry; }
    FDOKGKML_API FdoString* GetPoinGeometryPropertyName() { return m_PoinGeometry_PropertyName; };
    FDOKGKML_API FdoString* GetPointXOraColumn() { return m_PoinGeometry_X_OraColumn; };
    FDOKGKML_API FdoString* GetPointYOraColumn() { return m_PoinGeometry_Y_OraColumn; };
    FDOKGKML_API FdoString* GetPointZOraColumn() { return m_PoinGeometry_Z_OraColumn; };
    
    	

    FDOKGKML_API virtual void InitFromXml(FdoXmlSaxContext* Context, FdoXmlAttributeCollection* Attrs);
    FDOKGKML_API virtual FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, FdoXmlAttributeCollection* Attrs);
    FDOKGKML_API virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGKML_API virtual void _writeXml(FdoXmlWriter* Writer, const FdoXmlFlags* Flags);

    /// find property with column name
    FDOKGKML_API FdoKgKmlPropertyDefinition* FindByColumnName(FdoString* ColumnName);

protected:
    FdoKgKmlClassDefinition(void);
    virtual ~FdoKgKmlClassDefinition(void);
    virtual void Dispose(void);

private:
	  FdoStringP m_OracleTableFullName; // complete table name in Oracle (owner.table) for fdo class
	  FdoStringP m_OraTableAlias; // table allias used for in SQL (generated in describe schema)
	  FdoStringP m_UseSequenceForIdentity; // if this
	  
	  // Class can have geometry property defined not in standard way as column in oracle table (SDO_GEOMETRY)
	  // but as point which is created from numeric colimns X,Y and/or Z
	  bool m_IsPointGeometry; // true if geometry property is created
	  FdoStringP m_PoinGeometry_PropertyName; // name of geometry property in fdo class- must be same as
	  FdoStringP m_PoinGeometry_X_OraColumn;  // name of oracle column (number) which is used for X coordinate of point
	  FdoStringP m_PoinGeometry_Y_OraColumn;  // name of oracle column (number) which is used for Y coordinate of point
	  FdoStringP m_PoinGeometry_Z_OraColumn;  // name of oracle column (number) which is used for Z coordinate of point - can be empty for 2D points
	  
    FdoKgKmlPropertyDefinitionCollectionP m_Properties;
};

typedef FdoPtr<FdoKgKmlClassDefinition> FdoKgKmlClassDefinitionP;

#endif // FdoKgOraClassDefinition_H


