struct VertexInput
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

cbuffer TransformBuffer : register(b0)
{
    matrix WorldMat;
    matrix WorldInvTransposedMat;
    matrix ViewMat;
    matrix ProjectionMat;
}

VertexOutput main(VertexInput Input)
{
    VertexOutput output;
    float4 worldPos = mul(float4(Input.Position, 1.0), WorldMat);
    float4 viewPos = mul(worldPos, ViewMat);
    float4 projectionPos = mul(viewPos, ProjectionMat);
    output.Position = projectionPos;
    output.UV = Input.UV;
    return output;
}