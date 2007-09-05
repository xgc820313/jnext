// Js2n.cpp : Implementation of CJs2nApp and DLL registration.

#include "stdafx.h"
#include "Js2n.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CJs2nApp NEAR theApp;

// {7E829C5E-6093-4e3d-9622-7BC829D3EB65}
const GUID CDECL BASED_CODE _tlid =
	{ 0x7e829c5e, 0x6093, 0x4e3d, { 0x96, 0x22, 0x7b, 0xc8, 0x29, 0xd3, 0xeb, 0x65 } };

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CJs2nApp::InitInstance - DLL initialization

BOOL CJs2nApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CJs2nApp::ExitInstance - DLL termination

int CJs2nApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
