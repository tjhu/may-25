//******************************************************//
//ShadowmapShader.hlsl by Tianjiao Huang, 2016-2017		//
//******************************************************//
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

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to homogeneous clip space.
    vout.PosH = mul(vin.PosL, gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    vout.Normal = vin.Normal;
    
    return vout;
}

void PS(VertexOut pin)
{
}