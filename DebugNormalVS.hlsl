#include "VSCommon.hlsli"

struct DebugVertexOutput
{
    float4 PositionProjection   : SV_Position;
    float3 PositionWorld        : POSITION;
    float3 NormalWorld          : NORMAL;
    float2 UV                   : TEXCOORD0;
    float3 Color                : COLOR;
};

DebugVertexOutput main(VertexInput Input)
{
    DebugVertexOutput output;
    float4 pos = float4(Input.Position, 1.0f);

    // Normal 먼저 변환
    float4 normal = float4(Input.Normal, 0.0f);
    output.NormalWorld = normalize(mul(normal, CBWorldInvTransposedMat).xyz);
    
    pos = mul(pos, CBWorldMat);
    
    float t = Input.UV.x;
    
    pos.xyz += output.NormalWorld * t;

    output.PositionWorld = pos.xyz;
    
    pos = mul(pos, CBViewMat);
    pos = mul(pos, CBProjectionMat);

    output.PositionProjection = pos;
    output.UV = Input.UV;
    
    output.Color = float3(1.0, 1.0, 0.0) * (1.0 - t) + float3(1.0, 0.0, 0.0) * t;
    return output;
}
