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

#ifndef CPP_UNIT_XML_FORMATTER
#define CPP_UNIT_XML_FORMATTER

class XmlFormatter 
{
public:
    XmlFormatter(FdoIoStream* inStream, FdoIoStream* outStream, int elementIndent = 4 );
    ~XmlFormatter(void);

    void Format();
private:
    FdoIoStream* mInStream;
    FdoIoStream* mOutStream;
    int mIndent;
    int mLevel;
};

#endif // CPP_UNIT_XML_FORMATTER
