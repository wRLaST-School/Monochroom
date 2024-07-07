#include "SSAO.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    return float4(1, 0, 0, 1);

    //const float sigma = 0.0025f;
    //const float loopNum = 5;
    //float4 gaussianBlur = GaussianBlur(tex, input.uv, sigma, loopNum);
    //gaussianBlur.a = 1;
    //return gaussianBlur;
}