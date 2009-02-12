
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Fri Mar 05 13:28:56 2004
 */
/* Compiler settings for \transient\xerces-c-src_2_5_0\src\xercesc\com\xml4com.idl:
    Os, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)

#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_Xerces,0xC4775B62,0xECD6,0x11d4,0x94,0xB4,0x00,0xA0,0xCC,0x57,0xCB,0xCB);


MIDL_DEFINE_GUID(IID, IID_IIBMXMLDOMNodeIdentity,0x35ADBB42,0x47B2,0x4b22,0x9D,0x2E,0x1D,0xA2,0x60,0xEE,0x50,0x07);


MIDL_DEFINE_GUID(CLSID, CLSID_DOMDocument,0xC4775B61,0xECD6,0x11d4,0x94,0xB4,0x00,0xA0,0xCC,0x57,0xCB,0xCB);


MIDL_DEFINE_GUID(CLSID, CLSID_XMLHTTPRequest,0x3A725053,0x15FB,0x4065,0x91,0x71,0xBC,0x02,0xBC,0xF3,0x87,0x6C);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

