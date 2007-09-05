// Js2nCtrl.cpp : Implementation of the CJs2nCtrl ActiveX Control class.

#include "stdafx.h"
#include "Js2n.h"
#include "Js2nCtrl.h"
#include "Js2nPropPage.h"
#include "helpers.h"
#include "registry.h"
#include "queue.h"

#include <comcat.h>
#include <objsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CJs2nCtrl, COleControl)

#define WM_NLOGICEVENT WM_USER+101

// Message map

BEGIN_MESSAGE_MAP(CJs2nCtrl, COleControl)
	ON_MESSAGE(WM_NLOGICEVENT,OnNativeLogicEvent)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


// Dispatch map

BEGIN_DISPATCH_MAP(CJs2nCtrl, COleControl)
	DISP_FUNCTION(CJs2nCtrl, "sendCmd", Send, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CJs2nCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


// Event map

BEGIN_EVENT_MAP(CJs2nCtrl, COleControl)
	EVENT_CUSTOM("Js2nEvent", FireOnEvent, VTS_BSTR)
END_EVENT_MAP()


// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CJs2nCtrl, 1)
	PROPPAGEID(CJs2nPropPage::guid)
END_PROPPAGEIDS(CJs2nCtrl)



// Initialize class factory and guid
// {C802F39D-BF85-427a-A334-77E501DB62E9}

IMPLEMENT_OLECREATE_EX(CJs2nCtrl, "Js2n.Js2nCtrl.1",
	0xc802f39d, 0xbf85, 0x427a, 0xa3, 0x34, 0x77, 0xe5, 0x1, 0xdb, 0x62, 0xe9);

// Type library ID and version

IMPLEMENT_OLETYPELIB(CJs2nCtrl, _tlid, _wVerMajor, _wVerMinor)


// Interface IDs

// {DB94C377-4B82-4cac-B5C6-B927ABCE6E52}
const IID BASED_CODE IID_DJs2n =
		{ 0xdb94c377, 0x4b82, 0x4cac, { 0xb5, 0xc6, 0xb9, 0x27, 0xab, 0xce, 0x6e, 0x52 } };

// {6FCA21FB-78E6-4825-8319-5C8285AE08CA}
const IID BASED_CODE IID_DJs2nEvents =
		{ 0x6fca21fb, 0x78e6, 0x4825, { 0x83, 0x19, 0x5c, 0x82, 0x85, 0xae, 0x8, 0xca } };



// Control type information

static const DWORD BASED_CODE _dwJs2nOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CJs2nCtrl, IDS_Js2n, _dwJs2nOleMisc)

PString FindPluginsPath( void )
{
	CRegString regStr( "CLSID\\{C802F39D-BF85-427A-A334-77E501DB62E9}\\InprocServer32\\", "", FALSE, HKEY_CLASSES_ROOT );

	string strRead			= regStr.read();
	PString strPluginPath	= strRead;
	PINDEX nPos				= strPluginPath.FindLast( "\\" );
	strPluginPath = strPluginPath.Mid( 0, nPos + 1 ) + "js2n\\";

	return strPluginPath;
}

// CJs2nCtrl::CJs2nCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CJs2nCtrl

BOOL CJs2nCtrl::CJs2nCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister) {
		BOOL retval = AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_Js2n,
			IDB_Js2n,
			afxRegApartmentThreading,
			_dwJs2nOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);

		// mark as safe for scripting--failure OK
		HRESULT hr = CreateComponentCategory(CATID_SafeForScripting, 
			L"Controls that are safely scriptable");

		if (SUCCEEDED(hr))
			// only register if category exists
			RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);
			// don't care if this call fails

		// mark as safe for data initialization
		hr = CreateComponentCategory(CATID_SafeForInitializing, 
			L"Controls safely initializable from persistent data");

		if (SUCCEEDED(hr))
			// only register if category exists
			RegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing);
			// don't care if this call fails

		return retval;
	}
	else
		// since unregistering the class automatically eliminates 
		// the category registration and since we don't want to 
		// remove the categories, we don't need to do anything special

		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);

}



CJs2nCtrl* g_pCJs2nCtrl = NULL;

// CJs2nCtrl::CJs2nCtrl - Constructor
CJs2nCtrl::CJs2nCtrl()
{
	InitializeIIDs(&IID_DJs2n, &IID_DJs2nEvents);
	// TODO: Initialize your control's instance data here.
	g_pCJs2nCtrl = this;
}


// CJs2nCtrl::~CJs2nCtrl - Destructor

CJs2nCtrl::~CJs2nCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


LRESULT CJs2nCtrl::OnNativeLogicEvent(WPARAM wParam, LPARAM lParam)
{
	if ( !m_stringQueue.IsEmpty() )
	{
		PString strEvent = m_stringQueue.RemoveHead();
		FireOnEvent( strEvent );
	}
	return 0;
}

// CJs2nCtrl::OnDraw - Drawing function

void CJs2nCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CJs2nCtrl::DoPropExchange - Persistence support

void CJs2nCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CJs2nCtrl::OnResetState - Reset control to default state

void CJs2nCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CJs2nCtrl::AboutBox - Display an "About" box to the user

void CJs2nCtrl::AboutBox()
{
	//CDialog dlgAbout(IDD_ABOUTBOX_Js2n);
	//dlgAbout.DoModal();
}

void CJs2nCtrl::OnSetClientSite() 
{
	// TODO: Add your specialized code here and/or call the base class
	LPOLECLIENTSITE pClientSite = GetClientSite();

	if (pClientSite != NULL)
	{
		// Obtain URL from container moniker.
		IMoniker*	ppmk;
		LPOLESTR	pszDisplayName;

		if (SUCCEEDED(pClientSite->GetMoniker( OLEGETMONIKER_TEMPFORUSER,
											   OLEWHICHMK_CONTAINER,
											   &ppmk)))
		{
			if (SUCCEEDED(ppmk->GetDisplayName(NULL, NULL, &pszDisplayName)))
			{
				PString strURL( (WORD *)pszDisplayName );
				PString strPluginsPath = FindPluginsPath();
				m_NativeLogic.Init( strURL, strPluginsPath );
				CoTaskMemFree((LPVOID)pszDisplayName);
			}
		}
	}

	COleControl::OnSetClientSite();
}

// CJs2nCtrl message handlers
BSTR CJs2nCtrl::Send(LPCTSTR szCommand) 
{
	string strCmd		= szCommand;
	string strResult	= m_NativeLogic.InvokeFunction( strCmd );
	CString strRet		= strResult.c_str();
	return strRet.AllocSysString();
}

//___________________________________________________________________________

bool SendEventToJS( const PString& strEvent )
{
	if ( g_pCJs2nCtrl == NULL )
	{
		return false;
	}

	g_pCJs2nCtrl->m_stringQueue.AddTail( strEvent ); // replace this with queue;
	SendMessage( g_pCJs2nCtrl->m_hWnd, WM_NLOGICEVENT, 0, 0 );

	//LPDISPATCH pDisp = g_pCjs2nCtrl->GetIDispatch( FALSE );
	/*
	LPDISPATCH pDisp;
	g_pCjs2nCtrl->get_accParent( &pDisp );
	CWebPage page;
	page.SetDocument( pDisp );
	string strEvent1 = strEvent;
	page.CallJScript( "callback", strEvent1.c_str() );
	*/
	return true;
}
