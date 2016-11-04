#pragma once
#ifndef MYWINDOWSPROCS_H
#define MYWINDOWSPROCS_H
#include "WindowsGlobalVariables.h"
#include "MyWindowsStuff.h"
#include "Geometry.h"
#include "DirectXGlobalVariables.h"

LRESULT CALLBACK					WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK					UIWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK					DXWndProc(HWND, UINT, WPARAM, LPARAM);

ATOM								RegisterMyClass(HINSTANCE hInstance);
bool								InitWindow(HINSTANCE, int);
#endif // !MYWINDOWSPROCS_H
