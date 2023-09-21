struct PixelInput
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD0;
};

cbuffer AnimationBuffer : register(b1)
{
    float2  LeftTop;
    float2  Size;
    float2  Offset;
    float2  AtlasImageSize;
    uint    IsFlip;
}

SamplerState PointSampler : register(s0);
Texture2D NonAnimTexture : register(t0);
Texture2D AtlasTexture : register(t1);

float4 main(PixelInput Input) : SV_TARGET
{
    float4 color;
    float2 diff = (AtlasImageSize - Size) * 0.5f;
    float2 uv = (LeftTop - diff - Offset) + (AtlasImageSize * Input.UV);
    if (uv.x < LeftTop.x            || 
        uv.y < LeftTop.y            || 
        uv.x > LeftTop.x + Size.x   || 
        uv.y > LeftTop.y + Size.y)
    {
        discard;
    }
    color = AtlasTexture.Sample(PointSampler, uv);
    if (color.w == 0.0f)
    {
        discard;
    }
    return color;
}
