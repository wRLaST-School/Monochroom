#include "GaussianBlur.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

// �K�E�X�֐�
float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    // �`��s�N�Z���ƐF�擾���W�Ƃ̋���
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

// �K�E�V�A���u���[
float4 GaussianBlur(Texture2D<float4> tex, float2 uv, float sigma, float loopNum)
{
    float4 result = float4(0, 0, 0, 0);
    float totalWeight = 0;
    float max = sigma * 2;
    float min = -sigma * 2;
    float stepWidth = (max - min) / loopNum;
    float asepect = 1920.f / 1080.f;

    [unroll]
    for (float py = min; py <= max; py += stepWidth)
    {
        [unroll]
        for (float px = min; px <= max; px += stepWidth)
        {
		    // �F�擾����UV���W
            float2 offset = float2(px, py) /* * float2(asepect, 1.0f)*/;
            float2 pickUV = uv + offset;
            
            // ��ʊO�̐F���擾���Ȃ��悤��
            pickUV = clamp(pickUV, 0.001, 0.999);

			// �E�F�C�g
            float weight = Gaussian(uv, pickUV, sigma);
            totalWeight += weight;

            result += tex.Sample(smp, pickUV) * weight;
        }
    }

    result.rgb = result.rgb / totalWeight;
    result.a = 1;
    return result;
}

float4 main(VSOutput input) : SV_TARGET
{
    const float sigma = 0.005f;
    const float loopNum = 5;
    float4 gaussianBlur = GaussianBlur(tex, input.uv, sigma, loopNum);
    gaussianBlur.a = 1;
    return gaussianBlur;
}