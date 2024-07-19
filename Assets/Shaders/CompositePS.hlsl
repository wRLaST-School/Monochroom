#include "Composite.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
Texture2D<float4> mask : register(t2);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 result = float4(0, 0, 0, 1);
    float4 col0 = tex0.Sample(smp, input.uv);
    float4 col1 = tex1.Sample(smp, input.uv);
    
    float m = mask.Sample(smp, input.uv).a;
    if (m > 0)
    {
        result = col1;
    }
    else
    {
        result = col0 + col1;
    }
    return float4(result.rgb, 1);
}