#pragma once

// Js2nCtrl.h : Declaration of the CJs2nCtrl ActiveX Control class.


// CJs2nCtrl : See Js2nCtrl.cpp for implementation.
#include <ptlib.h>
#include "..\common\nativelogic.h"
#include "queue.h"

class CJs2nCtrl : public COleControl
{
	DECLARE_DYNCREATE(CJs2nCtrl)

friend bool SendEventToJS( const PString& strEvent );
// Constructor
public:
	CJs2nCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnSetClientSite();

// Implementation
protected:
	~CJs2nCtrl();

	DECLARE_OLECREATE_EX(CJs2nCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CJs2nCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CJs2nCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CJs2nCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	afx_msg BSTR Send(LPCTSTR strCommand);
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	void FireOnEvent(LPCTSTR strEventType)
		{FireEvent(eventidJs2nEvent,EVENT_PARAM(VTS_BSTR), strEventType);}
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CMaptiveXCtrl)
	dispidSend = 1L,
	eventidJs2nEvent = 1L,
	//}}AFX_DISP_ID
	};

private:
	CString					m_strURL;
	tNativeLogic			m_NativeLogic;
	Queue<PString>			m_stringQueue;
	CRITICAL_SECTION		m_lock;
private:
	LRESULT OnNativeLogicEvent(WPARAM wParam, LPARAM lParam);
};
