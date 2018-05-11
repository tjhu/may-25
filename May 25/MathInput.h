//******************************************************//
//	MathInput.h by Tianjiao Huang, 2016-2018			//
//******************************************************//
#pragma once
#ifndef MATHINPUT_H
#define MATHINPUT_H
// Reference: https://msdn.microsoft.com/en-us/library/windows/desktop/dd371425(v=vs.85).aspx

#include "micaut.h"
#include <atlbase.h>	// CComPtr
#include "Imathinput.h" // This should be the lase one

class MathInput;



const _ATL_FUNC_INFO CMathInputControlEventHandler<MathInput>::OnMICInsertInfo = { CC_STDCALL, VT_I4, 1,{ VT_BSTR } };
const _ATL_FUNC_INFO CMathInputControlEventHandler<MathInput>::OnMICCloseInfo = { CC_STDCALL, VT_I4, 0,{ VT_EMPTY } };

class MathInput : public CMathInputControlEventHandler<MathInput>
{
public:
	HWND TargetHWND;
	CComPtr<IMathInputControl> spMIC; // Math Input Control
	std::string RecognitionResult;

public:
	HRESULT CreateMathInput();
	HRESULT SetParent(HWND hWNd);
	HRESULT Show();
	void OnInsert(std::wstring result);
};

extern MathInput*			g_pMathInput;

#endif // !MATHINPUT_H


