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
    
    float4 normal = float4(Input.Normal, 0.0f);
    output.NormalWorld = normalize(mul(normal, CBWorldInvTransposedMat).xyz);
    
    return output;
}