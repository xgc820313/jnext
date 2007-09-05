

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __Js2nidl_h__
#define __Js2nidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DJs2n_FWD_DEFINED__
#define ___DJs2n_FWD_DEFINED__
typedef interface _DJs2n _DJs2n;
#endif 	/* ___DJs2n_FWD_DEFINED__ */


#ifndef ___DJs2nEvents_FWD_DEFINED__
#define ___DJs2nEvents_FWD_DEFINED__
typedef interface _DJs2nEvents _DJs2nEvents;
#endif 	/* ___DJs2nEvents_FWD_DEFINED__ */


#ifndef __Js2n_FWD_DEFINED__
#define __Js2n_FWD_DEFINED__

#ifdef __cplusplus
typedef class Js2n Js2n;
#else
typedef struct Js2n Js2n;
#endif /* __cplusplus */

#endif 	/* __Js2n_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __Js2nLib_LIBRARY_DEFINED__
#define __Js2nLib_LIBRARY_DEFINED__

/* library Js2nLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_Js2nLib;

#ifndef ___DJs2n_DISPINTERFACE_DEFINED__
#define ___DJs2n_DISPINTERFACE_DEFINED__

/* dispinterface _DJs2n */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DJs2n;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DB94C377-4B82-4cac-B5C6-B927ABCE6E52")
    _DJs2n : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DJs2nVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DJs2n * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DJs2n * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DJs2n * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DJs2n * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DJs2n * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DJs2n * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DJs2n * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DJs2nVtbl;

    interface _DJs2n
    {
        CONST_VTBL struct _DJs2nVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DJs2n_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DJs2n_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DJs2n_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DJs2n_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DJs2n_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DJs2n_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DJs2n_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DJs2n_DISPINTERFACE_DEFINED__ */


#ifndef ___DJs2nEvents_DISPINTERFACE_DEFINED__
#define ___DJs2nEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DJs2nEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DJs2nEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6FCA21FB-78E6-4825-8319-5C8285AE08CA")
    _DJs2nEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DJs2nEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DJs2nEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DJs2nEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DJs2nEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DJs2nEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DJs2nEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DJs2nEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DJs2nEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DJs2nEventsVtbl;

    interface _DJs2nEvents
    {
        CONST_VTBL struct _DJs2nEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DJs2nEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DJs2nEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DJs2nEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DJs2nEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DJs2nEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DJs2nEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DJs2nEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DJs2nEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Js2n;

#ifdef __cplusplus

class DECLSPEC_UUID("C802F39D-BF85-427a-A334-77E501DB62E9")
Js2n;
#endif
#endif /* __Js2nLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


