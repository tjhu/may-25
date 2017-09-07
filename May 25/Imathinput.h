#pragma once

#include "micaut.h"
#include <atlwin.h>
// Reference: https://msdn.microsoft.com/en-us/library/windows/desktop/dd371425(v=vs.85).aspx

static const int MATHINPUTCONTROL_SINK_ID = 1;

template <class T>
class ATL_NO_VTABLE CMathInputControlEventHandler :
	public IDispEventSimpleImpl <MATHINPUTCONTROL_SINK_ID, CMathInputControlEventHandler<T>, &__uuidof(_IMathInputControlEvents)>
{
private:
	IUnknown	*m_pUnknown;
	ULONG m_ulAdviseCount;

public:
	static const _ATL_FUNC_INFO OnMICInsertInfo; // = {CC_STDCALL, VT_I4, 1, {VT_BSTR}};
	static const _ATL_FUNC_INFO OnMICCloseInfo;  // = {CC_STDCALL, VT_I4, 0, {VT_EMPTY}};

	BEGIN_SINK_MAP(CMathInputControlEventHandler)
		SINK_ENTRY_INFO(MATHINPUTCONTROL_SINK_ID, __uuidof(_IMathInputControlEvents), DISPID_MICInsert, OnMICInsert, const_cast<_ATL_FUNC_INFO*>(&OnMICInsertInfo))
		SINK_ENTRY_INFO(MATHINPUTCONTROL_SINK_ID, __uuidof(_IMathInputControlEvents), DISPID_MICClose, OnMICClose, const_cast<_ATL_FUNC_INFO*>(&OnMICCloseInfo))
	END_SINK_MAP()

	HRESULT Initialize(IUnknown *pUnknown)
	{
		m_pUnknown = pUnknown;
		m_ulAdviseCount = 0;
		return S_OK;
	}

	// Methods
	HRESULT __stdcall OnMICInsert(BSTR bstrRecoResult)
	{
		CComQIPtr<IMathInputControl> spMIC(m_pUnknown);
		HRESULT hr = S_OK;
		if (spMIC)
		{
			g_pMathInput->OnInsert((LPCWSTR)bstrRecoResult);
			//hr = spMIC->Hide();
			return hr;
		}
		return E_FAIL;
	}

	HRESULT __stdcall OnMICClose()
	{
		CComPtr<IMathInputControl> spMIC;
		HRESULT hr = m_pUnknown->QueryInterface<IMathInputControl>(&spMIC);
		if (SUCCEEDED(hr))
		{
			hr = spMIC->Hide();
			return hr;
		}
		return hr;
	}
};