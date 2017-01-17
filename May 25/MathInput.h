#pragma once
#ifndef MATHINPUT_H
#define MATHINPUT_H

#include "micaut.h"
#include <atlbase.h>	// CComPtr

class MathInput {
	CComPtr<IMathInputControl> spMIC; // Math Input Control

public:
	HRESULT CreateMathInput();
	HRESULT SetParent(HWND hWNd);
	HRESULT Show();
};

extern MathInput g_mMathInput;

#endif // !MATHINPUT_H


