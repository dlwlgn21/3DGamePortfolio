struct PixelInput
{
    float4 Position : SV_Position;
    float3 PositionWorld : POSITION;
    float3 NormalWorld : NORMAL;
    float2 UV : TEXCOORD0;
};


TextureCube CubeTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(PixelInput Input) : SV_TARGET
{
    // 주의: 텍스춰 좌표가 float3 입니다.
    return CubeTexture.Sample(Sampler, Input.PositionWorld.xyz);
}