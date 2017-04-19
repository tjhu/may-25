#pragma once

#ifndef GEOMETRY_H
#define GEOMETRY_H

#define NumOfSlices 40

enum SolidMethod
{
	Disk,
	Washer,
	CrossSection_Semicircle,
	CrossSection_EquilateralTriangle,
	CrossSection_Square
};

template <typename T>
struct DynamicArray
{
	UINT size;
	T *data;

	void release() { delete[] data; };
};

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
};

struct MeshData
{
	DynamicArray<SimpleVertex> vertices;
	DynamicArray<UINT> indices;

	void release() { vertices.release(); indices.release(); };
};

namespace Geometry
{
	MeshData BuildDiskGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice);

	//MeshData BuildEntireWasherGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice,
	//	UINT g_NCount, float g_LeftBound, float g_RightBound, std::wstring Expression_1, std::wstring Expression_2);
	//
	//MeshData BuildEntireShellGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice, UINT g_NCount,
	//	float g_LeftBound, float g_RightBound, std::wstring Expression_1, bool BoundToLeft);
	//
	//MeshData BuildCrossSectionGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice, 
	//	SolidMethod g_SolidMethod);
	//
	//MeshData BuildConeGeometryBuffers(UINT * NumOfVertices_Axis, UINT * NumOfIndices_Axis);
	//
	//MeshData BuildInfDiskGeometryBuffers(UINT* NumOfVertex, UINT* NumOfIndice);
	//
	//void SwapXAndYVertices(MeshData in, UINT n, UINT NumOfIndices);

	//void BuildAxesGeometryBuffers();

}

#endif // !GEOMETRY_H