#include "HolographicCaster.hlsli"

static const float _FresnelSize = 1.0f;
static const float _FresnelMin = 0.0f;
static const float _FresnelMax = 1.0f;
static const float3 _FresnelColor = float3(1.0f, 0.0f, 0.0f);

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

static const float4 _RimColor = float4(1, 0, 0, 1);
static const float _RimStrength = 0.75f;

float FresnelEffect(float3 Normal, float3 ViewDir, float Power)
{
    return pow((1.0 - saturate(dot(normalize(Normal), normalize(ViewDir)))), Power);
}

float4 main(V2P input) : SV_TARGET
{
    float4 texCol = tex.Sample(smp, input.uv);
    
    float3 N = input.normal;
    float3 V = normalize(cameraPos - input.worldpos.xyz);
    
    float fresnel = FresnelEffect(N, V, 2);
    
    // Fresnel
    //float NdotV = dot(N, V);
    //float4 fresnel = (1 - pow(saturate(NdotV), _FresnelSize));
    //fresnel = smoothstep(_FresnelMin, _FresnelMax, fresnel);
    //fresnel.rgb *= _FresnelColor;
    //fresnel.a = 1.f;
    
    //float4 result = smoothstep(0.45, 0.5, pow(rim, rimStrength.r)
    
    float4 result = fresnel.xxxx;
    result.rgba = smoothstep(texCol, fresnel * _RimColor, _RimStrength);
    //result.rgb *= _FresnelColor;
    //result.a = 1.0f;
    return result;
    
    
    //float4 result = texCol;
    //return result;
}