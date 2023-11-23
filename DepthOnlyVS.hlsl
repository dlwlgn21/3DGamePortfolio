#include "VSCommon.hlsli"
struct DepthOnlyVSOutput
{
    float4 Position : SV_Position;
    float4 ClipPosition : TEXCOORD0;
};

DepthOnlyVSOutput main(VertexInput Input)
{
    DepthOnlyVSOutput output;
    float4 clipPos = mul(float4(Input.Position, 1.0f), CBWorldMat);
    clipPos = mul(clipPos, CBLightViewMat);
    clipPos = mul(clipPos, CBLightProjectionMat);
    output.ClipPosition = clipPos;
    output.Position = output.ClipPosition;
    return output;
}