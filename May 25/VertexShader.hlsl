#include "StructsForShader.hlsl"
#include "LightingUtil.hlsl"
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.PosW = mul(input.Pos, World).xyz;
    output.Pos = mul(input.Pos, WorldViewProj);
    output.Normal = mul(input.Normal, (float3x3) WorldInvTranspose);
    output.Pos_L = mul(input.Pos, WorldLightviewProj);
    return output;
}
