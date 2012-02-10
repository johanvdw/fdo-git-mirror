#ifndef FDOSMPHODBCDBCOLUMN_H
#define FDOSMPHODBCDBCOLUMN_H        1
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

#include <Sm/Ph/Column.h>
#include <Sm/Ph/Rd/ColumnReader.h>

// Odbc Provider implementation of a Column object.
class FdoSmPhOdbcColumn : virtual public FdoSmPhColumn
{
public:

    void SetComputedExpression(FdoString* value) { m_computedExpression = value; };
    FdoString* GetComputedExpression(void) { return m_computedExpression; };

protected:
    FdoSmPhOdbcColumn(FdoSmPhRdColumnReader* reader);
    virtual ~FdoSmPhOdbcColumn(void);

    virtual FdoStringP GetAddSql();

    virtual FdoStringP GetAutoincrementSql();

protected:
    // NOTE: The following constructor is to satisfy the compiler, and should never actually be called:
    FdoSmPhOdbcColumn() {}

    FdoStringP m_computedExpression;
};

typedef FdoPtr<FdoSmPhOdbcColumn> FdoSmPhOdbcColumnP;

#endif
