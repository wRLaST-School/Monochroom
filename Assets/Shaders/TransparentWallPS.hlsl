#include "TransparentWall.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> dissolveTex : register(t1);
Texture2D<float4> shadowMap : register(t2);

SamplerState smp : register(s0);

static const float3 _TargetPos = float3(0, 0, 0);
static const float _DefDis = 6;
static const float _MaxDis = 10;
static const float _RadiusRate = 1;

float4 main(VSOutput input) : SV_TARGET
{
    float dis = distance(input.worldpos.xyz, cameraPos);
    float mask = 1 - smoothstep(_DefDis, _MaxDis, dis * _RadiusRate);
    
    float4 texCol = tex.Sample(smp, input.uv);
    
    float4 result = texCol/* * mask*/;
    return result;
}