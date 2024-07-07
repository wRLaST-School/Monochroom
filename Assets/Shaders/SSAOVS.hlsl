#include "SSAO.hlsli"

const static float2 screenSize = float2(1920.f, 1080.f);

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.svpos = pos;
    output.uv = uv;

    float2 screenPos = uv * screenSize;
    float2 ndcPos = 2.0 * (screenPos / screenSize) - 1.0;
    float4 clipSpacePos = float4(ndcPos.x, -ndcPos.y, 0.0, 1.0);
    output.viewVec = clipSpacePos.xy;
    return output;
}