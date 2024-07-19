#include "RGBShift.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

const static float2 _RShiftUV = float2(+0.006f, 0.f);
const static float2 _GShiftUV = float2(0.f, 0.f);
const static float2 _BShiftUV = float2(-0.008f, 0.f);
const static float _GlitchIntensity = 0.2f;

float Random(float2 seed)
{
    return frac(sin(dot(seed, float2(12.9898f, 78.233))) * 43758.5453f);
}

float BlockNoise(float2 uv, float density)
{
    float2 seed = floor(uv * density) / density;
    return Random(seed);
}

float4 main(VSOutput input) : SV_TARGET
{
    float noise = BlockNoise(float2(timer * 0.1f, input.uv.y + frac(timer) * 0.01f), 32) * 2.f - 1.f;
    noise += Random(input.uv.xx) * 0.3f;
    
    float2 uv = input.uv;
    float randomValue = Random(float2(input.uv.y, timer * 0.01f));
    uv.x += randomValue * sin(-sin(_GlitchIntensity) * 0.5f) * sin(-sin(noise) * 0.2f) * frac(timer);
    
    float r = tex.Sample(smp, uv + shiftR).r;
    float g = tex.Sample(smp, uv + shiftG).g;
    float b = tex.Sample(smp, uv + shiftB).b;
    
    float3 shift = float3(r, g, b);
    
    return float4(shift.rgb, 1);
    
}