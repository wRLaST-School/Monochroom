#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct VS_INPUT
{
    float3 Position;
    float2 UV;
    float4 Color;
};

struct VS_OUTPUT
{
    float4 Position;
    float2 UV;
    float4 Color;
};

struct compute
{
    float _data[1];
};

struct main0_out
{
    float2 _entryPointOutput_UV [[user(locn0)]];
    float4 _entryPointOutput_Color [[user(locn1)]];
    float4 gl_Position [[position]];
};

struct main0_in
{
    float3 input_Position [[attribute(0)]];
    float2 input_UV [[attribute(1)]];
    float4 input_Color [[attribute(2)]];
};

static inline __attribute__((always_inline))
VS_OUTPUT _main(thread const VS_INPUT& _input, const device compute& compute0)
{
    VS_OUTPUT _output;
    _output.Position = float4(_input.Position, 1.0) + float4(compute0._data[0]);
    _output.UV = _input.UV;
    _output.Color = _input.Color;
    return _output;
}

vertex main0_out main0(main0_in in [[stage_in]], const device compute& compute0 [[buffer(1)]])
{
    main0_out out = {};
    VS_INPUT _input;
    _input.Position = in.input_Position;
    _input.UV = in.input_UV;
    _input.Color = in.input_Color;
    VS_INPUT param = _input;
    VS_OUTPUT flattenTemp = _main(param, compute0);
    out.gl_Position = flattenTemp.Position;
    out._entryPointOutput_UV = flattenTemp.UV;
    out._entryPointOutput_Color = flattenTemp.Color;
    return out;
}

