cbuffer cbuff0 : register(b0)
{
    float4x4 projMat;
    float4x4 invProjMat;
    //float4 weight[2];
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 viewVec : TEXCOORD1;
};

const static float2 screenSize = float2(1920.f, 1080.f);
