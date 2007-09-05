

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Sep 04 22:28:30 2007
 */
/* Compiler settings for .\Js2n.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


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

MIDL_DEFINE_GUID(IID, LIBID_Js2nLib,0x7E829C5E,0x6093,0x4e3d,0x96,0x22,0x7B,0xC8,0x29,0xD3,0xEB,0x65);


MIDL_DEFINE_GUID(IID, DIID__DJs2n,0xDB94C377,0x4B82,0x4cac,0xB5,0xC6,0xB9,0x27,0xAB,0xCE,0x6E,0x52);


MIDL_DEFINE_GUID(IID, DIID__DJs2nEvents,0x6FCA21FB,0x78E6,0x4825,0x83,0x19,0x5C,0x82,0x85,0xAE,0x08,0xCA);


MIDL_DEFINE_GUID(CLSID, CLSID_Js2n,0xC802F39D,0xBF85,0x427a,0xA3,0x34,0x77,0xE5,0x01,0xDB,0x62,0xE9);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



