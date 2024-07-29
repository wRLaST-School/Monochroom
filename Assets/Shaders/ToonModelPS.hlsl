#include "ToonModel.hlsli"

static const float _LightingCutoff = 0.1f;
static const float _FalloffAmount = 0.1f;
static const float _Smoothness = 50.f;
static const float _FresnelSize = 1.0f;
static const float _FresnelMin = 1.0f;
static const float _FresnelMax = 1.0f;
static const float3 _FresnelColor = float3(1.0f, 1.0f, 1.0f);
static const float3 _ShadowColor = float3(0.25f, 0.25f, 0.25f);
static const float _ShadowBias = 0.001f;

Texture2D<float4> tex : register(t0);
Texture2D<float4> dissolveTex : register(t1);
Texture2D<float4> shadowMap : register(t2);

SamplerState smp : register(s0);

// ソフトシャドウ
float CalcShadowPFC(float4 spos);

float4 main(VSOutput input) : SV_TARGET
{
    //return float4(shadowMap.Sample(smp, input.uv).rrr, 1.0f);
    float3 L = lightVec;
    float3 N = input.normal;
	
	//Directional Light
	// Diffuse
    float NdotL = dot(N, L);
    float3 diffuse = 0.5f + NdotL * 0.5f;
    float3 dRate = smoothstep(_LightingCutoff, _LightingCutoff + _FalloffAmount, diffuse);
    diffuse = dRate + (1 - dRate) * _ShadowColor;
	
	// Specular
    float3 V = normalize(cameraPos - input.worldpos.xyz);
    float3 H = normalize(V + L);
    float NdotH = dot(N, H);
    float3 specular = pow(saturate(NdotH), _Smoothness) * m_specular * diffuse;
	
	// Fresnel
    float NdotV = dot(N, V);
    float3 fresnel = (1 - pow(saturate(NdotV), _FresnelSize)) * diffuse;
    fresnel = smoothstep(_FresnelMin, _FresnelMax, fresnel);
    fresnel *= _FresnelColor;

    float4 shadeCol;
    shadeCol.rgb = (diffuse /*+ specular*/ + fresnel) * lightColor.rgb;
    shadeCol.a = 1.0f;

    float shadow = CalcShadowPFC(input.shadowpos);
    
    float4 texCol = tex.Sample(smp, input.uv * tiling);
    //float4 texCol = tex.Sample(smp, input.uv * tiling);
    
    float2 blockSize = float2(64.f, 64.f);
    float2 blockUV = floor(input.uv * blockSize) / blockSize;
    
    float dissolveCol = dissolveTex.Sample(smp, blockUV + float2(0, dissolveStrength.x / 10.f)).r;
    
    //return float4(dissolveCol2, 0, 0, 1);
    
    float t = dissolveStrength;
    float tsub = 0.03f;
    
    float drate = step(t, dissolveCol.r).xxxx;
    float drate2 = step(t - tsub, dissolveCol.r).xxxx;
    texCol.rgb = texCol.rgb * (1 - drate2) + (drate2 - drate) * float3(0.2f, 0.3f, 0.9f);
    
    float4 result = texCol * shadeCol * brightness;
    result.rgb *= shadow;
    
    return result;
}

// ソフトシャドウ
float CalcShadowPFC(float4 spos)
{
    float shadow = 1;
    
    // ライトビューでのスクリーン空間でのz値を計算する
    float z = spos.z / spos.w;
    
    // シャドウマップのUVを算出
    float2 shadowTexUV = spos.xy / spos.w;
    shadowTexUV *= float2(0.5f, -0.5f);
    shadowTexUV += 0.5f;
    
    float shadowFactor = 0;
    float shiftNum = 3;
    float shiftWidth = 0.0001f;
    float count = 0;
    [unroll]
    for (float py = -shiftNum / 2; py <= shiftNum / 2; py++)
    {
        [unroll]
        for (float px = -shiftNum / 2; px <= shiftNum / 2; px++)
        {
		    // 色取得するUV座標
            float2 offset = float2(px, py);
            float2 pickUV = shadowTexUV + offset * shiftWidth;
            
            // 画面外の色を取得しないように
            pickUV = clamp(pickUV, 0.001, 0.999);

            // シャドウ マップから深度をサンプリング
            float shadowDepth = shadowMap.Sample(smp, pickUV).r;
            if (shadowDepth + _ShadowBias < z)
            {
                shadowFactor += 0.35f;
            }
            count++;
        }
    }

    // サンプル数で割って正規化
    shadow = 1 - shadowFactor / count;
    return shadow;
}