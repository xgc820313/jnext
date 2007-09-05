#pragma once

// Js2nPropPage.h : Declaration of the CJs2nPropPage property page class.


// CJs2nPropPage : See Js2nPropPage.cpp for implementation.

class CJs2nPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CJs2nPropPage)
	DECLARE_OLECREATE_EX(CJs2nPropPage)

// Constructor
public:
	CJs2nPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_Js2n };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

