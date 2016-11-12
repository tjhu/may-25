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

struct Material
{
	XMFLOAT4 DiffuseAlbedo;
	XMFLOAT3 FresnelR0;
	float Shininess;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mWorldViewProj;
	XMMATRIX mWorldLightviewProj;
	XMMATRIX mWorldInvTranspose;
	XMFLOAT3 mEyePosW;
	Material mMaterial;
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