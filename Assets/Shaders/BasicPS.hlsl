#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> disTex : register(t1);
Texture2D<float4> shadowMap : register(t2);

SamplerState smp : register(s0);

float4 calcRim(GSOutput i, float4 color)
{
    float3 eyeDir = normalize(cameraPos.xyz - i.worldpos.xyz);

    half rim = 1.0 - abs(dot(eyeDir, i.normal));
    float3 emission = lerp(color.rgb, rimColor.rgb, smoothstep(0.45, 0.5, pow(rim, rimStrength.r) * rimStrength.r));
    color.rgb = emission;
    return color;
}

float HeightFog(float4 wpos)
{
    float3 origin = float3(0, 0, 0);
    
    if (wpos.y >= 0)
    {
        return 0.f;
    }
    
    const float maxDis = 50.f;
    float dis = distance(wpos.y, origin.y) / maxDis;

    float rate = smoothstep(0.f, 0.7f, dis);
    return rate;
}

float4 main(GSOutput input) : SV_TARGET
{
    float4 texcolor = float4(tex.Sample(smp, input.uv));
    //float4 dissolvecolor = float4(disTex.Sample(smp, input.uv));
	
    //clip(dissolvecolor.r - 0.00001f - dissolveStrength.r);

    float4 shadecolor;

    float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
    const float luster = 4.0f;

	//Directional Light
	{

        float3 dotLightNormal = dot(lightVec, input.normal);

        float3 ambient = m_ambient;
        float3 diffuse = dotLightNormal * m_diffuse;
        float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);
        float3 specular = pow(saturate(dot(reflect, eyeDir)), luster) * m_specular;

        float3 color = (ambient + diffuse + specular) * lightColor;
        shadecolor.xyz = color;
        shadecolor.a = m_alpha;
    }


	//Point Lights
    for (int i = 0; i < MAX_PLIGHTS; i++)
    {
        if (pointLights[i].isActive)
        {
            float3 lightVec = pointLights[i].lightPos - input.worldpos.xyz;
            float d = length(lightVec);
            lightVec = normalize(lightVec);

            float att = 1.0f / (pointLights[i].lightAtt.x + pointLights[i].lightAtt.y * d + pointLights[i].lightAtt.z * d * d);
			
            float3 dotLightNormal = dot(lightVec, input.normal);

            float3 diffuse = dotLightNormal * m_diffuse;

            float3 reflect = normalize(-lightVec + 2 * dotLightNormal * input.normal);

            float3 specular = specular = pow(saturate(dot(reflect, eyeDir)), luster) * m_specular;

			//shadecolor.rgb += att * (diffuse + specular) * pointLights[i].lightColor;
			
            float3 color = att * (diffuse + specular) * pointLights[i].lightColor;
            if (color.x < 0.f)
                color.x = 0.f;
            if (color.y < 0.f)
                color.y = 0.f;
            if (color.z < 0.f)
                color.z = 0.f;
            shadecolor.rgb += color;
        }
		
    }
	
    float rate = HeightFog(input.worldpos);
    
    float4 ads = shadecolor * texcolor * brightness;
    ads.rgb = ads.rgb * (1 - rate) + float3(0, 0, 0) * rate;
    return calcRim(input, ads);
}