//******************************************************//
//	CommonStruct.cpp by Tianjiao Huang, 2016-2018		//
//******************************************************//
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

struct FPOINT // 2D point in float
{
	float x = 0;
	float y = 0;
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
	XMFLOAT4 FresnelR0AndShininess;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mWorldViewProj;
	XMMATRIX mWorldLightviewProj;
	XMMATRIX mWorldInvTranspose;
	Material mMaterial;
	XMFLOAT3 mEyePosW;
};

struct cbPerFrame
{
	XMMATRIX mWorldViewProj;
	XMMATRIX mWorldLightviewProj;
	XMFLOAT3 mEyePosW;
};

struct ConstantBuffer_Shadowmap
{
	XMMATRIX mWorldViewProj;
};

struct GeometryPointers
{
	SimpleVertex*           pVertexPointer = nullptr;
	UINT*					pIndexPointer = nullptr;
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
	CrossSection_Semicircle,
	CrossSection_EquilateralTriangle,
	CrossSection_Square
};

enum RenderObject
{
	Shadowmap,
	Scene
};