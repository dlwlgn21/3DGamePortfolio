struct DepthOnlyPixelShaderInput
{
    float4 PositionProjection : SV_POSITION;
};

float4 main(DepthOnlyPixelShaderInput Input : SV_POSITION) : SV_Target0
{
    return float4(1, 1, 1, 1);
}