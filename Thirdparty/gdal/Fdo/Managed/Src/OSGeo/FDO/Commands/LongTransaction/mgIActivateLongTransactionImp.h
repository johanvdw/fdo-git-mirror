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

#include "FDO\Commands\LongTransaction\mgIActivateLongTransaction.h"
#include "FDO\Commands\mgICommandImp.h"

class FdoIActivateLongTransaction;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \ingroup (OSGeoFDOCommandsLongTransaction)
/// \brief
/// The IActivateLongTransactionImp class is a concrete implementation of IActivateLongTransaction.
/// The IActivateLongTransaction interface defines the ActivateLongTransaction
/// command, which activates a long transaction where feature manipulation and
/// locking commands operate on it. Input to the activate
/// long transaction command is the long transaction name. The Execute operation
/// activates the identified long transaction if the user has access privileges
/// for it.
private ref class IActivateLongTransactionImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                                 public NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IActivateLongTransaction
{
public:
    /// \brief
    /// Gets the name of the long transaction to activate as a string.
    /// 
    /// \return
    /// Returns the name of the long transaction
    /// 
    /// \brief
    /// Sets the name of the long transaction to activate as a string.
    /// 
    /// \param value 
    /// Input the name of the long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::String^ Name
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Executes the activate long transaction command. An exception is
    /// thrown if the long transaction does not exist or the user does not
    /// have access privileges to the long transaction.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual System::Void Execute();

internal:
	IActivateLongTransactionImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIActivateLongTransaction* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


