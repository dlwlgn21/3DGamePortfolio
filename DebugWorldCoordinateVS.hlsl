#include "VSCommon.hlsli"

VertexOutput main(VertexInput Input)
{
    VertexOutput output;
    float4 worldPos = mul(float4(Input.Position, 1.0), CBWorldMat);
    output.PositionWorld = worldPos.xyz;
    
    float4 viewPos = mul(worldPos, CBViewMat);
    float4 projectionPos = mul(viewPos, CBProjectionMat);
    output.PositionProjection = projectionPos;
    
    output.UV = Input.UV;
    
    // Normal == Color
    output.NormalWorld = Input.Normal;
    output.TangentWorld = float3(0.0, 0.0, 0.0);
    return output;
}