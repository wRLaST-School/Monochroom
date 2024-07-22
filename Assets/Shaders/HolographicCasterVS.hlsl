#include "HolographicCaster.hlsli"

V2P main(Appdata input)
{
	float4 wnormal = normalize(mul(wMat, float4(input.normal, 0)));
    float4 wpos = mul(wMat, input.pos);

    V2P output;
    output.svpos = mul(vpMat, wpos);
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = input.uv;

	return output;
}