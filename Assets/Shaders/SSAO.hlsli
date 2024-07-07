cbuffer cbuff0 : register(b0)
{
    float4x4 invViewProjMat;
    //float4 weight[2];
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
    float2 viewVec : TEXCOORD1;
};