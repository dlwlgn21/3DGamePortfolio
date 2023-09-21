struct DebugVertexInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct DebugVertexOutput
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
};

cbuffer DebugTransformBuffer : register(b0)
{
    float4x4 ViewMat;
    float4x4 ProjectionMat;
}

DebugVertexOutput main(DebugVertexInput Input)
{
    DebugVertexOutput Output;
    Output.Position = mul(float4(Input.Position, 1.0f), ViewMat);
    Output.Position = mul(Output.Position, ProjectionMat);
    Output.Color = Input.Color;
    return Output;
}