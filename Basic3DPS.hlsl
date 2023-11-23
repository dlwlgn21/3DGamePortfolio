#include "PSCommon.hlsli"

float4 main(PixelInput Input) : SV_TARGET0
{
    float3 toEyeDirVec = normalize(CBEyeWorld.xyz - Input.PositionWorld);
    
    float3 color = float3(0.0, 0.0, 0.0);
    
    int i = 0;
    
    float3 normalWorld = GetNormal(Input);
    
    //[unroll]
    //for (i = 0; i < NUM_DIR_LIGHTS; ++i)
    //{
    //    color += ComputeDirectionalLight(CBLight[i], CBMaterial, normalWorld, toEyeDirVec);
    //}
    
    [unroll]
    for (i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; ++i)
    {
        color += ComputePointLight(CBLight[i], CBMaterial, Input.PositionWorld, normalWorld, toEyeDirVec);
    }
    
    //[unroll]
    //for (i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; ++i)
    //{
    //    color += ComputeSpotLight(CBLight[i], CBMaterial, Input.PositionWorld, normalWorld, toEyeDirVec);
    //}
    
    //return float4(color, 1.0);
    //return float4(1.0, 1.0, 1.0, 1.0);
    
    float shadowFactor = 1.0;
    const float nearZ = 1.0; // 카메라 설정과 동일
        
    float currentDepth = Input.ClipPosition.z / Input.ClipPosition.w;
    
    // 좌표계 변환 For Sampling from ShadowMap
    float2 uv = Input.ClipPosition.xy / Input.ClipPosition.w;
    uv.y = -uv.y;
    uv = uv * 0.5 + 0.5;
    float shadowDepth = ShadowMap.Sample(ShadowSampler, uv).r;

    if (currentDepth > shadowDepth + 0.00125f)
    {
        shadowFactor = 0.5;
    }
    color *= shadowFactor;
    
    float4 diffuse = DiffuseCube.Sample(PointSampler, normalWorld);
    diffuse.rgb *= CBMaterial.MaterialDiffuse.xyz * color;;
    
    float4 specular = SpecularCube.Sample(PointSampler, reflect(-toEyeDirVec, normalWorld));
    
    specular *= pow((specular.r + specular.g + specular.b) / 3.0f, CBMaterial.MaterialShininess);
    
    
    specular.xyz *= CBMaterial.MaterialSpecular.xyz;
    
    if (CBIsUseDiffuseTexture)
    {
        diffuse.xyz *= DiffuseTexture.Sample(PointSampler, Input.UV).xyz;
    }
    
    return diffuse + specular;
    
    //return CBIsUseDiffuseTexture == 1 ? float4(color, 1.0) * DiffuseTexture.Sample(PointSampler, Input.UV) : float4(color, 1.0);
    //return float4(Input.Color, 1.0);
    
}
