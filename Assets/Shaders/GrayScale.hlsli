cbuffer cbuff0 : register(b0)
{
    float2 offset;
    float2 grayPoint1;
    float2 grayPoint2;
    float grayEffectRadius;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};


//---------------

float2 GetDirToNearEffectPos(float2 uv, float maxLength)
{
    float2 dir1 = grayPoint1 - uv;
    float2 dir2 = grayPoint2 - uv;
    
    // �������v�Z
    float len1 = length(dir1);
    float len2 = length(dir2);
    
    // �Z�����̃x�N�g����I��(step��x��y�ȏゾ������1�Ԃ�)
    // �Z�����̃x�N�g����I��
    float factor = (len1 < len2) ? 1.0f : 0.0f;
    float2 result = factor * dir1 + (1.0f - factor) * dir2;
    
    //�����̏���Ɏ��܂�悤�ɂ���
    result *= min(maxLength / length(result), 1.0f);
    
    return result;

}