struct SkinnedVertexInput
{
    float3 PositionModel : POSITION;
    float3 NormalModel : NORMAL;
    float2 UV : TEXCOORD0;
    float3 TangentModel : TANGENT0;
    
    float4 BoneWeights0 : BLENDWEIGHT0;
    float4 BoneWeights1 : BLENDWEIGHT1;
    uint4  BoneIndices0 : BLENDINDICES0;
    uint4  BoneIndices1 : BLENDINDICES1;
};

struct SkinnedVertexOutput
{
    //float4 PositionProjection : SV_Position;
    //float3 PositionWorld : POSITION;
    //float3 NormalWorld : NORMAL;
    //float2 UV : TEXCOORD0;
    //float3 TangentWorld : TANGENT0;
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

StructuredBuffer<float4x4> BoneTransforms : register(t0);

SkinnedVertexOutput main(SkinnedVertexInput Input)
{
    SkinnedVertexOutput output;
    // Animation Part START
    float weights[8];
    weights[0] = Input.BoneWeights0.x;
    weights[1] = Input.BoneWeights0.y;
    weights[2] = Input.BoneWeights0.z;
    weights[3] = Input.BoneWeights0.w;
    weights[4] = Input.BoneWeights1.x;
    weights[5] = Input.BoneWeights1.y;
    weights[6] = Input.BoneWeights1.z;
    weights[7] = Input.BoneWeights1.w;
    
    uint indices[8];
    indices[0] = Input.BoneIndices0.x;
    indices[1] = Input.BoneIndices0.y;
    indices[2] = Input.BoneIndices0.z;
    indices[3] = Input.BoneIndices0.w;
    indices[4] = Input.BoneIndices1.x;
    indices[5] = Input.BoneIndices1.y;
    indices[6] = Input.BoneIndices1.z;
    indices[7] = Input.BoneIndices1.w;
    
    float3 PositionModel = float3(0.0f, 0.0f, 0.0f);
    float3 NormalModel = float3(0.0f, 0.0f, 0.0f);
    float3 TangentModel = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 8; ++i)
    {
        PositionModel +=    weights[i] * mul(float4(Input.PositionModel, 1.0f), BoneTransforms[indices[i]]).xyz;
        NormalModel +=      weights[i] * mul(Input.NormalModel, (float3x3) BoneTransforms[indices[i]]);
        TangentModel +=     weights[i] * mul(Input.TangentModel, (float3x3) BoneTransforms[indices[i]]);
    }
    // Animation Part END
    
    float4 worldPos = mul(float4(PositionModel, 1.0), CBWorldMat);
    float4 clipPos = worldPos;
    output.PositionWorld = worldPos.xyz;
    
    float4 viewPos = mul(worldPos, CBViewMat);
    float4 projectionPos = mul(viewPos, CBProjectionMat);
    output.PositionProjection = projectionPos;
    
    clipPos = mul(clipPos, CBLightViewMat);
    clipPos = mul(clipPos, CBLightProjectionMat);
    output.ClipPosition = clipPos;
    
    output.UV = Input.UV;
    
    float4 normal = float4(NormalModel, 0.0f);
    output.NormalWorld = normalize(mul(normal, CBWorldInvTransposedMat).xyz);
    
    float3 tangentWorld = mul(float4(TangentModel, 0.0), CBWorldMat).xyz;
    output.TangentWorld = tangentWorld.xyz;
    return output;
}