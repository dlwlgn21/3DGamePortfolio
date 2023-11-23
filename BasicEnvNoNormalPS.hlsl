#include "PSCommon.hlsli"

float4 main(PixelInput Input) : SV_TARGET0
{
    float3 toEyeDirVec = normalize(CBEyeWorld.xyz - Input.PositionWorld);
    
    float3 color = float3(0.0, 0.0, 0.0);
    
    int i = 0;
    
    //[unroll]
    //for (i = 0; i < NUM_DIR_LIGHTS; ++i)
    //{
    //    color += ComputeDirectionalLight(CBLight[i], CBMaterial, normalWorld, toEyeDirVec);
    //}
    
    [unroll]
    for (i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; ++i)
    {
        color += ComputePointLight(CBLight[i], CBMaterial, Input.PositionWorld, Input.NormalWorld, toEyeDirVec);
    }
    
    //[unroll]
    //for (i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; ++i)
    //{
    //    color += ComputeSpotLight(CBLight[i], CBMaterial, Input.PositionWorld, normalWorld, toEyeDirVec);
    //}
    
    //return float4(color, 1.0);
    //return float4(1.0, 1.0, 1.0, 1.0);
        
    //float4 diffuse = DiffuseCube.Sample(PointSampler, Input.NormalWorld);
    //diffuse.rgb *= CBMaterial.MaterialDiffuse;
    
    //float4 specular = SpecularCube.Sample(PointSampler, reflect(-toEyeDirVec, Input.NormalWorld));
    
    //specular *= pow((specular.r + specular.g + specular.b) / 3.0f, CBMaterial.MaterialShininess);
    
    
    //specular.xyz *= CBMaterial.MaterialSpecular;
    
    //if (CBIsUseDiffuseTexture)
    //{
    //    diffuse.xyz *= DiffuseTexture.Sample(PointSampler, Input.UV);
    //}
    
    //float shadowFactor = 1.0;
    //const float nearZ = 0.1; // 카메라 설정과 동일
        
    //// 1. Project posWorld to light screen. 광원투영 공간의 NDC로 프로젝션 해버림.
    //// light.viewProj 사용
    //float4 lightScreen = mul(float4(Input.PositionWorld, 1.0), CBLightViewProjectionMatrix);
    //lightScreen.xyz /= lightScreen.w;
    //float currentDepth = lightScreen.z;
        
    //// 2. 카메라(광원)에서 볼 때의 텍스춰 좌표 계산
    //// [-1, 1]x[-1, 1] -> [0, 1]x[0, 1]
    //// 주의: 텍스춰 좌표와 NDC는 y가 반대
    //float2 lightTexcoord = float2(lightScreen.x, -lightScreen.y);
    //lightTexcoord += 1.0;
    //lightTexcoord *= 0.5;
    //// 3. 쉐도우맵에서 값 가져오기
    //float shadowDepth = ShadowMap.Sample(ShadowSampler, lightTexcoord).r;
    //// 4. 가려져 있다면 그림자로 표시
    //// 힌트: 작은 bias (0.001 정도) 필요
    //if (shadowDepth + 0.00125 < currentDepth)
    //{
    //    shadowFactor = 0.0;
    //}
    //color *= shadowFactor;
    
    
    //return diffuse + specular;
    return CBIsUseDiffuseTexture == 1 ? float4(color, 1.0) * DiffuseTexture.Sample(PointSampler, Input.UV) : float4(color, 1.0);
    //return float4(Input.Color, 1.0);
    
}
