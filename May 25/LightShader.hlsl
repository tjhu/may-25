//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

// Defaults for number of lights.
#include "LightingUtil.hlsl"

//sampler ShadowMapSampler : register(s0);;
//Texture2D Shadowmap : register(t0);;

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.PosW = mul(input.Pos, World).xyz;
    output.Pos = mul(input.Pos, WorldViewProj);
    output.Normal = mul(input.Normal, (float3x3) WorldInvTranspose);
    output.Pos_L = mul(input.Pos, WorldLightviewProj);
    return output;
}

float4 PS(PS_INPUT pin) : SV_Target
{
    float3 FresnelR0 = mMaterial.FresnelR0AndShininess.xyz;
    float Shininess = mMaterial.FresnelR0AndShininess.w;
    float4 AmbientLight = (0.15f, 0.15f, 0.15f, 1.0f);

    Light Lights[16];
    Lights[0].Direction = float3(0.57735f, -0.57735f, 0.57735f);
    //Lights[0].Direction = float3(0.7071067811865475f, 0.0f, 0.7071067811865475f);
    Lights[0].Strength = float3(0.4f, 0.4f, 0.4f);

    // Interpolating normal can unnormalize it, so renormalize it.
    pin.Normal = normalize(pin.Normal);

    // Vector from point being lit to eye. 
    float3 toEyeW = normalize(EyePos - pin.PosW);

	// Indirect lighting.
    float4 ambient = AmbientLight * mMaterial.DiffuseAlbedo;

    pin.Pos_L.xyz = pin.Pos_L.xyz / pin.Pos_L.w;
    pin.Pos_L.xy = (pin.Pos_L.xy + 1) / 2;
    //float Deep_Shadowmap = Shadowmap.Sample(ShadowMapSampler, float2(pin.Pos_L.x, pin.Pos_L.y));

    // Compute Shadow
    //if (pin.Pos_L.z > Deep_Shadowmap + 0.000001f) return ambient;

    Material mat = { mMaterial.DiffuseAlbedo, FresnelR0, Shininess };
    float3 shadowFactor = 1.0f;
    float4 directLight = ComputeLighting(Lights, mat, pin.PosW,
        pin.Normal, toEyeW, shadowFactor);

    float4 litColor = ambient + directLight;

    // Common convention to take alpha from diffuse material.
    litColor.a = mMaterial.DiffuseAlbedo.a;

    return litColor;
}