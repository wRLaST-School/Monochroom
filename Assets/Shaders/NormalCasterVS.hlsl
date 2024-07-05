#include "NormalCaster.hlsli"

V2P main(Appdata i)
{
    float4x4 mat = mul(vpMat, wMat);
    
    V2P o;
    o.svpos = mul(mul(vpMat, wMat), i.pos);
    o.normal = mul(wMat, i.normal);
    //o.normal = i.normal;
    return o;
}