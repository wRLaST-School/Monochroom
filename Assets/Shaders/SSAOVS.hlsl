#include "SSAO.hlsli"

float4 ComputeScreenPos(float4 clipPos);

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.svpos = pos;
    output.uv = uv;
    
    float4 screenPos = ComputeScreenPos(output.svpos);
    float4 ndcPos = (screenPos / screenPos.w) * 2 - 1;
    float3 clipVec = normalize(float3(ndcPos.xy, 1.0f) * 1000.f);
    clipVec.y *= -1;
    output.viewVec = mul(invProjMat, clipVec.xyzz).xyz;
    return output;
}

float4 ComputeScreenPos(float4 clipPos)
{
    // �N���b�v�X�y�[�X�ʒu��NDC�i���K���f�o�C�X���W�j�ɕϊ�
    float3 ndcPos = clipPos.xyz / clipPos.w;

    // NDC��[0, 1]�͈̔͂ɕϊ�
    float2 texCoords;
    texCoords.x = (ndcPos.x * 0.5f) + 0.5f;
    texCoords.y = (ndcPos.y * -0.5f) + 0.5f; // Y���W�͋t����

    return float4(texCoords, 0.0f, 1.0f);
}