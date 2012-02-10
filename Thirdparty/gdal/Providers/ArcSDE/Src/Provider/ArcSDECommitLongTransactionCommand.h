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
 */

#ifndef ArcSDECommitLongTransactionCommand_H
#define ArcSDECommitLongTransactionCommand_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDECommitLongTransactionCommand :
    public ArcSDEFeatureCommand<FdoICommitLongTransaction>
{
    // name of the long transaction
    FdoStringP mName;

    // conflict dispositions
    FdoPtr <ArcSDELongTransactionConflictDirectiveEnumerator> mDispositions;

protected:
    void checkConflict (
        SE_CONNECTION connection,
        CHAR* table,
        CHAR* rowid,
        LONG id,
        LONG diff,
        LONG type,
        ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList* list);
    void checkTable (
        SE_REGINFO table,
        LONG parent,
        LONG child);
    void copyRows (
        SE_CONNECTION connection,
        CHAR* table,
        CHAR* rowid,
        LONG target,
        LONG from,
        LONG source,
        LONG diff,
        LONG type,
        ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList* list);
    void deleteRows (
        SE_CONNECTION connection,
        CHAR* table,
        CHAR* rowid,
        LONG target,
        LONG source,
        LONG diff,
        LONG type,
        ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList* list);
    void commitTable (
        SE_REGINFO table,
        LONG target,
        LONG source,
        LONG diff);

	FdoBoolean m_bKeepLongTransaction;

public:
    ArcSDECommitLongTransactionCommand (FdoIConnection *connection);
    virtual ~ArcSDECommitLongTransactionCommand (void);

    /// <summary>Gets the name of the long transaction to commit as a string.</summary>
    /// <returns>Returns the name of the long transaction</returns> 
    FdoString* GetName ();

    /// <summary>Sets the name of the long transaction to commit as a string.</summary>
    /// <param name="value">Input the name of the long transaction</param> 
    /// <returns>Returns nothing</returns> 
    void SetName (FdoString* value);

    /// <summary>Gets a reference to the lock conflict reader.</summary>
    /// <returns>Returns a reference to the lock conflict reader</returns> 
    FdoILockConflictReader* GetLockConflictReader();

    // Executes the commit long transaction command and returns an
    // FdoILongTransactionConflictDirectiveEnumerator.
    // If the returned enumerator's count is 0 then
    // no conflicts were detected and the command was executed successfully.
    // Otherwise conflicts were detected and the user must specify how to
    // resolve them using the enumerator and call Execute again.
    FdoILongTransactionConflictDirectiveEnumerator* Execute ();

    /// \brief
    /// Indicates whether to keep the long transaction after it is committed.
    /// Returns true if keeping the long transaction after it is committed.
    /// Returns false if removing the long transaction after it is committed.
    /// Default value for KeepLongTransaction is false.
    /// 
    /// \return
    /// Returns true if keeping the long transaction after it is committed.
    /// 
	FdoBoolean GetKeepLongTransaction () { return (m_bKeepLongTransaction); }

    /// \brief
    /// Sets whether to keep the long transaction after it is committed.
    /// Default value for KeepLongTransaction is false.
    ///
    /// \param value 
    /// Inputs one boolean value to indicate whether to keep the committed
    /// long transaction
    /// 
    /// \return
    /// Returns nothing
    /// 
    void SetKeepLongTransaction (FdoBoolean value) { m_bKeepLongTransaction = value; }
};

#endif // ArcSDECommitLongTransactionCommand_H

