#include "stdafx.h"
#include "MyDirectXStuff.h"
#include "DirectXGlobalVariables.h"

void BuildGeometryBuffers(GeometryPointers mGeoPointers, ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexBuffer,
	UINT NumOfVertex, UINT NumOfIndice)
{
	// Release old geometry buffers
	ReleaseCom(pVertexBuffer);
	ReleaseCom(pIndexBuffer);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * NumOfVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = mGeoPointers.pVertexPointer;
	ThrowIfFailed(g_pd3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = NumOfIndice * sizeof(WORD);        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = mGeoPointers.pIndexPointer;
	ThrowIfFailed(g_pd3dDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer));


	delete[] mGeoPointers.pVertexPointer;
	mGeoPointers.pVertexPointer = nullptr;
	delete[] mGeoPointers.pIndexPointer;
	mGeoPointers.pIndexPointer = nullptr;
}