cbuffer cbuff0 : register(b0)
{
    float2 offset;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};