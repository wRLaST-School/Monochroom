cbuffer cbuff0 : register(b0)
{
	matrix wMat;
    matrix vpMat;
}

struct Appdata
{
    float4 pos : POSITION;
    float4 normal : NORMAL;
};

struct V2P
{
    float4 svpos : SV_POSITION;
    float4 normal : NORMAL;
};