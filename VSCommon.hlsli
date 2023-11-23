struct VertexInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
    float3 TangentModel : TANGENT0;
};

struct VertexOutput
{
    float4 PositionProjection : SV_Position;
    float3 PositionWorld : POSITION;
    float4 ClipPosition : TEXCOORD0;
    float3 NormalWorld : NORMAL;
    float2 UV : TEXCOORD1;
    float3 TangentWorld : TANGENT0;
};

cbuffer TransformBuffer : register(b0)
{
    matrix CBWorldMat;
    matrix CBWorldInvTransposedMat;
    matrix CBViewMat;
    matrix CBProjectionMat;
}

cbuffer ShadowBuffer : register(b2)
{
    matrix CBLightViewMat;
    matrix CBLightProjectionMat;
}