#include "Silhouette.hlsli"

V2P main(Appdata i)
{
    V2P o;
    o.svpos = mul(mul(vpMat, wMat), i.pos);
    return o;
}