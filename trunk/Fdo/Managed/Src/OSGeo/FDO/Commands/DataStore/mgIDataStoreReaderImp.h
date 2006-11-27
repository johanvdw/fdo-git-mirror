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

#pragma once

#include "FDO\Commands\DataStore\mgIDataStoreReader.h"

class FdoIDataStoreReader;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE
public __gc __interface IDataStorePropertyDictionary;

/// \brief
/// The FdoISQLCommand interface defines the SQL command, which supports the
/// execution of a SQL statement against an underlying RDBMS. Two execute
/// methods are provided to distinguish between statements that return table data
/// versus those that execute non query type operations.
private __gc class IDataStoreReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReader
{
public:
    /// \brief
    /// Gets the name of the datastore currently being read.
    /// 
    /// \return
    /// Returns the description of the datastore.
    /// 
	System::String* GetName();

    /// \brief
    /// Gets the description of the datastore currently being read.
    /// 
    /// \return
    /// Returns the description of the datastore.
    /// 
	System::String* GetDescription();

    /// \brief
    /// Gets the flag that indicates whether the datastore is FDO enabled.
    /// Non-FDO databases are also referred to as �Foreign?datastores.
    /// 
    /// \return
    /// Returns the flag
    /// 
	System::Boolean GetIsFdoEnabled();

    /// \brief
    /// Gets the FdoIConnectionPropertyDictionary interface that 
    /// includes the properties of the datastores set at datastore create 
    /// time, with the exception of password.
    /// 
    /// \return
    /// Returns the property dictionary
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary* GetDataStoreProperties();

    /// \brief
    /// Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	System::Boolean ReadNext();

    /// \brief
    /// Closes the FdoIDataStoreReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Close();

public private:
	IDataStoreReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIDataStoreReader* GetImpObj();

protected:
	System::Void ReleaseUnmanagedObject();	
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE


