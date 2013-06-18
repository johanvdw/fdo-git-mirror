#ifndef _SdfBinaryReader_H
#define _SdfBinaryReader_H
// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifdef _WIN32
#include <hash_map>
#else
#include <list>
#include <ext/hash_map>
namespace stdext = ::__gnu_cxx;
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
using namespace std;
#endif

//the initial string buffer is created with this (smallish) size
//if we need bigger, either a new buffer will be allocated or a 
//the bigger size will be used if it is the first buffer we
//create
#define SDF_STRING_CACHE_SIZE 256

#define SDF_STRING_CACHE_COUNT 10

class SdfBinaryReader
{
public:

    SdfBinaryReader(unsigned char* data, int len );
    SdfBinaryReader(unsigned char* data, int len, int propCount );
	SdfBinaryReader();
    virtual ~SdfBinaryReader();
    void Init( int propCount );
    void Reset(unsigned char* data, int len);
    void SetPosition(int offset);
    int GetPosition();
    unsigned char* GetDataAtCurrentPosition();
    unsigned GetDataLen();
    
    double ReadDouble();
    float ReadSingle();
    short ReadInt16();
    unsigned short ReadUInt16();
    int ReadInt32();
    unsigned ReadUInt32();
    FdoInt64 ReadInt64();
    unsigned char ReadByte();
    char ReadChar();
    const wchar_t* ReadString(); 
    const wchar_t* ReadRawString(unsigned mbstrlen);
    const wchar_t* ReadRawString(unsigned mbstrlen, int index);
	const wchar_t* ReadRawStringNoCache(unsigned mbstrlen);
    FdoDateTime ReadDateTime();
    FdoDataValue* ReadDataValue();
    //add more as needed...

private:

    unsigned char* m_data;
    unsigned m_len;
    unsigned m_pos;

    //current unicode string buffer
    wchar_t* m_wcsCache;
    unsigned m_wcsCacheCurrent;
    unsigned m_wcsCacheLen; 

	//since we cannot invalidate pointers to strings we have returned
    //we need to keep all previous caches valid until we are reset or
    //destroyed.
    typedef struct _wcsStringCache {
        wchar_t* wcsString;
        unsigned int wcsLen;
    } SdfStringCacheDef;
	
    SdfStringCacheDef  *m_wcsStringCache;
    unsigned int wcsCacheLen;
    unsigned int wcsCacheLastIndex;
};

#endif

