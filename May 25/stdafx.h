// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

//-------------------------------------
// Windows Header Files
//-------------------------------------
#include <windows.h>
#include <windowsx.h>

//-------------------------------------
// C RunTime Header Files
//-------------------------------------
#include <stdlib.h>
#include <fstream>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <array>
#include <algorithm>


//-------------------------------------
// DirectX Headers
//-------------------------------------
#include <d3d11.h>
#include <DirectXMath.h>
#include <DXGI1_2.h>
#include <d3dcompiler.h>
#include <directxcolors.h>

//-----------------------------------
// My Header Files
//-----------------------------------
#include "EvaluateExpression\tmath.h"
#include "CommonStruct.h"
#include "DXUtility.h"

//-----------------------------------
//My Definition
//-----------------------------------
#define NumOfChar 100

#define NumOfPoint 400
#define NumOfSlices 60

#define NumOfIntergal 1000
#define g_WorldCount 199

#define MaxLights 16


