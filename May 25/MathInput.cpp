#include "stdafx.h"
#include "MathInput.h"
#include "micaut_i.c"

HRESULT MathInput::CreateMathInput()
{
	// Create math input control
	if (SUCCEEDED(this->spMIC.CoCreateInstance(CLSID_MathInputControl))) {
		// Enable redo button
		return this->spMIC->EnableExtendedButtons(VARIANT_TRUE);
	}
	return E_FAIL;
}

HRESULT MathInput::SetParent(HWND hWnd)
{
	return this->spMIC->SetOwnerWindow((LONG_PTR)hWnd);
}

HRESULT MathInput::Show()
{
	return this->spMIC->Show();
}
