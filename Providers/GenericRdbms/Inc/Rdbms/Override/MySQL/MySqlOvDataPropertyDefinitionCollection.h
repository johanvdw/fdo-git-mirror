#ifndef FDO_MySQLOVDATAPROPERTYDEFINITIONCOLLECTION_H
#define FDO_MySQLOVDATAPROPERTYDEFINITIONCOLLECTION_H

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
#include <Fdo/Schema/SchemaException.h>
#include <Fdo/Commands/Schema/PhysicalElementMappingCollection.h>
#include <Rdbms/Override/MySQL/MySqlOv.h>
#include <Rdbms/Override/MySQL/MySqlOvDataPropertyDefinition.h>

/// \brief
/// Concrete class defining physical schema overrides for a collection of
/// data properties.
class FdoMySQLOvDataPropertyDefinitionCollection : public FdoPhysicalElementMappingCollection<FdoMySQLOvDataPropertyDefinition>
{
public:
    /// \brief
    /// Constructs a default of an FdoMySQLOvDataPropertyDefinitionCollection
    /// 
    /// \return
    /// Returns FdoMySQLOvDataPropertyDefinitionCollection
    /// 
    FDOMYSQL_OV_API static FdoMySQLOvDataPropertyDefinitionCollection* Create();

    /// \brief
    /// Constructs an instance of an FdoMySQLOvDataPropertyDefinitionCollection using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoMySQLOvDataPropertyDefinitionCollection
    /// 
    FDOMYSQL_OV_API static FdoMySQLOvDataPropertyDefinitionCollection* Create(FdoPhysicalElementMapping* parent);

protected:
    FdoMySQLOvDataPropertyDefinitionCollection();
    FdoMySQLOvDataPropertyDefinitionCollection(FdoPhysicalElementMapping* parent);
    virtual ~FdoMySQLOvDataPropertyDefinitionCollection();
    virtual void Dispose();
};

#endif


