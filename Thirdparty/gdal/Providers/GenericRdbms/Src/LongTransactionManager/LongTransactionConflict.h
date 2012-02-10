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

// =============================================================================
// | The file contains the definition of the class FdoRdbmsLongTransactionCon-
// | flict. The class defines the structure used to store information associated
// | with a long transaction conflict report for a single table.
// =============================================================================


#ifndef FdoRdbmsLongTransactionConflict_h
#define FdoRdbmsLongTransactionConflict_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include "Disposable.h"
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "LongTransactionConflictInfos.h"

class FdoRdbmsLongTransactionConflict: public FdoLtmDisposable
{

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  GetConflicts:
    //      The function returns a handle to the long transaction conflict info
    //      collection for the current object of this class.

    FdoRdbmsLongTransactionConflictInfos *GetConflicts ();

    //  GetDataStore:
    //      The function returns the data store name for the current object of
    //      this class.

    FdoString *GetDataStore ();

    //  GetTableName:
    //      The function returns the table name for the current object of this
    //      class.

    FdoString *GetTableName ();

    //  SetConflicts:
    //      The function sets the reference to the long transaction conflict
    //      info collection for the current object of this class.

    void SetConflicts (FdoRdbmsLongTransactionConflictInfos *root);

    //  SetDataStore:
    //      The function sets the data store name for the current object of
    //      this class.

    void SetDataStore (FdoString *value);

    //  SetTableName:
    //      The function sets the table name for the current object of this
    //      class.

    void SetTableName (FdoString *value);


    protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  Dispose:
    //      Function to delete an object of this class.

    virtual void Dispose () { delete this; }

    //  FdoRdbmsLongTransactionConflict:
    //      The class constructor.

    FdoRdbmsLongTransactionConflict ();

    //  ~FdoRdbmsLongTransactionConflict:
    //      The class destructor.

    virtual ~FdoRdbmsLongTransactionConflict ();


    private:

    // ************************************************************************
    // *                          Private Interfaces                          *
    // ************************************************************************

    //  ClearMemory:
    //      The function releases memory allocated for the class variables.

    void ClearMemory ();

    //  SetToZero:
    //      The function initializes the class variables.

    void SetToZero ();

    //  SetValue:
    //      The function allocates memory for a copy of the given value.
 
    wchar_t *SetValue (FdoString *value_buffer);


    // ************************************************************************
    // *                        Private Member Variables                      *
    // ************************************************************************

    //  conflicts:
    //      The variable keeps a reference to the collection of long transac-
    //      tion conflict information that exist for the current table.

    FdoRdbmsLongTransactionConflictInfos *conflicts;

    //  data_store:
    //      The variable holds the data store name that hosts the table for
    //      which long transaction conflicts are reported.

    FdoString *data_store;

    //  table_name:
    //      The variable holds the table name for which long transaction con-
    //      flicts are reported.

    FdoString *table_name;


};  //  class FdoRdbmsLongTransactionConflict

#endif

