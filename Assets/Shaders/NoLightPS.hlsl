#include "NoLight.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> disTex : register(t1);

SamplerState smp : register(s0);

float4 calcRim(VSOutput i, float4 color)
{
    float3 eyeDir = normalize(cameraPos.xyz - i.worldpos.xyz);

    half rim = 1.0 - abs(dot(eyeDir, i.normal));
    float3 emission = lerp(color.rgb, rimColor.rgb, smoothstep(0.45, 0.5, pow(rim, rimStrength.r) * rimStrength.r));
    color.rgb = emission;
    return color;
}

float4 main(VSOutput input) : SV_TARGET
{
    //return float4(brightness.rgb, 1.0f);
    
    //float4 dissolvecolor = float4(disTex.Sample(smp, input.uv));
	
    //clip(dissolvecolor.r - 0.00001f - dissolveStrength.r);
    float4 texColor = tex.Sample(smp, input.uv * tiling + offset);
    //float4 texColor = tex.Sample(smp, (input.uv/* + offset*/) * offset);
    clip(texColor.a - 0.5f);
    return (texColor * brightness);
}