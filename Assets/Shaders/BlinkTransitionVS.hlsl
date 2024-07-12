#include "BlinkTransition.hlsli"

float4 ComputeScreenPos(float4 clipPos);

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.svpos = pos;
    output.uv = uv;
    return output;
}
