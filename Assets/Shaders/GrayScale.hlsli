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
float2 GetNearEffectPos(float2 uv)
{    
    float2 dir1 = grayPoint1 - uv;
    float2 dir2 = grayPoint2 - uv;
    
    // 長さを計算
    float len1 = length(dir1);
    float len2 = length(dir2);
    
    // 短い方のベクトルを選択(stepはxがy以上だったら1返す)
    // 短い方のベクトルを選択
    float factor = (len1 < len2) ? 1.0f : 0.0f;
    float2 result = factor * dir1 + (1.0f - factor) * dir2;
    
    
    return uv + result;
}

float2 GetDirToNearEffectPos(float2 uv, float maxLength)
{
    float2 nearEffectPos = GetNearEffectPos(uv);
    
    float2 result = nearEffectPos - uv;
    
    // 楕円形のスケーリングファクタ
    float2 ellipseScale = float2(0.8f, 2.4f); // 横に1.5倍、縦にそのまま

    // 距離を楕円形のスケールに基づいて調整
    float2 adjustedResult = result * (ellipseScale + ellipseScale * maxLength);
    float adjustedDistance = length(adjustedResult);

    // 長さの上限に収まるようにする
    adjustedResult *= min(maxLength / adjustedDistance, 1.0f);

    // スケーリングを元に戻す
    adjustedResult /= ellipseScale;

    return adjustedResult;
}