#include "BilateralFilter.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> aoTex : register(t1);
Texture2D<float4> normalMap : register(t2);
SamplerState smp : register(s0);

static const float _BilaterFilterFactor = 0.01f;

float3 GetNormal(float2 uv)
{
    return normalMap.Sample(smp, uv);
}

float CompareNormal(float3 nor1, float3 nor2)
{
    //return 1.0f;
    return smoothstep(_BilaterFilterFactor, 1.0f, dot(nor1, nor2));
}

// ガウス関数
float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    // 描画ピクセルと色取得座標との距離
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 mainCol = tex.Sample(smp, input.uv);
    
    float2 texelSize = 1.0f / screenSize;
    float2 delta = texelSize * 8.f;
    
    float2 uv1 = input.uv + float2(-delta.x, -delta.y);
    float2 uv2 = input.uv + float2(0, -delta.y);
    float2 uv3 = input.uv + float2(+delta.x, -delta.y);
    float2 uv4 = input.uv + float2(-delta.x, 0);
    float2 uv5 = input.uv + float2(0, 0);
    float2 uv6 = input.uv + float2(+delta.y, 0);
    float2 uv7 = input.uv + float2(-delta.x, +delta.y);
    float2 uv8 = input.uv + float2(0, -delta.y);
    float2 uv9 = input.uv + float2(+delta.x, +delta.y);
    
    float3 normal1 = GetNormal(uv1);
    float3 normal2 = GetNormal(uv2);
    float3 normal3 = GetNormal(uv3);
    float3 normal4 = GetNormal(uv4);
    float3 normal5 = GetNormal(uv5);
    float3 normal6 = GetNormal(uv6);
    float3 normal7 = GetNormal(uv7);
    float3 normal8 = GetNormal(uv8);
    float3 normal9 = GetNormal(uv9);
    
    float4 col1 = aoTex.Sample(smp, uv1);
    float4 col2 = aoTex.Sample(smp, uv2);
    float4 col3 = aoTex.Sample(smp, uv3);
    float4 col4 = aoTex.Sample(smp, uv4);
    float4 col5 = aoTex.Sample(smp, uv5);
    float4 col6 = aoTex.Sample(smp, uv6);
    float4 col7 = aoTex.Sample(smp, uv7);
    float4 col8 = aoTex.Sample(smp, uv8);
    float4 col9 = aoTex.Sample(smp, uv9);
    
    float w1 = CompareNormal(normal5, normal1) * 0.075f;
    float w2 = CompareNormal(normal5, normal2) * 0.1f;
    float w3 = CompareNormal(normal5, normal3) * 0.075f;
    float w4 = CompareNormal(normal5, normal4) * 0.1f;
    float w5 = 0.3f;
    float w6 = CompareNormal(normal5, normal6) * 0.1f;
    float w7 = CompareNormal(normal5, normal7) * 0.075f;
    float w8 = CompareNormal(normal5, normal8) * 0.1f;
    float w9 = CompareNormal(normal5, normal9) * 0.075f;
    
    //float3 blCol = 0;
    //blCol += col1;
    //blCol += col2;
    //blCol += col3;
    //blCol += col4;
    //blCol += col6;
    //blCol += col7;
    //blCol += col8;
    //blCol += col9;
    //blCol /= 8.f;
    
    float3 blCol = 0;
    blCol += w1 * col1;
    blCol += w2 * col2;
    blCol += w3 * col3;
    blCol += w4 * col4;
    blCol += w6 * col6;
    blCol += w7 * col7;
    blCol += w8 * col8;
    blCol += w9 * col9;
    blCol /= w1 + w2 + w3 + w4 + w6 + w7 + w8 + w9;
    return float4(mainCol.rgb * blCol.rrr, 1.0);

    //float2 uv = input.uv;
    //float2 uv0a = input.uv - delta;
    //float2 uv0b = input.uv + delta;
    //float2 uv1a = input.uv - 2.0 * delta;
    //float2 uv1b = input.uv + 2.0 * delta;
    //float2 uv2a = input.uv - 3.0 * delta;
    //float2 uv2b = input.uv + 3.0 * delta;
    
    //float3 normal = GetNormal(uv);
    //float3 normal0a = GetNormal(uv0a);
    //float3 normal0b = GetNormal(uv0b);
    //float3 normal1a = GetNormal(uv1a);
    //float3 normal1b = GetNormal(uv1b);
    //float3 normal2a = GetNormal(uv2a);
    //float3 normal2b = GetNormal(uv2b);
    
    //float4 col = aoTex.Sample(smp, uv);
    //float4 col0a = aoTex.Sample(smp, uv0a);
    //float4 col0b = aoTex.Sample(smp, uv0b);
    //float4 col1a = aoTex.Sample(smp, uv1a);
    //float4 col1b = aoTex.Sample(smp, uv1b);
    //float4 col2a = aoTex.Sample(smp, uv2a);
    //float4 col2b = aoTex.Sample(smp, uv2b);

    //float w = 0.37004405286;
    //float w0a = CompareNormal(normal, normal0a) * 0.31718061674;
    //float w0b = CompareNormal(normal, normal0b) * 0.31718061674;
    //float w1a = CompareNormal(normal, normal1a) * 0.19823788546;
    //float w1b = CompareNormal(normal, normal1b) * 0.19823788546;
    //float w2a = CompareNormal(normal, normal2a) * 0.11453744493;
    //float w2b = CompareNormal(normal, normal2b) * 0.11453744493;
    
    //float3 blCol;
    //blCol = w * col.rgb;
    //blCol += w0a * col0a.rgb;
    //blCol += w0b * col0b.rgb;
    //blCol += w1a * col1a.rgb;
    //blCol += w1b * col1b.rgb;
    //blCol += w2a * col2a.rgb;
    //blCol += w2b * col2b.rgb;
    //blCol /= w + w0a + w0b + w1a + w1b + w2a + w2b;
    
    ////float4 mainCol = tex.Sample(smp, uv);
    //return float4( /*mainCol.rgb * */blCol.rrr, 1.0);
}