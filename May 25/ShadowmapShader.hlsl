
cbuffer cbPerObject : register(b0)
{
    matrix gWorldViewProj;
};

struct VertexIn
{
    float4 PosL : POSITION;
    float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 Normal : NORMAL;
};

VertexOut ShadowMapVS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to homogeneous clip space.
    vout.PosH = mul(vin.PosL, gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    vout.Normal = vin.Normal;
    
    return vout;
}

void ShadowMapPS(VertexOut pin)
{
}