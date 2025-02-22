#include "Depth.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 result = tex.Sample(smp, input.uv) * color;
    result.r = result.r * result.r * result.r * result.r * result.r;
    result.r *= result.r;
    result.r *= result.r;
    result.a = 1;
    
    return result;
	//return tex.Sample(smp, input.uv) * color;
}