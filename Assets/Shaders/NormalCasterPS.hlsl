#include "NormalCaster.hlsli"
SamplerState smp : register(s0);

float4 main(V2P i) : SV_TARGET
{
    float3 resultNormal = normalize(i.normal.xyz);
    
    return float4(resultNormal * 2.f - 1.0f, 1.0f);
    //return float4(0, 1, 0, 1.0f);
}