struct DepthOnlyVSOutput
{
    float4 Position : SV_Position;
    float4 ClipPosition : TEXCOORD0;
};
float4 main(DepthOnlyVSOutput Input) : SV_Target0
{
    float depth = Input.ClipPosition.z / Input.ClipPosition.w;
    return float4(depth.xxx, 1);
}