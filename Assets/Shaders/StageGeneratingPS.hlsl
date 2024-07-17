#include "StageGenerating.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> depthTex : register(t1);
Texture2D<float4> planeTex : register(t2);
SamplerState smp : register(s0);

float GrayScale(float4 col)
{
    return 0.2125 * col.r + 0.7154 * col.g + 0.0721 * col.b;
}

float Sobel(Texture2D<float4> tex, float2 uv, float2 pixelSize)
{
    float gx[9];
    gx[0] = -1.0;
    gx[1] = -2.0;
    gx[2] = -1.0;
    gx[3] = 0.0;
    gx[4] = 0.0;
    gx[5] = 0.0;
    gx[6] = 1.0;
    gx[7] = 2.0;
    gx[8] = 1.0;

    float gy[9];
    gy[0] = -1.0;
    gy[1] = 0.0;
    gy[2] = 1.0;
    gy[3] = -2.0;
    gy[4] = 0.0;
    gy[5] = 2.0;
    gy[6] = -1.0;
    gy[7] = 0.0;
    gy[8] = 1.0;

    float2 suv[9];
    suv[0] = uv + pixelSize * float2(-1, -1);
    suv[1] = uv + pixelSize * float2(0, -1);
    suv[2] = uv + pixelSize * float2(1, -1);
    suv[3] = uv + pixelSize * float2(-1, 0);
    suv[4] = uv + pixelSize * float2(0, 0);
    suv[5] = uv + pixelSize * float2(1, 0);
    suv[6] = uv + pixelSize * float2(-1, 1);
    suv[7] = uv + pixelSize * float2(0, 1);
    suv[8] = uv + pixelSize * float2(1, 1);

    float edgeX = 0;
    float edgeY = 0;
    for (int index = 0; index < 9; index++)
    {
        float4 texCol = tex.Sample(smp, suv[index]);
        float grayScale = GrayScale(texCol);
        edgeX += texCol * gx[index];
        edgeY += texCol * gy[index];
    }

    float edge = abs(edgeX) + abs(edgeY);
    return edge;
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 mainCol = tex.Sample(smp, input.uv);
    float depth = depthTex.Sample(smp, input.uv).r;
    float plane = planeTex.Sample(smp, input.uv).r;
    
    float4 eage = (Sobel(tex, input.uv, 0.0015f) * (float4(1, 0, 1, 0)));
    
    float4 result = plane;
    result.rgb = depth < plane ? mainCol.rgb :/* mainCol.rgb * */eage.rgb;
    //result.rgb = mainCol.rgb * eage.rgb;
    //result.rgb = float3(1, 0, 0);
    result.a = 1;
    return result;
}