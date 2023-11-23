struct VertexInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
    float3 TangentModel : TANGENT0;
};

struct SkinnedVertexInput
{
    float3 PositionModel : POSITION;
    float3 NormalModel : NORMAL;
    float2 UV : TEXCOORD0;
    float3 TangentModel : TANGENT0;
    
    float4 BoneWeights0 : BLENDWEIGHT0;
    float4 BoneWeights1 : BLENDWEIGHT1;
    uint4 BoneIndices0 : BLENDINDICES0;
    uint4 BoneIndices1 : BLENDINDICES1;
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