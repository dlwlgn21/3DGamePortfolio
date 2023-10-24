struct PixelInput
{
    float4 PositionProjection : SV_Position;
};

float4 main(PixelInput Input) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}