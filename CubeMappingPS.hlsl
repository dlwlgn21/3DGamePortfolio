struct PixelInput
{
    float4 Position : SV_Position;
    float3 PositionWorld : POSITION;
};

TextureCube DiffuseCube : register(t0);
TextureCube SpecularCube : register(t1);
SamplerState Sampler : register(s0);

float4 main(PixelInput Input) : SV_TARGET0
{
    // ����: �ؽ��� ��ǥ�� float3 �Դϴ�.
    return SpecularCube.Sample(Sampler, Input.PositionWorld.xyz);
}