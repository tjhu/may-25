// dialog.cpp : implementation file
//

#include "stdafx.h"
#include "dialog.h"
#include "afxdialogex.h"

// includes for implementation
#include "micaut.h"
#include "micaut_i.c"

// include for event handler
#include "mathinput.h"

CComPtr<IMathInputControl> g_spMIC; // Math Input Control

// dialog dialog

IMPLEMENT_DYNAMIC(dialog, CDialog)

dialog::dialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
{
	// TODO: Add extra initialization here
	CoInitialize(NULL);
	HRESULT hr = g_spMIC.CoCreateInstance(CLSID_MathInputControl);
	if (SUCCEEDED(hr)) {
		hr = CMathInputControlEventHandler<dialog>::Initialize(m_spMIC, this);
		if (SUCCEEDED(hr)) {
			hr = CMathInputControlEventHandler<dialog>::DispEventAdvise(m_spMIC);
			if (SUCCEEDED(hr)) {
				hr = m_spMIC->Show();
			}
		}
	}
}


dialog::~dialog()
{
}

void dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(dialog, CDialog)
END_MESSAGE_MAP()


// dialog message handlers
