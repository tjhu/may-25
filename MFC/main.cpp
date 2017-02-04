#include <afxwin.h>
#include <afxwinappex.h>
#include "hello.h"
#include "dxapp.h"
#include "mathinput.h"
#include "micaut_i.c"

CMyApp myApp;


BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();


	HWND hWnd = m_pMainWnd->GetSafeHwnd();
	InitDevice(hWnd);
	return TRUE;
}

BOOL CMyApp::OnIdle(LONG lCount)
{
	// CWinApp::OnIdle(lCount);
	AfxGetMainWnd()->Invalidate(false);
	Render();
	return TRUE;
}

BOOL CMyApp::ExitInstance()
{
	CleanupDevice();
	return 0;
}

BEGIN_MESSAGE_MAP(CMainWindow, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

CMainWindow::CMainWindow()
{
	Create(NULL, _T("Hi Ms. Tanimasa"));

	HRESULT hr = CoInitialize(NULL);
	hr = g_spMIC.CoCreateInstance(CLSID_MathInputControl);
	hr = g_spMIC->Show();
}

void CMainWindow::OnPaint()
{
	CPaintDC dc(this);
	
	CRect rect;
	GetClientRect(&rect);

}