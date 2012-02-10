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

#include "FDO\Commands\mgICommand.h"
#include "FDO\Commands\LongTransaction\mgLongTransactionFreezeOperations.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

/// \ingroup (OSGeoFDOCommandsLongTransaction)
/// \interface OSGeo::FDO::Commands::LongTransaction::IFreezeLongTransaction
/// \brief
/// The IFreezeLongTransaction interface defines the FreezeLongTransaction
/// command, which freezes or thaws a long transaction. When a long transaction
/// is frozen the user who froze it gains exclusive access to it. Other users can
/// read it, but not apply any updates.
/// Input to the FreezeLongTransaction command is the long transaction name and
/// the operation to perform. The Execute method performs the specified
/// operation on the identified long transaction if the user has access
/// privileges for the indicated long transaction.
public interface class IFreezeLongTransaction : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the long transaction to freeze or unfreeze as a string.
    /// 
    /// \return
    /// Returns the name of the long transaction
    /// 
    /// \brief
    /// Sets the name of the long transaction to freeze or unfreeze as a string.
    /// 
    /// \param value 
    /// Input the name of the long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::String^ Name
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    ///  Gets the operation to perform (Freeze or Thaw) on the long transaction.
    /// 
    /// \return
    /// Returns the operation to perform
    /// 
    /// \brief
    /// Sets the operation to perform (Freeze or Thaw) on the long transaction.
    /// 
    /// \param value 
    /// Input the operation to perform
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionFreezeOperations Operation
    {
        NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionFreezeOperations get();
        System::Void set(NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::LongTransactionFreezeOperations value);
    }

    /// \brief
    /// Executes the command.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION


