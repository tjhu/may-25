#include "stdafx.h"
#include "MathInput.h"
#include "micaut_i.c"

#import "C:\Users\maiti\Source\Repos\May 25\CSharpSBML\bin\Debug\CCR.tlb" no_namespace

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
				hr = spMIC->Show();
			}
		}
	}
	return hr;
}

HRESULT MathInput::SetParent(HWND hWnd)
{
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
	std::string a = MathMLToInfix(output);

	MessageBoxA(NULL, a.c_str(), NULL, NULL);
}
