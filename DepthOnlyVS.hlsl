#include "VSCommon.hlsli"

float4 main(VertexInput Input) : SV_POSITION
{
    float4 pos = mul(float4(Input.Position, 1.0f), CBWorldMat);
    pos = mul(pos, CBViewMat);
    pos = mul(pos, CBProjectionMat);
    return pos;
}