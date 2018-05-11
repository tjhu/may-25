//******************************************************//
//MyDirectXStuff.h by Tianjiao Huang, 2016-2018			//
//******************************************************//
#pragma once
#ifndef MYDIRECTXSTUFF_H
#define MYDIRECTXSTUFF_H

void InitDevice(HWND hWnd, HINSTANCE hInstance);

void CompileShaders();

bool GetFrameRate(IDXGISwapChainMedia* pSwapChainMedia, UINT* pFrameRate);

void BuildGeometryBuffers(GeometryPointers mGeoPointers, ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexShader,
	UINT NumOfVertex, UINT NumOfIndice);

void BuildAxisGeometryBuffers(ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexBuffer, UINT& NumOfIndices_Cylinder, UINT& NumOfIndices_Cone);

void DrawSolids(RenderObject);

void DrawAxis();
#endif // !MYDIRECTXSTUFF_H
