struct PixelInput
{
    float4 Position : SV_Position;
    float3 Color : COLOR;
};

float4 main(PixelInput Input) : SV_TARGET0
{
    return float4(Input.Color, 1.0f);
}