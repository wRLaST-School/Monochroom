#include "BlinkTransition.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

static const float _EffectWaitTime = 0.0f;
static const float _EffectEdgePow = 2.7f;
static const float _EffectEdgeRatio = 0.1f;
static const float _EffectParabolaRate = 0.23f;

static const float2 _Tiling = float2(1.f, 1.f);

float easeInOutElastic(float t)
{
    const float c5 = (2 * 3.1415f) / 4.5;
                
    return t == 0
                  ? 0
                  : t == 1
                  ? 1
                  : t < 0.5
                  ? -(pow(2, 20 * t - 10) * sin((20 * t - 11.125) * c5)) / 2
                  : (pow(2, -20 * t + 10) * sin((20 * t - 11.125) * c5)) / 2 + 1;
}

float4 EyelidsColor(float2 uv, float time, float maxTime)
{
    const float HALF_MAX_TIME = (maxTime) / 2.0f;
    
                //どのくらいの範囲をぼかす（境界線）にするか
    const float EDGE_LENGTH = _Tiling.y * _EffectEdgeRatio;
    const float EDGE_CENTOR_X = _Tiling.x / 2.0f;
    const float EDGE_CENTOR_Y = _Tiling.y / 2.0f;
    
    float4 col = float4(1, 1, 1, 1);
    
    if (time <= maxTime + _EffectWaitTime)
    {
        float t = 0;
        
        t = time / maxTime;
        t = min(t, 1.0f);
        
                    //瞼なので上と下に分ける
        float isYHalfUp = 1.0f + -2.0f * step(EDGE_CENTOR_Y, uv.y); //0.5より大きかったらマイナス1.0
        float edgeY = (
                        (EDGE_CENTOR_Y - EDGE_CENTOR_Y * isYHalfUp)
                        + EDGE_CENTOR_Y * isYHalfUp * easeInOutElastic(t)
                    );
        
                    //放物線
        float parabolaLength = pow(( /*1 - */abs(uv.x - EDGE_CENTOR_X + 0.001f) / EDGE_CENTOR_X), _EffectEdgePow)
                        * -_EffectParabolaRate * isYHalfUp *
                        t /*edgeが残り続けないように*/
                        * (1 - t);
        
        float edgeYMin = edgeY + EDGE_LENGTH * -isYHalfUp - parabolaLength; //0.5以上ならプラス方向に
        
                    //ぼやかす範囲の中で
        float colT = max(isYHalfUp * (uv.y - edgeYMin), 0) / (EDGE_LENGTH) - (t);
       
        colT = min(colT, 1.0f);
        
        col = lerp(float4(0, 0, 0, 1), float4(1, 1, 1, 1), (colT));
    }
    
    return col;
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = tex.Sample(smp, input.uv);
    return col * EyelidsColor(input.uv, effectTime, effectTimeMax);
}