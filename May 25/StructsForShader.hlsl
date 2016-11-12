//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

struct Light
{
    float3 Strength;
    float FalloffStart; // point/spot light only
    float3 Direction; // directional/spot light only
    float FalloffEnd; // point/spot light only
    float3 Position; // point light only
    float SpotPower; // spot light only
};

struct Material
{
    float4 DiffuseAlbedo;
    float4 FresnelR0AndShininess;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix WorldViewProj;
    matrix WorldLightviewProj;
    matrix WorldInvTranspose;
    Material mMaterial;
    float3 EyePos;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 PosW : POSITION;
	float4 Pos_L : TEXCOORD0;
    float3 Normal : NORMAL;
    Material a : TEXCOORD1;
};