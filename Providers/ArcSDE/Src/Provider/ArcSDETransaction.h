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

#ifndef ARCSDETRANSACTION_H
#define ARCSDETRANSACTION_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDETransaction : public FdoITransaction
{
    friend class ArcSDEConnection;

protected:
    ArcSDEConnection* mConnection; // can't use FdoPtr<> because of circular reference
    bool mSpent; // goes true when commit or rollback done

public:
    ArcSDETransaction ();
    virtual ~ArcSDETransaction(void);

    /// <summary> Gets the FdoIConnection object associated with the transaction.</summary>
    /// <returns>Returns FdoIConnection object</returns>
    FdoIConnection* GetConnection ();

    /// <summary> Commits the transaction.</summary>
    /// <returns>Returns nothing</returns>
    void Commit ();

    /// <summary> Rolls back a transaction from a pending state.</summary>
    /// <returns>Returns nothing</returns>
    void Rollback ();

    /// <summary> Creates a save point in this transaction.</summary>
    /// <param name="suggestName">Suggested save point name.</param> 
    /// <returns>The actual save point name used by the provider </returns>
    FdoString* AddSavePoint(FdoString* suggestName);

    /// <summary> Releases a specific save point.</summary>
    /// <param name="savePointName">Save point name.</param>
    /// <returns>Returns nothing</returns>
    void ReleaseSavePoint(FdoString* savePointName);

    /// <summary> Rolls back a transaction to a specified save point.</summary>
    /// <param name="savePointName">Save point name.</param>
    /// <returns>Returns nothing</returns>
    void Rollback(FdoString* savePointName);

    void Dispose ();

protected:
    void SetConnection (ArcSDEConnection* connection);
    bool IsSpent ();

};

#endif // ARCSDETRANSACTION_H

