#include "SSAO.hlsli"

const static float2 screenSize = float2(1920.f, 1080.f);

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.svpos = pos;
    output.uv = uv;

    //float2 screenPos = uv * screenSize;
    //float2 ndcPos = 2.0f * screenPos.x / screenSize.x - 1.0f;

    //ndcX = 2.0f * screenX / screenWidth - 1.0f;
    //ndcY = 1.0f - 2.0f * screenY / screenHeight;

    return output;
}