#include "GrayScale.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> texMask : register(t1);
SamplerState smp : register(s0);

static const float4 kGoggleFrameColor={0,0,0,1};
static const float4 kGoggleLensColor={0.2125f, 0.7154f, 0.0721f,1};

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = tex.Sample(smp, input.uv + offset);
    //ゴーグルのフレームか
   float goggleFrameRate = texMask.Sample(smp, input.uv + offset).r;
    //ゴーグルのレンズか
    float goggleLensRate = texMask.Sample(smp, input.uv + offset).b;
    
    //ゴーグルの色（レンズかフレームか）
    float4 color2 = lerp(color,kGoggleFrameColor,goggleFrameRate.xxxx);
    
    //グレースケールの色
    float grayScale = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    
    float4 color3 = lerp(color2, grayScale.xxxx, goggleLensRate.xxxx);
    color3.a = 1.f;

    return color3;
}