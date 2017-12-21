//******************************************************//
//MathInput.cpp by Tianjiao Huang, 2016-2017				//
//******************************************************//
#include "stdafx.h"
#include "MathInput.h"
#include "micaut_i.c"
#include "Utility.h"

// #import "C:\Users\maiti\Source\Repos\May 25\CSharpSBML\bin\Debug\CCR.tlb" no_namespace

MathInput*			g_pMathInput;

HRESULT MathInput::CreateMathInput()
{
	CoInitialize(NULL);
	HRESULT hr = spMIC.CoCreateInstance(CLSID_MathInputControl);
	if (SUCCEEDED(hr)) {
		hr = CMathInputControlEventHandler<MathInput>::Initialize(spMIC);
		if (SUCCEEDED(hr)) {
			hr = CMathInputControlEventHandler<MathInput>::DispEventAdvise(spMIC);
			if (SUCCEEDED(hr)) {
				CComBSTR cap1(L"Please write your function here");
				spMIC->SetCaptionText((BSTR)cap1);
				spMIC->EnableExtendedButtons(VARIANT_TRUE);
				//spMIC->SetPreviewHeight(200);
			}
		}
	}
	return hr;
}

HRESULT MathInput::SetParent(HWND hWnd)
{
	RECT parentRect, thisRect;
	LONG height, width, left, top;
	GetWindowRect(hWnd, &parentRect);
	height = (parentRect.bottom - parentRect.top) * 1 / 3;
	width = (parentRect.right - parentRect.left) * 1 / 3;
	top = parentRect.top + (parentRect.bottom - parentRect.top) / 3;
	left = parentRect.left +  (parentRect.right - parentRect.left) / 3;
	thisRect = { left, top, left + width, top + height };
	this->spMIC->SetPosition(thisRect.left, thisRect.top, thisRect.right, thisRect.bottom);
	return this->spMIC->SetOwnerWindow((LONG_PTR)hWnd);
}

HRESULT MathInput::Show()
{
	return this->spMIC->Show();
}

void MathInput::OnInsert(std::wstring result)
{
	std::wstring output = result;
	const auto len = (output.length() + 1)*2;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), output.c_str(), len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hMem);
	CloseClipboard();
	std::string a;
	try
	{
		a = MathMLToInfix(output);
	}
	catch (std::wstring e)
	{
		MessageBox(NULL, e.c_str(), L"Exception thrown when parsing MathML", NULL);
		return;
	}

	SetWindowTextA(this->TargetHWND, a.c_str());
	this->spMIC->Clear();
	this->spMIC->Hide();
}
