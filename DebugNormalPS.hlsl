struct DebugVertexOutput
{
    float4 PositionProjection : SV_Position;
    float3 PositionWorld : POSITION;
    float3 NormalWorld : NORMAL;
    float2 UV : TEXCOORD0;
    float3 Color : COLOR;
};

float4 main(DebugVertexOutput Input) : SV_TARGET
{
    return float4(Input.Color, 1.0f);
}