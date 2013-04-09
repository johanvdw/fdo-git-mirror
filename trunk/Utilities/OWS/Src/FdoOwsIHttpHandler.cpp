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

#include <stdafx.h>
#include <OWS/FdoOwsIHttpHandler.h>
#include "FdoOwsHttpHandler.h"

FdoOwsIHttpHandler* FdoOwsIHttpHandler::Create(const char* url, bool bGet, const char* parameters,
    const char* userName, const char* passwd, const char* proxy_url, const char* proxy_port, const char* proxy_userName, const char* proxy_passwd)
{
    return FdoOwsHttpHandler::Create(url, bGet, parameters, userName, passwd, proxy_url, proxy_port, proxy_userName, proxy_passwd);
}

FdoOwsIHttpHandler* FdoOwsIHttpHandler::Create(const char* url, bool bGet, const char* parameters,
    const char* userName, const char* passwd)
{
    return FdoOwsHttpHandler::Create(url, bGet, parameters, userName, passwd);
}