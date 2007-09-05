// Js2nPropPage.cpp : Implementation of the CJs2nPropPage property page class.

#include "stdafx.h"
#include "Js2n.h"
#include "Js2nPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CJs2nPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CJs2nPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CJs2nPropPage, "Js2n.Js2nPropPage.1",
	0xe8a14b47, 0xb8b6, 0x409e, 0x9d, 0x97, 0xfb, 0x5e, 0xde, 0xfe, 0xcb, 0xc6)



// CJs2nPropPage::CJs2nPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CJs2nPropPage

BOOL CJs2nPropPage::CJs2nPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_Js2n_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CJs2nPropPage::CJs2nPropPage - Constructor

CJs2nPropPage::CJs2nPropPage() :
	COlePropertyPage(IDD, IDS_Js2n_PPG_CAPTION)
{
}



// CJs2nPropPage::DoDataExchange - Moves data between page and properties

void CJs2nPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CJs2nPropPage message handlers
