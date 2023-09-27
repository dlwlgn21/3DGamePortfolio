struct VertexInput
{
    float3 Position : POSITION;
};

struct VertexOutput
{
    float4 PositionProjection : SV_Position;
    float3 PositionWorld : POSITION;
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
    float4 worldPos = mul(float4(Input.Position, 1.0), CBWorldMat); // Ientity
    output.PositionWorld = Input.Position;
    
    float4 viewPos = mul(worldPos, CBViewMat);
    float4 projectionPos = mul(viewPos, CBProjectionMat);
    output.PositionProjection = projectionPos;
    
    return output;
}