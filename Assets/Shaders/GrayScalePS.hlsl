#include "GrayScale.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> texMask : register(t1);
SamplerState smp : register(s0);

static const float4 kGoggleFrameColor = { 0, 0, 0, 1 };
static const float4 kGoggleLensColor = { 0.2125f, 0.7154f, 0.0721f, 1 };

static const float kPerliningNearLength= 0.09f;

float4 main(VSOutput input) : SV_TARGET
{
    float2 maskPos = input.uv - offset;
    maskPos.y = min(maskPos.y, 1.0f);
    
    float2 dirEffectPos= GetDirToNearEffectPos(maskPos, grayEffectRadius);
    
    //�~���L����悤�ȉ��o
    maskPos += dirEffectPos;    
    
    float2 nearPos= GetNearEffectPos(input.uv);
    float lengthOfEffectPos=length(nearPos - maskPos) * dot(nearPos - maskPos, dirEffectPos);
    //�����傤��effectPos�͈̔͂ɓ������������m�C�Y�����邽�߂Ɋ������o��
    float lengthA = lengthOfEffectPos / kPerliningNearLength;
    int lengthI;
    float lengthF=modf(lengthA,lengthI);
    //1.0�����Ă���0�ɂȂ�悤��
    lengthA = max(lengthF - max(lengthA - lengthF,0) ,0);
    lengthA = (lengthA != 0.0f) * 1.0f - lengthA;
    lengthA= pow(lengthA,1.2f);
    
    
    float4 color = tex.Sample(smp, input.uv);
    //�S�[�O���̃t���[����
    float goggleFrameRate = texMask.Sample(smp, maskPos).r;
    //�S�[�O���̃����Y��
    float goggleLensRate = texMask.Sample(smp, maskPos).b;
    
    //�m�C�Y
    float4 noise = ( /*pow(mainImage(maskPos, 0.7f), 0.3f) * */lengthA.xxxx);
    
    //�S�[�O���̐F�i�����Y���t���[�����j
    //�O���[�X�P�[���̐F
    float grayScale = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    color = lerp(color, grayScale.xxxx, goggleLensRate.xxxx);
    color = lerp(color, kGoggleFrameColor,max( goggleFrameRate.xxxx - noise,0));
    
    color.a = 1.f;

    return color;
}