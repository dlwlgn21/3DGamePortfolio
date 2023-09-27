struct PixelInput
{
    float4 Position : SV_Position;
    float3 PositionWorld : POSITION;
    float3 NormalWorld : NORMAL;
    float2 UV : TEXCOORD0;
};

float4 main(PixelInput Input) : SV_TARGET
{
    return Input.UV.x < 0.9f ? float4(Input.NormalWorld.rgb, 1.0f) : float4(1.0f, 1.0f, 0.0f, 1.0f);
}