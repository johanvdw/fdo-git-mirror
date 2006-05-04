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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING

/// \brief
/// The FdoLockType enumeration defines lock types. The lock type for 
/// a locked object indicates a user's access privileges (including the user 
/// who applied the persistent lock) to an object.
/// <p>
/// The enumeration values are used with the commands FdoIAcquireLock, FdoISelect 
/// and FdoIGetLockInfo. With the command FdoIAcquireLock or FdoISelect, the 
/// specified value indicates the lock type to be used when applying a lock on
/// an object. With the command FdoIGetLockInfo, the enumaration values are used
/// to report lock types back to the user.
/// <p>
/// The enumeration contains some special lock types (FdoLockType_None and FdoLockType_Unsupported) 
/// that are used only when reporting lock type information.
/// These special lock types cannot be used to specify a lock type to be applied 
/// to an object.
/// <param name="FdoLockType_None">
/// Indicates that an object is unlocked. 
/// This type cannot be used with the commands FdoIAcquireLock and FdoISelect.
/// </param>
/// <param name="FdoLockType_Unsupported">
/// Indicates that an unsupported lock 
/// type has been encountered. This type cannot be used with the commands 
/// FdoIAcquireLock and FdoISelect.
/// </param>
/// <param name="FdoLockType_Shared">
/// Indicates a shared lock type. The type can 
/// be used with the commands FdoIAcquireLock and FdoISelect.
/// </param>
/// <param name="FdoLockType_Transaction">
/// Indicates that a transaction lock 
/// should be applied on an object. This type can be used with the commands 
/// FdoIAcquireLock and FdoISelect.
/// </param>
/// <param name="FdoLockType_Exclusive">
/// Indicates that only this user can modify 
/// this object. In a long transaction situation, any user can modify the object in any 
/// other long transaction, including the root long transaction if it is not the current 
/// long transaction. 
/// <P>
/// The type can be used with the commands FdoIAcquireLock and FdoISelect.
/// </param>
/// <param name="FdoLockType_LongTransactionExclusive">
/// Indicates that only this user can 
/// modify this object in the long transaction containing the object or any long 
/// transaction created as a descendent of that one. When not in a long transaction 
/// situation (for example, if only a root long transaction exists), the lock behaves 
/// like an Exclusive lock. 
/// <P>
/// The type can be used with the commands FdoIAcquireLock and FdoISelect.
/// </param>
/// <param name="FdoLockType_AllLongTransactionExclusive">
/// Indicates that only this user can 
/// modify this object in this long transaction. No user, not even the user locking the object, 
/// can modify the object in any long transaction created as a descendent of the one 
/// containing the object being locked. When not in a long transaction situation (for example, 
/// if only a root long transaction exists), the lock behaves like an Exclusive lock. 
/// <P>
/// This type can be used with the commands FdoIAcquireLock and FdoISelect.
/// </param>

public __value enum LockStrategy
{
	LockStrategy_All = FdoLockStrategy_All,
	LockStrategy_Partial = FdoLockStrategy_Partial
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING


