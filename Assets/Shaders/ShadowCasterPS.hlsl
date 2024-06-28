#include "ShadowCaster.hlsli"
SamplerState smp : register(s0);

float4 main() : SV_TARGET
{
    return float4(1, 0, 0, 1);
}