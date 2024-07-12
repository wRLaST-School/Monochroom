cbuffer cbuff0 : register(b0)
{
    float2 shiftR;
    float2 shiftG;
    float2 shiftB;
    float timer;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

const static float2 screenSize = float2(1920.f, 1080.f);
