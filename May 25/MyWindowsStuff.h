//******************************************************//
//	MyWindowsStuff.h by Tianjiao Huang, 2016-2018		//
//******************************************************//
#pragma once

#ifndef MYWINDOWSSTUFF_H
#define MYWINDOWSSTUFF_H

#include <Windows.h>

#define MaxRaidus 50.0f
#define MinRaidus 1.0f

#define AxisMin -15.0f
#define AxisMax +15.0f

void								TcharToChar(char* String);

ErrorCode							ValidateInput(std::string, std::string, std::string, std::string);
void								PopInvalideMessage(ErrorCode ErrorMessage);
void								SetInputs(std::string, std::string, std::string, std::string, UINT);
void								SetInputs();
void								SetSetting();
void								UpdateSetting();
void								UpdateVariables();
void								Integration();
void								GuessZero(HWND hWnd);


// Controls
void								OnMouseDown(HWND g_hWndDX, POINT* mLastMousePos, WPARAM btnState, int x, int y);
void								OnMouseMove(WPARAM btnState, POINT* mLastMousePos,
												int x, int y, float* mTheta, float* mPhi, float* mRadius);
void								DXOnKeyDown(float* mTheta, float* mPhi, float* mRadius, FPOINT* pos);
void								OnKeyDown(HWND hWnd, HWND g_hWndButton);

void								AdjustClientRect(RECT ClientRect, RECT* DXClientRect, RECT* UIClientRect);

void								AdjustControlCoords(HWND);


#endif