#pragma once


#ifndef MYDIRECTXSTUFF_H
#define MYDIRECTXSTUFF_H

void BuildGeometryBuffers(GeometryPointers mGeoPointers, ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexShader,
	UINT NumOfVertex, UINT NumOfIndice);

void BuildAxisGeometryBuffers(ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexBuffer);

void DrawSolids(RenderObject);

#endif // !MYDIRECTXSTUFF_H
