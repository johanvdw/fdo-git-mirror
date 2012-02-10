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

#include "Runtime\Disposable.h"

class FdoIoTextWriter;

BEGIN_NAMESPACE_OSGEO_COMMON_IO
ref class IoStream;

/// \ingroup (OSGeoFDOCommonIo)
/// \brief
///     IoTextWriter writes Unicode wide character text to a binary stream.
///     The text is written in UTF8 format.
public ref class IoTextWriter sealed : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    ///     Constructs a text writer on a file.
    /// 
    /// \param fileName 
    ///     Input name of the file to write to. The file is created
    ///     if it does not yet exist. If it already exists, its previous contents are 
    ///     completely overwritten.
    /// 
    /// \return
    ///     Returns IoTextWriter
    /// 
    ///  
	IoTextWriter(System::String^ fileName);

    /// \brief
    ///     Constructs a text writer on a stream.
    /// 
    /// \param stream 
    ///     Input the stream to write to. Writing starts at the 
    ///     stream's current position.
    /// 
    /// \return
    ///     Returns IoTextWriter
    /// 
    ///  
	IoTextWriter(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream);

    /// \brief
    ///     Gets the underlying stream. If a stream was passed to this object
    ///     then this stream is returned.
    ///     Otherwise, an auto-generated stream (wrapped around the  file name 
    ///     that was passed to this object) is returned.
    /// 
    /// \return
    ///     Returns the underlying stream.
    /// 
    ///  
    property NAMESPACE_OSGEO_COMMON_IO::IoStream^ Stream
    {
        NAMESPACE_OSGEO_COMMON_IO::IoStream^ get();
    }

    /// \brief
    ///     Writes to this text writer. The text is written in UTF8 format.
    /// 
    /// \param data 
    ///     The text to write.
    /// 
	System::Void Write(System::String^ data);

    /// \brief
    ///     Same as Write(), except that a carriage return (n) is appended to
    ///     the text.
    /// 
    /// \param data 
    ///     The text to write.
    /// 
	System::Void WriteLine(System::String^ data);

    /// \brief
    ///     Constructs a text writer based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged writer.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed object should be automatically deleted 
    ///     once it no longer referenced.
    /// 
	IoTextWriter(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	inline FdoIoTextWriter* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_IO


