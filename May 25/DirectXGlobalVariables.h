#pragma once
#include <DirectXMath.h>
#include "stdafx.h"
using namespace DirectX;

extern D3D_DRIVER_TYPE				g_driverType;
extern D3D_FEATURE_LEVEL			g_featureLevel;
extern ID3D11Device*				g_pd3dDevice;
extern ID3D11DeviceContext*			g_pImmediateContext;
extern IDXGISwapChain*				g_pSwapChain;
extern IDXGISwapChain1*				g_pSwapChain1;
extern ID3D11RenderTargetView*		g_pRenderTargetView;
extern ID3D11Texture2D*				g_pDepthStencil;
extern ID3D11DepthStencilView*		g_pDepthStencilView;
extern ID3D11Texture2D*				g_Shadowmap;
extern ID3D11DepthStencilView*		g_ShadowmapDepthView;
extern ID3D11ShaderResourceView *	g_ShadowmapResourceView;
extern ID3D11VertexShader*			g_pVertexShader;
extern ID3D11PixelShader*			g_pPixelShader;
extern ID3D11VertexShader*			g_pShadowmapVertexShader;
extern ID3D11PixelShader*			g_pShadowmapPixelShader;
extern ID3D11PixelShader*			g_pPixelShaderSolid;
extern ID3D11InputLayout*			g_pVertexLayout;
extern ID3D11Buffer*				g_pVertexBuffer;
extern ID3D11Buffer*				g_pIndexBuffer;
extern ID3D11Buffer*				g_pAxesVertexBuffer;
extern ID3D11Buffer*				g_pAxesIndexBuffer;
extern ID3D11Buffer*				g_pFunctionVertexBuffer;
extern ID3D11Buffer*				g_pFunctionIndexBuffer;
extern ID3D11Buffer*				g_pConstantBuffer;
extern ID3D11Buffer*				g_pConstantBuffer_Shadowmap;
extern ID3D11SamplerState*			g_pSamplerState;

extern UINT							stride;
extern UINT							NumOfIndices_Solid;
extern UINT							NumOfIndices_Function;
extern UINT							NumOfIndices_Cone;
extern UINT							NumOfIndices_Cylinder;
extern UINT							NumOfIndices_Axis;

