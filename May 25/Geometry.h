#pragma once
#include <DirectXMath.h>
#include "DXUtility.h"

GeometryPointers BuildDiskGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice);

GeometryPointers BuildEntireWasherGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice,
	UINT NCount, float LeftBound, float RightBound, std::string Expression_1, std::string Expression_2);

GeometryPointers BuildEntireShellGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice, UINT NCount,
	float LeftBound, float RightBound, std::string Expression_1, BoundToWhat g_BoundToWhat);

GeometryPointers BuildCrossSectionGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice, 
	SolidMethod g_SolidMethod);

GeometryPointers BuildConeGeometryBuffers(UINT * NumOfVertices_Axes, UINT * NumOfIndices_Axes);

GeometryPointers BuildInfDiskGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice);



//void BuildAxesGeometryBuffers();