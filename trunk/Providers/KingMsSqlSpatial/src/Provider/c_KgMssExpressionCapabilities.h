/*
* Copyright (C) 2007  Haris Kurtagic
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

#ifndef _c_KGORAEXPRESSIONCAPABILITIES_H
#define _c_KGORAEXPRESSIONCAPABILITIES_H


class c_KgMssExpressionCapabilities : public FdoIExpressionCapabilities
{
public:
    c_KgMssExpressionCapabilities ();

protected:
    virtual ~c_KgMssExpressionCapabilities ();

protected:
    virtual void Dispose ();

public:
    /// \brief
    /// Returns an array of FdoExpressionType objects the feature provider supports.
    /// 
    /// \param length 
    /// Input the number of expression types
    /// 
    /// \return
    /// Returns the list of expression types
    /// 
    FDOKGMSS_API virtual FdoExpressionType* GetExpressionTypes(FdoInt32& length);

    /// \brief
    /// Returns a collection of FdoFunctionDefinition objects the feature provider supports within expressions.
    /// 
    /// \return
    /// Returns the collection of function definitions
    /// 
    FDOKGMSS_API virtual FdoFunctionDefinitionCollection* GetFunctions();
};

#endif 

