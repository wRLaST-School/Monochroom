#include "Silhouette.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

float4 main(V2P input) : SV_TARGET
{
    return float4(1, 1, 1, 1);
}