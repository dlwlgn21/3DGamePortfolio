struct VertexInput
{
    float3 Position : POSITION;
};

struct VertexOutput
{
    float4 PositionProjection : SV_Position;
};

cbuffer TransformBuffer : register(b0)
{
    matrix CBWorldMat;
    matrix CBWorldInvTransposedMat;
    matrix CBViewMat;
    matrix CBProjectionMat;
}


VertexOutput main(VertexInput Input)
{
    VertexOutput output;
    float4 posProjection = mul(float4(Input.Position.xyz, 1.0), CBWorldMat);
    posProjection = mul(posProjection, CBViewMat);
    posProjection = mul(posProjection, CBProjectionMat);
    output.PositionProjection = posProjection;
    return output;
}