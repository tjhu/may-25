#include "stdafx.h"
#include "Geometry.h"

GeometryPointers BuildDiskGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice)
{
	
	*NumOfVertex = 4 * NumOfSlices + 2;
	SimpleVertex* vertices;
	vertices = new SimpleVertex[*NumOfVertex];

	*NumOfIndice = 4 * NumOfSlices * 3;
	WORD* indices;
	indices = new WORD[*NumOfIndice];

	float radius = 1.0f;
	float a = 0.0f;
	float b = 1.0f;

	// Create vertex buffer


	for (unsigned int i = 0; i < NumOfSlices + 1; i++)
	{
		if (i == 0)
		{
			//center
			vertices[0] = { XMFLOAT3{ a, 0.0f, 0.0f }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
			vertices[NumOfSlices + 1] = { XMFLOAT3{ b, 0.0f, 0.0f }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			continue;
		}
		float theta = XM_2PI * (i - 1) / NumOfSlices;
		float nY = cos(theta);
		float nZ = sin(theta);
		float y = radius * nY;
		float z = radius * nZ;
		// Top and Bottom
		vertices[i] = { XMFLOAT3{ a, y, z }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
		vertices[i + 1 + NumOfSlices] = { XMFLOAT3{ b, y, z }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
		// Side
		vertices[2 * NumOfSlices + 1 + i] = { vertices[i].Pos, XMFLOAT3{0.0f, nY, nZ} };
		vertices[3 * NumOfSlices + 1 + i] = { vertices[i + 1 + NumOfSlices].Pos, XMFLOAT3{ 0.0f, nY, nZ } };
	}


	// Create index buffer

	// Left Hand Rule!

	// Top
	unsigned int k = 0;
	for (unsigned int i = 0; i < NumOfSlices - 1; i++)
	{
		indices[k++] = 0;
		indices[k++] = i + 2;
		indices[k++] = i + 1;
	}
	indices[k++] = 0;
	indices[k++] = 1;
	indices[k++] = NumOfSlices;

	// Bottom
	for (unsigned int i = 0; i < NumOfSlices - 1; i++)
	{
		indices[k++] = NumOfSlices + 1;
		indices[k++] = NumOfSlices + i + 2;
		indices[k++] = NumOfSlices + i + 3;
	}
	indices[k++] = NumOfSlices + 1;
	indices[k++] = NumOfSlices * 2 + 1;
	indices[k++] = NumOfSlices + 2;

	// Side
	for (unsigned int i = 1; i < NumOfSlices; i++)
	{
		indices[k++] = 2 * NumOfSlices + 1 + i;
		indices[k++] = 2 * NumOfSlices + 2 + i;
		indices[k++] = 3 * NumOfSlices + 2 + i;

		indices[k++] = 2 * NumOfSlices + 1 + i;
		indices[k++] = 3 * NumOfSlices + 2 + i;
		indices[k++] = 3 * NumOfSlices + 1 + i;
	}
	indices[k++] = 3 * NumOfSlices + 1;
	indices[k++] = 2 * NumOfSlices + 2;
	indices[k++] = 3 * NumOfSlices + 2;

	indices[k++] = 3 * NumOfSlices + 1;
	indices[k++] = 3 * NumOfSlices + 2;
	indices[k++] = 4 * NumOfSlices + 1;

	GeometryPointers mGeoPointers;
	mGeoPointers.pVertexPointer = vertices;
	mGeoPointers.pIndexPointer = indices;
	return mGeoPointers;
}

GeometryPointers BuildEntireWasherGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice,
	UINT NCount, float LeftBound, float RightBound, std::string Expression_1, std::string Expression_2)
{
	*NumOfVertex = 8 * NumOfSlices * NCount;
	SimpleVertex* vertices = nullptr;
	vertices = new SimpleVertex[*NumOfVertex];
	*NumOfIndice = 8 * NumOfSlices * NCount * 3;
	WORD* indices = nullptr;
	indices = new WORD[*NumOfIndice];

	float yf = 0.0f;
	float y0 = 0.0f;

	float x0 = (LeftBound < RightBound) ? LeftBound : RightBound;
	float dx = abs(RightBound - LeftBound) / (float)NCount;
	float theta = 0.0f;
	float dTheta = XM_2PI / NumOfSlices;


	for (UINT i = 0; i < NCount; i++)
	{
		float xf = x0 + dx;
		y0 = abs(evaluate(Expression_1, x0));
		yf = abs(evaluate(Expression_2, x0));
		if (y0 > yf)
			std::swap(y0, yf);
		for (UINT j = 0; j < NumOfSlices; j++)
		{
			theta = j * dTheta;
			float nY = sin(theta);
			float nZ = cos(theta);
			float pY0 = y0 * nY;
			float pYf = yf * nY;
			float pZ0 = y0 * nZ;
			float pZf = yf * nZ;
			// Top
			vertices[j + 0 * NumOfSlices + i * NumOfSlices * 8] 												// top inner
				= { XMFLOAT3{ x0, pY0, pZ0 }, XMFLOAT3{ -1.0f, 0.0f, 0.0f} };
			vertices[j + 1 * NumOfSlices + i * NumOfSlices * 8] 												// top outer
				= { XMFLOAT3{ x0, pYf, pZf }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
			// Bottom
			vertices[j + 2 * NumOfSlices + i * NumOfSlices * 8] 												// botton inner
				= { XMFLOAT3{ xf, pY0, pZ0 }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			vertices[j + 3 * NumOfSlices + i * NumOfSlices * 8]													// button outer
				= { XMFLOAT3{ xf, pYf, pZf }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			// Inner side
			vertices[j + 4 * NumOfSlices + i * NumOfSlices * 8] 												// top inner
				= { XMFLOAT3{ x0, pY0, pZ0 }, XMFLOAT3{ 0.0f, -nY, -nZ } };
			vertices[j + 5 * NumOfSlices + i * NumOfSlices * 8] 												// botton inner
				= { XMFLOAT3{ xf, pY0, pZ0 }, XMFLOAT3{ 0.0f, -nY, -nZ } };
			// Outer side
			vertices[j + 6 * NumOfSlices + i * NumOfSlices * 8] 												// top outer
				= { XMFLOAT3{ x0, pYf, pZf }, XMFLOAT3{ 0.0f, nY, nZ } };
			vertices[j + 7 * NumOfSlices + i * NumOfSlices * 8]													// button outer
				= { XMFLOAT3{ xf, pYf, pZf }, XMFLOAT3{ 0.0f, nY, nZ } };
		}
		x0 += dx;
	}
	// Create index buffer
	// Left Hand Rule
	DWORD k = 0;
	UINT space = 0; // The space of vertices bettwwen each Washer-Solid being drawn
	for (UINT c = 0; c < NCount; c++)
	{
		space = c * 8 * NumOfSlices;
		// Top
		{

			for (UINT i = 0; i < NumOfSlices - 1; i++)
			{
				indices[k++] = space + NumOfSlices + i;
				indices[k++] = space + i + 1;
				indices[k++] = space + i;

				indices[k++] = space + NumOfSlices + i;
				indices[k++] = space + NumOfSlices + i + 1;
				indices[k++] = space + i + 1;
			}
			indices[k++] = space + 2 * NumOfSlices - 1;
			indices[k++] = space + 0;
			indices[k++] = space + NumOfSlices - 1;

			indices[k++] = space + 2 * NumOfSlices - 1;
			indices[k++] = space + NumOfSlices;
			indices[k++] = space + 0;
		}

		// Botton
		{
			for (UINT i = 0; i < NumOfSlices - 1; i++)
			{
				indices[k++] = space + 3 * NumOfSlices + i;
				indices[k++] = space + 2 * NumOfSlices + i;
				indices[k++] = space + 2 * NumOfSlices + i + 1;

				indices[k++] = space + 3 * NumOfSlices + i;
				indices[k++] = space + 2 * NumOfSlices + i + 1;
				indices[k++] = space + 3 * NumOfSlices + i + 1;

			}
			indices[k++] = space + 4 * NumOfSlices - 1;
			indices[k++] = space + 3 * NumOfSlices - 1;
			indices[k++] = space + 2 * NumOfSlices;

			indices[k++] = space + 4 * NumOfSlices - 1;
			indices[k++] = space + 2 * NumOfSlices;
			indices[k++] = space + 3 * NumOfSlices;
		}

		// Inner side
		{
			for (UINT i = 0; i < NumOfSlices - 1; i++)
			{
				indices[k++] = space + 4 * NumOfSlices + i;
				indices[k++] = space + 4 * NumOfSlices + i + 1;
				indices[k++] = space + 5 * NumOfSlices + i + 1;

				indices[k++] = space + 4 * NumOfSlices + i;
				indices[k++] = space + 5 * NumOfSlices + i + 1;
				indices[k++] = space + 5 * NumOfSlices + i;
			}
			indices[k++] = space + 5 * NumOfSlices - 1;
			indices[k++] = space + 4 * NumOfSlices;
			indices[k++] = space + 5 * NumOfSlices;

			indices[k++] = space + 5 * NumOfSlices - 1;
			indices[k++] = space + 5 * NumOfSlices;
			indices[k++] = space + 6 * NumOfSlices - 1;
		}

		// Outer side
		{
			for (UINT i = 0; i < NumOfSlices - 1; i++)
			{
				indices[k++] = space + 6 * NumOfSlices + i;
				indices[k++] = space + 7 * NumOfSlices + i;
				indices[k++] = space + 6 * NumOfSlices + i + 1;

				indices[k++] = space + 6 * NumOfSlices + i + 1;
				indices[k++] = space + 7 * NumOfSlices + i;
				indices[k++] = space + 7 * NumOfSlices + i + 1;
			}
			indices[k++] = space + 7 * NumOfSlices - 1;
			indices[k++] = space + 8 * NumOfSlices - 1;
			indices[k++] = space + 6 * NumOfSlices;

			indices[k++] = space + 6 * NumOfSlices;
			indices[k++] = space + 8 * NumOfSlices - 1;
			indices[k++] = space + 7 * NumOfSlices;
		}
	}

	GeometryPointers mGeoPointers;
	mGeoPointers.pVertexPointer = vertices;
	mGeoPointers.pIndexPointer = indices;
	return mGeoPointers;
}

GeometryPointers BuildEntireShellGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice, UINT NCount,
	float LeftBound, float RightBound, std::string Expression_1, BoundToWhat g_BoundToWhat)
{
	// Function_1: with respect to y
	*NumOfVertex = 8 * NumOfSlices * NCount;
	SimpleVertex* vertices = nullptr;
	vertices = new SimpleVertex[*NumOfVertex];
	*NumOfIndice = 8 * NumOfSlices * NCount * 3;
	WORD* indices = nullptr;
	indices = new WORD[*NumOfIndice];

	bool IsBoundToLeft = (g_BoundToWhat == BoundToLeft) ? TRUE : FALSE;

	float yLeftBound = LeftBound;
	float yRightBound = RightBound;
	float xLeftBound = evaluate(Expression_1, LeftBound);
	float xRightBound = evaluate(Expression_1, RightBound);
	if (xLeftBound > xRightBound)
		std::swap(xLeftBound, xRightBound);

	float xf = 0.0f;
	float x0 = 0.0f;

	if (IsBoundToLeft)
	{
		x0 = xLeftBound;
	}
	else
	{
		xf = xRightBound;
	}

	float y0 = yLeftBound;
	float yf = 0.0f;
	float dy = (yRightBound - yLeftBound) / (float)NCount;
	float theta = 0.0f;
	float dTheta = XM_2PI / NumOfSlices;

	// Fill in vertices
	for (UINT i = 0; i < NCount; i++)
	{
		if (IsBoundToLeft)
		{
			xf = evaluate(Expression_1, y0);
		}
		else
		{
			x0 = evaluate(Expression_1, y0);
		}

		for (UINT j = 0; j < NumOfSlices; j++)
		{
			yf = y0 + dy;
			theta = j * dTheta;
			float nY = sin(theta);
			float nZ = cos(theta);
			float pY0 = y0 * nY;
			float pYf = yf * nY;
			float pZ0 = y0 * nZ;
			float pZf = yf * nZ;
			// Top
			vertices[j + 0 * NumOfSlices + i * NumOfSlices * 8] 												// top inner
				= { XMFLOAT3{ x0, pY0, pZ0 }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
			vertices[j + 1 * NumOfSlices + i * NumOfSlices * 8] 												// top outer
				= { XMFLOAT3{ x0, pYf, pZf }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
			// Bottom
			vertices[j + 2 * NumOfSlices + i * NumOfSlices * 8] 												// botton inner
				= { XMFLOAT3{ xf, pY0, pZ0 }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			vertices[j + 3 * NumOfSlices + i * NumOfSlices * 8]													// button outer
				= { XMFLOAT3{ xf, pYf, pZf }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			// Inner side
			vertices[j + 4 * NumOfSlices + i * NumOfSlices * 8] 												// top inner
				= { XMFLOAT3{ x0, pY0, pZ0 }, XMFLOAT3{ 0.0f, -nY, -nZ } };
			vertices[j + 5 * NumOfSlices + i * NumOfSlices * 8] 												// botton inner
				= { XMFLOAT3{ xf, pY0, pZ0 }, XMFLOAT3{ 0.0f, -nY, -nZ } };
			// Outer side
			vertices[j + 6 * NumOfSlices + i * NumOfSlices * 8] 												// top outer
				= { XMFLOAT3{ x0, pYf, pZf }, XMFLOAT3{ 0.0f, nY, nZ } };
			vertices[j + 7 * NumOfSlices + i * NumOfSlices * 8]													// button outer
				= { XMFLOAT3{ xf, pYf, pZf }, XMFLOAT3{ 0.0f, nY, nZ } };
		}
		y0 += dy;
	}

	// Fill in indices
	DWORD k = 0;
	UINT space = 0;
	for (UINT c = 0; c < NCount; c++)
	{
		space = c * 8 * NumOfSlices;
		// Top
		{
			for (UINT i = 0; i < NumOfSlices - 1; i++)
			{
				indices[k++] = space + NumOfSlices + i;
				indices[k++] = space + i + 1;
				indices[k++] = space + i;

				indices[k++] = space + NumOfSlices + i;
				indices[k++] = space + NumOfSlices + i + 1;
				indices[k++] = space + i + 1;
			}
			indices[k++] = space + 2 * NumOfSlices - 1;
			indices[k++] = space + 0;
			indices[k++] = space + NumOfSlices - 1;

			indices[k++] = space + 2 * NumOfSlices - 1;
			indices[k++] = space + NumOfSlices;
			indices[k++] = space + 0;
		}

		// Botton
		{
			for (UINT i = 0; i < NumOfSlices - 1; i++)
			{
				indices[k++] = space + 3 * NumOfSlices + i;
				indices[k++] = space + 2 * NumOfSlices + i;
				indices[k++] = space + 2 * NumOfSlices + i + 1;

				indices[k++] = space + 3 * NumOfSlices + i;
				indices[k++] = space + 2 * NumOfSlices + i + 1;
				indices[k++] = space + 3 * NumOfSlices + i + 1;

			}
			indices[k++] = space + 4 * NumOfSlices - 1;
			indices[k++] = space + 3 * NumOfSlices - 1;
			indices[k++] = space + 2 * NumOfSlices;

			indices[k++] = space + 4 * NumOfSlices - 1;
			indices[k++] = space + 2 * NumOfSlices;
			indices[k++] = space + 3 * NumOfSlices;
		}

		// Inner side
		{
			for (UINT i = 0; i < NumOfSlices - 1; i++)
			{
				indices[k++] = space + 4 * NumOfSlices + i;
				indices[k++] = space + 4 * NumOfSlices + i + 1;
				indices[k++] = space + 5 * NumOfSlices + i + 1;

				indices[k++] = space + 4 * NumOfSlices + i;
				indices[k++] = space + 5 * NumOfSlices + i + 1;
				indices[k++] = space + 5 * NumOfSlices + i;
			}
			indices[k++] = space + 5 * NumOfSlices - 1;
			indices[k++] = space + 4 * NumOfSlices;
			indices[k++] = space + 5 * NumOfSlices;

			indices[k++] = space + 5 * NumOfSlices - 1;
			indices[k++] = space + 5 * NumOfSlices;
			indices[k++] = space + 6 * NumOfSlices - 1;
		}

		// Outer side
		{
			for (UINT i = 0; i < NumOfSlices - 1; i++)
			{
				indices[k++] = space + 6 * NumOfSlices + i;
				indices[k++] = space + 7 * NumOfSlices + i;
				indices[k++] = space + 6 * NumOfSlices + i + 1;

				indices[k++] = space + 6 * NumOfSlices + i + 1;
				indices[k++] = space + 7 * NumOfSlices + i;
				indices[k++] = space + 7 * NumOfSlices + i + 1;
			}
			indices[k++] = space + 7 * NumOfSlices - 1;
			indices[k++] = space + 8 * NumOfSlices - 1;
			indices[k++] = space + 6 * NumOfSlices;

			indices[k++] = space + 6 * NumOfSlices;
			indices[k++] = space + 8 * NumOfSlices - 1;
			indices[k++] = space + 7 * NumOfSlices;
		}
	}

	GeometryPointers mGeoPointers;
	mGeoPointers.pVertexPointer = vertices;
	mGeoPointers.pIndexPointer = indices;
	return mGeoPointers;
}

GeometryPointers BuildCrossSectionGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice,
	SolidMethod g_SolidMethod)
{
	SimpleVertex* vertices = nullptr;
	WORD* indices = nullptr;

	switch (g_SolidMethod)
	{
	case CrossSection_Semicircle:
	{
		*NumOfVertex = (NumOfSlices + 1) * 4 + 8;
		*NumOfIndice = (4 * (NumOfSlices + 1)) * 3;
		vertices = new SimpleVertex[*NumOfVertex];
		indices = new WORD[*NumOfIndice];
		float radius = 1.0f;
		float a = 0.0f;
		float b = 1.0f;
		float dTheta = XM_PI / (float)(NumOfSlices);
		float theta = 0.0f;
		float pY = 0.0f;
		float pZ = 0.0f;
		float nY = 0.0f;
		float nZ = 0.0f;

		// Left center
		vertices[0] = { XMFLOAT3{ a, 0.0f, 0.0f }, XMFLOAT3{ -1.0f, 0.0f, 0.0f} };
		// Right center
		vertices[NumOfSlices + 2] = { XMFLOAT3{ b, 0.0f, 0.0f }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
		// Fill in vertices
		for (UINT i = 0; i < NumOfSlices + 1; i++)
		{
			theta = i * dTheta;
			nY = sin(theta);
			nZ = cos(theta);
			pY = radius * sin(theta);
			pZ = radius * cos(theta);
			// Left
			vertices[i + 1] = { XMFLOAT3{ a, pY, pZ }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
			// Right
			vertices[i + 2 + (NumOfSlices + 1)] = { XMFLOAT3{ b, pY, pZ }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			// Left curved side
			vertices[i + 2 + 2 * (NumOfSlices + 1)] = { XMFLOAT3{ a, pY, pZ }, XMFLOAT3{ 0.0f, nY, nZ } };
			// Right curved side
			vertices[i + 2 + 3 * (NumOfSlices + 1)] = { XMFLOAT3{ b, pY, pZ }, XMFLOAT3{ 0.0f, nY, nZ } };
		}
		WORD j = 0;
		// Left Bottom side
		vertices[2 + 4 * (NumOfSlices + 1)] = { XMFLOAT3{ a, 0.0f, radius }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };
		vertices[3 + 4 * (NumOfSlices + 1)] = { XMFLOAT3{ a, 0.0f, 0.0f }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };
		vertices[4 + 4 * (NumOfSlices + 1)] = { XMFLOAT3{ a, 0.0f, -radius }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };
		// Right Bottom side
		vertices[5 + 4 * (NumOfSlices + 1)] = { XMFLOAT3{ b, 0.0f, radius }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };
		vertices[6 + 4 * (NumOfSlices + 1)] = { XMFLOAT3{ b, 0.0f, 0.0f }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };
		vertices[7 + 4 * (NumOfSlices + 1)] = { XMFLOAT3{ b, 0.0f, -radius }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };


		UINT k = 0;
		// Fill in indices
		// Sides
		for (UINT i = 0; i < NumOfSlices; i++)
		{
			// Left side
			indices[k++] = i + 1;
			indices[k++] = i + 2;
			indices[k++] = 0;

			// Right side
			indices[k++] = i + NumOfSlices + 4;
			indices[k++] = i + NumOfSlices + 3;
			indices[k++] = NumOfSlices + 2;
		}

		// Top
		for (UINT i = 0; i < NumOfSlices; i++)
		{
			indices[k++] = i + 2 * (NumOfSlices + 1) + 2;
			indices[k++] = i + 3 * (NumOfSlices + 1) + 3;
			indices[k++] = i + 2 * (NumOfSlices + 1) + 3;


			indices[k++] = i + 2 * (NumOfSlices + 1) + 2;
			indices[k++] = i + 3 * (NumOfSlices + 1) + 2;
			indices[k++] = i + 3 * (NumOfSlices + 1) + 3;
		}

		// Button
		{
			indices[k++] = 2 + 4 * (NumOfSlices + 1);
			indices[k++] = 3 + 4 * (NumOfSlices + 1);
			indices[k++] = 6 + 4 * (NumOfSlices + 1);

			indices[k++] = 2 + 4 * (NumOfSlices + 1);
			indices[k++] = 6 + 4 * (NumOfSlices + 1);
			indices[k++] = 5 + 4 * (NumOfSlices + 1);

			indices[k++] = 3 + 4 * (NumOfSlices + 1);
			indices[k++] = 4 + 4 * (NumOfSlices + 1);
			indices[k++] = 7 + 4 * (NumOfSlices + 1);

			indices[k++] = 3 + 4 * (NumOfSlices + 1);
			indices[k++] = 7 + 4 * (NumOfSlices + 1);
			indices[k++] = 6 + 4 * (NumOfSlices + 1);
		}
	}
	break;
	case CrossSection_EquilateralTriangle:
	{
		*NumOfVertex = 6 + 3 * 4;
		*NumOfIndice = 3 * (2 + 3 * 2);
		vertices = new SimpleVertex[*NumOfVertex];
		indices = new WORD[*NumOfIndice];

		float a = 0.0f;
		float b = 1.0f;
		float Height = 1.0f;
		float SemiWidth = Height / sqrt(3.0f);

		UINT k = 0;
		// Fill in vertices
		{
			// Left 
			vertices[k++] = { XMFLOAT3{ a, 0.0f , SemiWidth }, XMFLOAT3{  -1.0f, 0.0f, 0.0f } };
			vertices[k++] = { XMFLOAT3{ a, Height , 0.0f }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
			vertices[k++] = { XMFLOAT3{ a, 0.0f , -SemiWidth }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };

			// Right 
			vertices[k++] = { XMFLOAT3{ b, 0.0f , SemiWidth }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			vertices[k++] = { XMFLOAT3{ b, Height , 0.0f }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			vertices[k++] = { XMFLOAT3{ b, 0.0f , -SemiWidth }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };

			float nY = sin((XM_PI / 3));
			float nZ = cos((XM_PI / 3));
			// L
			vertices[k++] = { XMFLOAT3{ b, 0.0f , SemiWidth }, XMFLOAT3{ 0.0f, nY, nZ } };
			vertices[k++] = { XMFLOAT3{ b, Height , 0.0f }, XMFLOAT3{ 0.0f, nY, nZ } };
			vertices[k++] = { XMFLOAT3{ a, Height , 0.0f }, XMFLOAT3{ 0.0f, nY, nZ } };
			vertices[k++] = { XMFLOAT3{ a, 0.0f , SemiWidth }, XMFLOAT3{ 0.0f, nY, nZ } };

			// R
			vertices[k++] = { XMFLOAT3{ b, 0.0f , -SemiWidth }, XMFLOAT3{ 0.0f, nY, -nZ } };
			vertices[k++] = { XMFLOAT3{ b, Height , 0.0f }, XMFLOAT3{ 0.0f, nY, -nZ } };
			vertices[k++] = { XMFLOAT3{ a, Height , 0.0f }, XMFLOAT3{ 0.0f, nY, -nZ } };
			vertices[k++] = { XMFLOAT3{ a, 0.0f , -SemiWidth }, XMFLOAT3{ 0.0f, nY, -nZ } };

			// B
			vertices[k++] = { XMFLOAT3{ a, 0.0f , SemiWidth }, XMFLOAT3{ 0.0f, -nY, 0.0f } };
			vertices[k++] = { XMFLOAT3{ a, 0.0f , -SemiWidth }, XMFLOAT3{ 0.0f, -nY, 0.0f } };
			vertices[k++] = { XMFLOAT3{ b, 0.0f , -SemiWidth }, XMFLOAT3{ 0.0f, -nY, 0.0f } };
			vertices[k++] = { XMFLOAT3{ b, 0.0f , SemiWidth }, XMFLOAT3{ 0.0f, -nY, 0.0f } };
		}

		k = 0;
		// Fill in indices
		{
			std::array <WORD, 3 * (2 + 3 * 2)>  aIndices = {
				// Left
				0, 1, 2,

				// Right
				5, 4, 3,

				// Sideral
				6, 7, 8,		6, 8, 9,
				10, 12, 11, 	10, 13, 12,
				14, 15, 16,		14, 16, 17
			};

			for (auto s : aIndices)
			{
				indices[k++] = s;
			}
		}
	}
	break;
	case CrossSection_Square:
	{
		*NumOfVertex = 24;
		*NumOfIndice = 3 * (6 * 2);
		vertices = new SimpleVertex[*NumOfVertex];
		indices = new WORD[*NumOfIndice];

		float a = 0.0f;
		float b = 1.0f;

		UINT k = 0;
		// Fill in vertices
		{
			// Left
			vertices[k++] = SimpleVertex{ { a, 1.0f, -0.5f }, XMFLOAT3{ -1.0f, 0.0f, 0.0f} };
			vertices[k++] = SimpleVertex{ { a, 1.0f, +0.5f }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { a, 0.0f, +0.5f }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { a, 0.0f, -0.5f }, XMFLOAT3{ -1.0f, 0.0f, 0.0f } };
			// Right
			vertices[k++] = SimpleVertex{ { b, 1.0f, -0.5f }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { b, 1.0f, +0.5f }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { b, 0.0f, +0.5f }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { b, 0.0f, -0.5f }, XMFLOAT3{ 1.0f, 0.0f, 0.0f } };
			// Top
			vertices[k++] = SimpleVertex{ { a, 1.0f, -0.5f }, XMFLOAT3{ 0.0f, 1.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { a, 1.0f, +0.5f }, XMFLOAT3{ 0.0f, 1.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { b, 1.0f, +0.5f }, XMFLOAT3{ 0.0f, 1.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { b, 1.0f, -0.5f }, XMFLOAT3{ 0.0f, 1.0f, 0.0f } };
			// Bottom
			vertices[k++] = SimpleVertex{ { a, 0.0f, -0.5f }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { a, 0.0f, +0.5f }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { b, 0.0f, +0.5f }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };
			vertices[k++] = SimpleVertex{ { b, 0.0f, -0.5f }, XMFLOAT3{ 0.0f, -1.0f, 0.0f } };
			// Front
			vertices[k++] = SimpleVertex{ { a, 0.0f, -0.5f }, XMFLOAT3{ 0.0f, 0.0f, -1.0f } };
			vertices[k++] = SimpleVertex{ { a, 1.0f, -0.5f }, XMFLOAT3{ 0.0f, 0.0f, -1.0f } };
			vertices[k++] = SimpleVertex{ { b, 1.0f, -0.5f }, XMFLOAT3{ 0.0f, 0.0f, -1.0f } };
			vertices[k++] = SimpleVertex{ { b, 0.0f, -0.5f }, XMFLOAT3{ 0.0f, 0.0f, -1.0f } };
			// Back
			vertices[k++] = SimpleVertex{ { a, 0.0f, 0.5f }, XMFLOAT3{ 0.0f, 0.0f, 1.0f } };
			vertices[k++] = SimpleVertex{ { a, 1.0f, 0.5f }, XMFLOAT3{ 0.0f, 0.0f, 1.0f } };
			vertices[k++] = SimpleVertex{ { b, 1.0f, 0.5f }, XMFLOAT3{ 0.0f, 0.0f, 1.0f } };
			vertices[k++] = SimpleVertex{ { b, 0.0f, 0.5f }, XMFLOAT3{ 0.0f, 0.0f, 1.0f } };
		}

		k = 0;
		// Fill in indices
		{
			std::array <WORD, 3 * (6 * 2)> aIndices = {
				// Left
				0, 2, 1, 		0, 3, 2,
				// Right
				5, 6, 4, 		6, 7, 4,
				// Top
				10, 11, 9, 		9, 11, 8,
				// Bottom
				12, 14, 13,		15, 14, 12,
				// Front
				17, 18, 19,		17, 19, 16,
				// Back
				21, 23, 22,		20, 23, 21
			};

			for (auto s : aIndices)
			{
				indices[k++] = s;
			}
		}
	}
	break;
	default:
		break;
	}

	GeometryPointers mGeoPointers;
	mGeoPointers.pVertexPointer = vertices;
	mGeoPointers.pIndexPointer = indices;
	return mGeoPointers;
}

GeometryPointers BuildInfDiskGeometryBuffers(UINT * NumOfVertex, UINT * NumOfIndice)
{

	return GeometryPointers();
}

GeometryPointers BuildConeGeometryBuffers(UINT * NumOfVertices_Axes, UINT * NumOfIndices_Axes)
{
	*NumOfVertices_Axes = NumOfSlices * 3 + 1;
	SimpleVertex* vertices = nullptr;
	vertices = new SimpleVertex[*NumOfVertices_Axes];
	*NumOfIndices_Axes = 2 * NumOfSlices * 3;
	WORD* indices = nullptr;
	indices = new WORD[*NumOfIndices_Axes];

	float radius = 1.0f;
	float height = 1.0f;

	float dTheta = XM_2PI / NumOfSlices;
	float innerTheta = XM_PI / NumOfSlices;
	float outerTheta = 0.0f;

	for (unsigned int i = 0; i < NumOfSlices; i++)
	{
		float y = radius * cos(outerTheta);
		float z = radius * sin(outerTheta);
		// Top
		vertices[i] = { XMFLOAT3{height, 0.0f, 0.0f}, XMFLOAT3{cos(innerTheta), sin(innerTheta), 0.0f} };
		// Side
		vertices[i + NumOfSlices] = { XMFLOAT3{0.0f, y, z}, XMFLOAT3{ cos(outerTheta), sin(outerTheta), 0.0f } };
		// Buttom
		vertices[i + 2 * NumOfSlices] = { XMFLOAT3{0.0f, y, z}, XMFLOAT3{-1.0f, 0.0f, 0.0f} };

		// Increment theta
		innerTheta += dTheta;
		outerTheta += dTheta;
	}
	vertices[3 * NumOfSlices] = { XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT3{-1.0f, 0.0f, 0.0f} };

	UINT k = 0;
	// Side
	for (WORD i = 0; i < NumOfSlices - 1; i++)
	{
		indices[k++] = i;
		indices[k++] = i + NumOfSlices;
		indices[k++] = i + NumOfSlices + 1;
	}
	indices[k++] = NumOfSlices - 1;
	indices[k++] = 2 * NumOfSlices - 1;
	indices[k++] = NumOfSlices;

	// Buttom
	for (WORD i = 0; i < NumOfSlices - 1; i++)
	{
		indices[k++] = 3 * NumOfSlices;
		indices[k++] = 2 * NumOfSlices + i + 1;
		indices[k++] = 2 * NumOfSlices + i;
	}
	indices[k++] = 3 * NumOfSlices;
	indices[k++] = 2 * NumOfSlices;
	indices[k++] = 3 * NumOfSlices - 1;

	GeometryPointers mGeoPointers;
	mGeoPointers.pVertexPointer = vertices;
	mGeoPointers.pIndexPointer = indices;
	return mGeoPointers;
}

//void BuildFunctionVertexBuffer()
//{
//	ReleaseCom(g_pFunctionVertexBuffer);
//
//	NumOfVertices_Function = NumOfPoint * 2;
//	SimpleVertex vertices[NumOfPoint * 2];
//
//	float b = (RightBound > AxisMax) ? RightBound : AxisMax;
//	float a = (LeftBound < AxisMin) ? LeftBound : AxisMin;
//	float dx = (b - a) / (float)(NumOfPoint -1);
//	float x = a;
//
//	for (UINT i = 0; i < NumOfPoint; i++)
//	{
//		// Function 1
//		vertices[i] = { XMFLOAT3{x, (float)evaluate(Expression_1, x), 0.0f }, FunctionColor_1 };
//		// Function 2
//		vertices[i + NumOfPoint] = { XMFLOAT3{ x, (float)evaluate(Expression_2, x), 0.0f }, FunctionColor_2 };
//		x += dx;
//	}
//
//
//	D3D11_BUFFER_DESC bd;
//	ZeroMemory(&bd, sizeof(bd));
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(SimpleVertex) * NumOfVertices_Function;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	D3D11_SUBRESOURCE_DATA InitData;
//	ZeroMemory(&InitData, sizeof(InitData));
//	InitData.pSysMem = vertices;
//	ThrowIfFailed(g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pFunctionVertexBuffer));
//}
//
//void BuildFunctionIndexBuffer()
//{
//	ReleaseCom(g_pFunctionIndexBuffer);
//
//	NumOfIndices_Function = 2 * ((NumOfPoint - 1) * 2);
//	WORD indices[2 * ((NumOfPoint - 1) * 2)];
//
//	UINT k = 0;
//	// Function 1
//	for (UINT i = 0; i < NumOfPoint - 1; i++)
//	{
//		indices[k++] = i;
//		indices[k++] = i + 1;
//	}
//
//	// Function 2
//	for (UINT i = 0; i < NumOfPoint - 1; i++)
//	{
//		indices[k++] = NumOfPoint + i;
//		indices[k++] = NumOfPoint + i + 1;
//	}
//
//	// Create index buffer
//	D3D11_BUFFER_DESC bd;
//	ZeroMemory(&bd, sizeof(bd));
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(WORD) * NumOfIndices_Function;        // 36 vertices needed for 12 triangles in a triangle list
//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	D3D11_SUBRESOURCE_DATA InitData;
//	ZeroMemory(&InitData, sizeof(InitData));
//	InitData.pSysMem = indices;
//	ThrowIfFailed(g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pFunctionIndexBuffer));
//}