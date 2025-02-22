#include "Basic.hlsli"

[maxvertexcount(3)]
void main(
	triangle VSOutput input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.svpos = input[i].svpos;
        element.normal = input[i].normal;
        element.uv = input[i].uv;
        element.worldpos = input[i].worldpos;
		output.Append(element);
	}
}