#pragma once
#include <atlbase.h>
#include <atlwin.h>

// include for MIC
#include "micaut.h"

// include for event sinks
#include "mathinput.h"

// dialog dialog
class dialog;

const _ATL_FUNC_INFO CMathInputControlEventHandler<dialog>::OnMICInsertInfo = { CC_STDCALL, VT_I4, 1,{ VT_BSTR } };
const _ATL_FUNC_INFO CMathInputControlEventHandler<dialog>::OnMICCloseInfo = { CC_STDCALL, VT_I4, 0,{ VT_EMPTY } };

// CMIC_TEST_EVENTSDlg dialog
class dialog : public CDialog,
	public CMathInputControlEventHandler<dialog>
{
public:
	CComPtr<IMathInputControl> m_spMIC; // Math Input Control
	DECLARE_DYNAMIC(dialog)
	
	dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~dialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
