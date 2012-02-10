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

#include "Pch.h"
#include "XmlFormatter.h"

XmlFormatter::XmlFormatter(FdoIoStream* inStream, FdoIoStream* outStream, FdoInt32 elementIndent)
{
    mInStream = inStream;
    mOutStream = outStream;
    mIndent = elementIndent;
    mLevel = 0;
}

XmlFormatter::~XmlFormatter(void)
{
}

void XmlFormatter::Format()
{
    FdoByte buffer;
    FdoByte cr = '\n';
    FdoByte cr2 = '\r';
    FdoByte cr3 = '\t';

    while ( mInStream->Read( &buffer, 1 ) > 0 ) {
        if ( buffer != cr && buffer != cr2 && buffer != cr3)
            mOutStream->Write( &buffer, 1 );
    }
}
