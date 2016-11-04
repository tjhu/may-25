#pragma once
#include <DirectXMath.h>
#include <Windows.h>
using namespace DirectX;

// Structures
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};

struct Light
{
    XMFLOAT3 Strength;
    float FalloffStart;		// point/spot light only
    XMFLOAT3 Direction;		// directional/spot light only
    float FalloffEnd;		// point/spot light only
    XMFLOAT3 Position;		// point light only
    float SpotPower;		// spot light only
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mWorldViewProj;
	XMMATRIX mWorldLightviewProj;
	XMMATRIX mWorldInvTranspose;
	XMFLOAT3 mEyePosW;
	UINT ColorSwitch;
};

struct ConstantBuffer_Shadowmap
{
	XMMATRIX mWorldViewProj;
};

struct GeometryPointers
{
	SimpleVertex*           pVertexPointer = nullptr;
	WORD*					pIndexPointer = nullptr;
};


// enum
enum ErrorCode
{
	INPUT_OK,
	InvalidFunction_1,
	InvalidFunction_2,
	InvalidLeftBound,
	InvalidRightBound
};

enum BoundToWhat
{
	BoundToLeft,
	BoundToRight
};

enum SolidMethod
{
	Disk,
	Washer,
	Shell,
	CrossSection_Semicircle,
	CrossSection_EquilateralTriangle,
	CrossSection_Square
};

enum RenderObject
{
	Shadowmap,
	Scene
};