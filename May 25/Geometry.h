#pragma once
#include <DirectXMath.h>
#include "DXUtility.h"

#define VertexColor XMFLOAT3({ 1.000000000f, 0.411764741f, 0.705882370f })
#define OuterVertexColor XMFLOAT3({ 1.000000000f * 0.95f, 0.411764741f * 0.95f, 0.705882370f * 0.95f })
#define SomeGreen XMFLOAT4({ 0.2392156862745098f, 0.6980392156f, 0.45490196078f, 1.000000000f })
#define HotPink  XMFLOAT4({ 1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f })
#define Gold XMFLOAT4({ 1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f })

GeometryPointers BuildDiskGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice);

GeometryPointers BuildEntireWasherGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice,
	UINT NCount, float LeftBound, float RightBound, std::string Expression_1, std::string Expression_2);

GeometryPointers BuildEntireShellGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice, UINT NCount,
	float LeftBound, float RightBound, std::string Expression_1, BoundToWhat g_BoundToWhat);

GeometryPointers BuildCrossSectionGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice, 
	SolidMethod g_SolidMethod);
//
//GeometryPointers BuildInfDiskGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice);

//void BuildAxesGeometryBuffers();