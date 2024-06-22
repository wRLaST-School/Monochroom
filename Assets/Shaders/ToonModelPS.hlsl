#include "ToonModel.hlsli"

static const float _LightingCutoff = 0.1f;
static const float _FalloffAmount = 0.1f;
static const float _Smoothness = 5.0f;
static const float _FresnelSize = 1.0f;
static const float _FresnelMin = 1.0f;
static const float _FresnelMax = 1.0f;
static const float3 _FresnelColor = float3(1.0f, 1.0f, 1.0f);
static const float3 _ShadowColor = float3(0.25f, 0.25f, 0.25f);

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

float4 calcRim(GSOutput i, float4 color)
{
    float3 eyeDir = normalize(cameraPos.xyz - i.worldpos.xyz);

    half rim = 1.0 - abs(dot(eyeDir, i.normal));
    float3 emission = lerp(color.rgb, rimColor.rgb, smoothstep(0.45, 0.5, pow(rim, rimStrength.r) * rimStrength.r));
    color.rgb = emission;
    return color;
}

float4 main(VSOutput input) : SV_TARGET
{
    float3 L = lightVec;
    float3 N = input.normal;
	
	//Directional Light
	// Diffuse
    float NdotL = dot(N, L);
    float3 diffuse = 0.5f + NdotL * 0.5f;
    float3 dRate = smoothstep(_LightingCutoff, _LightingCutoff + _FalloffAmount, diffuse);
    diffuse = dRate * m_diffuse + (1 - dRate) * _ShadowColor;
	
	// Specular
    float3 V = normalize(cameraPos - input.worldpos.xyz);
    float3 H = normalize(V + L);
    float NdotH = dot(N, H);
    float3 specular = pow(saturate(NdotH), _Smoothness) * diffuse;
	
	// Fresnel
    float NdotV = dot(N, V);
    float3 fresnel = (1 - pow(saturate(NdotV), _FresnelSize)) * diffuse;
    fresnel = smoothstep(_FresnelMin, _FresnelMax, fresnel);
    fresnel *= _FresnelColor;

    float4 shadeCol;
    shadeCol.rgb = (diffuse + specular + fresnel) * lightColor.rgb;
    shadeCol.a = 1.0f;

    return float4(diffuse.rgb, 1.0f);
	
    float4 texCol = tex.Sample(smp, input.uv);
    return texCol * shadeCol /* * brightness*/;

	
 //   float4 texcolor = float4(tex.Sample(smp, input.uv));
	
 //   float4 shadecolor;

 //   float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
 //   const float luster = 4.0f;

 //   float3 L = lightVec;
 //   float3 N = input.normal;
    
	////Directional Light
	//{
 //       float NdotL = dot(N, L);
 //       float3 dotLightNormal = dot(lightVec, input.normal);
 //       dotLightNormal = smoothstep(0.3f, 0.6f, dotLightNormal);

 //       float3 ambient = m_ambient;
 //       float3 diffuse = 0.5f + NdotL * 0.5f;
 //       //float3 dRate = smoothstep(_LightingCutoff, _LightingCutoff + _FalloffAmount, diffuse);
 //       //diffuse = dRate * m_diffuse;
        
 //       //m_diffuse;
 //       float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);
 //       float3 specular = pow(saturate(dot(reflect, eyeDir)), luster) * m_specular;

 //       float3 color = (ambient + diffuse /*+ specular*/) * lightColor;
 //       shadecolor.xyz = color;
 //       shadecolor.a = m_alpha;
 //   }

 //   return shadecolor * texcolor * brightness;
}