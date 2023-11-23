struct PixelInput
{
    float4 PositionProjection : SV_Position;
    float3 PositionWorld : POSITION;
    float4 ClipPosition : TEXCOORD0;
    float3 NormalWorld : NORMAL;
    float2 UV : TEXCOORD1;
    float3 TangentWorld : TANGENT0;
};

float4 main(PixelInput Input) : SV_TARGET0
{
    return Input.UV.x < 0.9f ? float4(Input.NormalWorld.rgb, 1.0f) : float4(1.0f, 1.0f, 0.0f, 1.0f);
}