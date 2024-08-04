cbuffer cbuff0 : register(b0)
{
    float2 offset;
    float2 grayPoint1;
    float2 grayPoint2;
    float grayEffectRadius;
    float time;
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
    float2 ellipseScale = float2(2.0f, 1.2f); // 横に1.5倍、縦にそのまま

    // 距離を楕円形のスケールに基づいて調整
    float2 adjustedResult = result * (ellipseScale + ellipseScale * maxLength);
    float adjustedDistance = length(adjustedResult);

    // 長さの上限に収まるようにする
    adjustedResult *= min(maxLength / adjustedDistance, 1.0f);

    // スケーリングを元に戻す
    adjustedResult /= ellipseScale;

    return adjustedResult;
}


//-----------------------------------
static const float2 kWindowSize={1280.0f,720};
static const float3 mixColor1 = float3(0.9, 0.45, 0.22);
static const float3 mixColor2 = float3(0.4, 0.85, 0.79);

// Get random value
float random(in float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

// Get noise
float noise(in float2 st)
{
    // Splited integer and float values.
    float2 i = floor(st);
    float2 f = frac(st);

    float a = random(i + float2(0.0, 0.0));
    float b = random(i + float2(1.0, 0.0));
    float c = random(i + float2(0.0, 1.0));
    float d = random(i + float2(1.0, 1.0));

    // -2.0f^3 + 3.0f^2
    float2 u = f * f * (3.0 - 2.0 * f);

    return lerp(a, b,u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

// fractional brown motion
//
// Reduce amplitude multiplied by 0.5, and frequency multiplied by 2.
float fbm(in float2 st)
{
    float v = 0.0;
    float a = 0.5;

    for (int i = 0; i < 3; i++)
    {
        v += a * noise(st);
        st = st * 2.0;
        a *= 0.5;
    }

    return v;
}

float4 mainImage(in float2 fragCoord ,float speed)
{
    // Calculate normalized UV values.
    float2 st = fragCoord /*/ kWindowSize.xy*/;

    float3 color = float3(4,4,4);

    float2 q = float2(0,0);
    q.x = fbm(st + float2(0,0));
    q.y = fbm(st + float2(1,0));

    // These numbers(such as 1.7, 9.2, etc.) are not special meaning.
    float2 r = float2(0,0);
    r.x = fbm(st + (4.0 * q) + float2(1.7, 9.2) + (0.15 * time * speed));
    r.y = fbm(st + (4.0 * q) + float2(8.3, 2.8) + (0.12 * time * speed));

    // Mixed color by 'q' and 'r'.
    color = lerp(color, mixColor1, clamp(length(q), 0.0, 1.0));
    color = lerp(color, mixColor2, clamp(length(r), 0.0, 1.0));

    // Calculate by 'r' is that getting domain warping.
    float f = fbm(st + 4.0 * r);

    // f^3 + 0.6f^2 + 0.5f
    float coef = (f * f * f + (0.6 * f * f) + (0.5 * f));
    color *= coef;

    return float4(color, 1.0);
}