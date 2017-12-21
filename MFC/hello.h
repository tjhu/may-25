#pragma once
// #define WINVER 0x0502  

#include "micaut.h"

class CMyApp : public CWinApp
{
public:
	CDialog* mydialog;
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL ExitInstance();
};

class CMainWindow : public CFrameWnd
{
public:
	CComPtr<IMathInputControl> g_spMIC; // Math Input Control
	CMainWindow();

protected:
	afx_msg VOID OnPaint();
	DECLARE_MESSAGE_MAP();
};