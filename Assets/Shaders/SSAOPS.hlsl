#include "SSAO.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> depthTex : register(t1);
Texture2D<float4> normalMap : register(t2);
SamplerState smp : register(s0);

static const float _AOStrength = 1.2f;
static const int _SampleKernelCount = 16;
static const float _SampleKeneralRadius = 1.4f;
static const float _RangeStrength = 0.015f;
static const float _DepthBiasValue = 0.002f;

float Hash(float3 p)
{
    p = frac(p * 0.3183099 + 0.1);
    p *= 17.0;
    return frac(p.x * p.y * p.z * (p.x + p.y + p.z));
}

float3 RandomVec3(float2 seed)
{
    float3 seed3 = float3(seed.x, seed.y + 1.0f, seed.x * seed.y + 2.0f);
    float x = Hash(seed3);
    float y = Hash(seed3 + 31.0f);
    float z = Hash(seed3 + 63.0f);

    return normalize(float3(x, y, z));
}

float4 main(VSOutput input) : SV_TARGET
{
    //return float4(1, 0, 0, 1);
    
    float depth = depthTex.Sample(smp, input.uv);
    float3 viewNormal = normalMap.Sample(smp, input.uv);
    
    float3 viewPos = depth * input.viewVec;
    
    viewNormal = normalize(viewNormal) * float3(1, 1, -1);
    
    float2 noiseScale = screenSize / 4.0f;
    float2 noiseUV = input.uv * noiseScale;
    float3 randvec = RandomVec3(noiseUV);
    
    float3 tangent = normalize(randvec - viewNormal * dot(randvec, viewNormal));
    float3 bitangnt = cross(viewNormal, tangent);
    float3x3 TBN = float3x3(tangent, bitangnt, viewNormal);
    
    float ao = 0;
    for (int index = 0; index < _SampleKernelCount; index++)
    {
        float3 rand = RandomVec3(input.uv.yx);
        float scale = (float) index / (float) _SampleKernelCount;
        scale = lerp(0.01f, 1.0f, scale * scale);
        rand *= scale;
        
        float3 randomVec = mul(rand.xyz, TBN);
        
        float weight = smoothstep(0, 0.2, length(randomVec.xy));
        
        float3 randomPos = viewPos + randomVec * _SampleKeneralRadius;
        
        float3 rclipPos = mul((float3x3) projMat, randomPos);
        float2 rscreenPos = (rclipPos.xy / rclipPos.z) * 0.5 + 0.5;
        
        float randomDepth = depthTex.Sample(smp, rscreenPos);
        float randomNormal = normalMap.Sample(smp, rscreenPos);
        
        float range = abs(randomDepth - depth) > _RangeStrength ? 0.0 : 1.0;
        float selfCheck = randomDepth + _DepthBiasValue < depth ? 1.0 : 0.0;
        
        ao += range * selfCheck * weight;
    }
    
    ao /= _SampleKernelCount;
    ao = max(0.0, 1 - ao * _AOStrength);
    
    float4 mainCol = tex.Sample(smp, input.uv);
    
    return float4(mainCol.rgb * ao.xxx, 1.f);
}