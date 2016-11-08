#pragma once
#ifndef MYDIRECTXSTUFF_H
#define MYDIRECTXSTUFF_H

void BuildGeometryBuffers(GeometryPointers mGeoPointers, ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexShader,
	UINT NumOfVertex, UINT NumOfIndice);

void BuildAxisGeometryBuffers(ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexBuffer, UINT& NumOfIndices_Cylinder, UINT& NumOfIndices_Cone);

void DrawSolids(RenderObject);

void DrawAxis();
#endif // !MYDIRECTXSTUFF_H
