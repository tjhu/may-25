//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix WorldViewProj;
    matrix WorldLightviewProj;
    matrix WorldInvTranspose;
    float3 EyePos;
    uint ColorSwitch;
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
};