#pragma once


#ifndef MYDIRECTXSTUFF_H
#define MYDIRECTXSTUFF_H

void BuildGeometryBuffers(GeometryPointers mGeoPointers, ID3D11Buffer** ppVertexBuffer, ID3D11Buffer** ppIndexShader,
	UINT NumOfVertex, UINT NumOfIndice);



#endif // !MYDIRECTXSTUFF_H
