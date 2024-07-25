cbuffer cbuff0 : register(b0)
{
    float2 offset;
	float grayEffectRatio;
	float2 grayPoint1;
	float2 grayPoint2;
    float grayEffectRadius;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};