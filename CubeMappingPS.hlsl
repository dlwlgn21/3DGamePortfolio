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
    // ����: �ؽ��� ��ǥ�� float3 �Դϴ�.
    return CubeTexture.Sample(Sampler, Input.PositionWorld.xyz);
}