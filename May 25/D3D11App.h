     //*************************************************************************************************************************//
    //																														   //
   //										Copyright (c) 2016, Tianjiao Huang												  //	
  //												All rights reserved.													 //
 //																															//
//*************************************************************************************************************************//


#pragma once
#ifndef D3D11App_H
#define D3D11App_H


#include "stdafx.h"
#include "resource.h"
#include "Geometry.h"
#include "MyWindowsProcs.h"

using namespace DirectX;

//Variables
char LogFile[50] = "Log.txt";


//---------------------------------------------------
// DirectX Stuff
//---------------------------------------------------

// Variables
D3D_DRIVER_TYPE				g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL			g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*				g_pd3dDevice = nullptr;
ID3D11DeviceContext*		g_pImmediateContext = nullptr;
IDXGISwapChain*				g_pSwapChain = nullptr;
IDXGISwapChain1*			g_pSwapChain1 = nullptr;
ID3D11RenderTargetView*		g_pRenderTargetView = nullptr;
ID3D11Texture2D*			g_pDepthStencil = nullptr;
ID3D11DepthStencilView*		g_pDepthStencilView = nullptr;
ID3D11Texture2D*			g_Shadowmap = nullptr;
ID3D11DepthStencilView*		g_ShadowmapDepthView = nullptr;
ID3D11ShaderResourceView *	g_ShadowmapResourceView = nullptr;
ID3D11VertexShader*			g_pVertexShader = nullptr;
ID3D11PixelShader*			g_pPixelShader = nullptr;
ID3D11VertexShader*			g_pShadowmapVertexShader = nullptr;
ID3D11PixelShader*			g_pShadowmapPixelShader = nullptr;
ID3D11PixelShader*			g_pPixelShaderSolid = nullptr;
ID3D11InputLayout*			g_pVertexLayout = nullptr;
ID3D11Buffer*				g_pVertexBuffer = nullptr;
ID3D11Buffer*				g_pIndexBuffer = nullptr;
ID3D11Buffer*				g_pAxesVertexBuffer = nullptr;
ID3D11Buffer*				g_pAxesIndexBuffer = nullptr;
ID3D11Buffer*				g_pFunctionVertexBuffer = nullptr;
ID3D11Buffer*				g_pFunctionIndexBuffer = nullptr;
ID3D11Buffer*				g_pConstantBuffer = nullptr;
ID3D11Buffer*				g_pConstantBuffer_Shadowmap = nullptr;
ID3D11SamplerState*			g_pSamplerState = nullptr;

XMMATRIX					g_View = XMMatrixIdentity();
XMMATRIX					g_Projection = XMMatrixIdentity();
XMMATRIX					g_World = XMMatrixIdentity();
XMFLOAT3					g_LightDir = XMFLOAT3{ 0.57735f, -0.57735f, 0.57735f };
XMMATRIX					g_LightView = XMMatrixIdentity();

std::array <std::wstring, 6> SolidMethodDropDownList =
{
	L"Disk",
	L"Washer",
	L"Shell (in terms of y)",
	L"CrossSection-Semicircle",
	L"CrossSection-EquilateralTriangle",
	L"CrossSection-Square"
};


UINT					stride = sizeof(SimpleVertex);
UINT					NumOfIndices_Solid = 0;
UINT					NumOfIndices_Function = 0;

UINT					NumOfIndices_Cone = 0;
UINT					NumOfIndices_Cylinder = 0;

UINT					NumOfIndices_Axis = 0;

POINT					mLastMousePos = { 0,0 };

// Input File
std::string				Expression_1 = "";
std::string				UnparsedExpression_1 = "4-(x-2)^2";
std::string				Expression_2 = "";
std::string				UnparsedExpression_2 = " 0";

// Setting File
float					mTheta = 137.23f;
float					mPhi = 1.47994f;
float					mRadius = 18.0f;
bool					AxisOn = FALSE;

// Functions
void InitDevice(HWND hWnd, HINSTANCE hInstance);
void CompileShaders();
void Render();
void RenderTest();
void CleanupDevice();
void UpdateScene();

//---------------------------------------------------
// Window Stuff
//---------------------------------------------------
std::ofstream						LogFileObject;
#endif
