struct PixelInput
{
    float4 Position : SV_Position;
    float3 PositionWorld : POSITION;
    float3 NormalWorld : NORMAL;
    float2 UV : TEXCOORD0;
    float3 TangentWorld : TANGENT0;
};

#define MAX_LIGHTS          (3)
#define NUM_DIR_LIGHTS      (1)
#define NUM_POINT_LIGHTS    (1)
#define NUM_SPOT_LIGHTS     (1)

SamplerState PointSampler : register(s0);
Texture2D DiffuseTexture : register(t0);
Texture2D AmbientTexture : register(t1);
Texture2D SpecularTexture : register(t2);
Texture2D NormalTexture : register(t3);

struct Material
{
    float3  MaterialAmbient;
    float   MaterialShininess;
    float4  MaterialDiffuse;
    float4  MaterialSpecular;
};

struct Light
{
    float3  LightStrengh;
    float   LightFallOffStart;
    float3  LightDirection;
    float   LightFallOffEnd;
    float3  LightPosition;
    float   LightSpotPower;
};

cbuffer LighthingConstantBuffer : register(b1)
{
    float4          CBEyeWorld;
    int             CBIsUseDiffuseTexture;
    int             CBIsUseAmbientTexture;
    int             CBIsUseSpecularTexture;
    int             CBIsUseNormalTexture;
    Material        CBMaterial;
    Light           CBLight[MAX_LIGHTS];
}


float CalculateAttenuation(float d, float falloffStart, float falloffEnd)
{
    // Linear falloff
    return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}

float3 BlinnPhong(float3 lightStrength, float3 toLightDirVector, float3 normalWorldVec, float3 toEyeDirVec, Material material)
{
    float3 halfwayVec = normalize(toEyeDirVec + toLightDirVector);
    float HdotN = dot(halfwayVec, normalWorldVec);
    float3 specular = material.MaterialSpecular.xyz * pow(max(HdotN, 0.0f), material.MaterialShininess);
    return (material.MaterialDiffuse.xyz + specular) * lightStrength + material.MaterialAmbient;
}

float3 ComputeDirectionalLight(Light light, Material mateirial, float3 normalVec, float3 toEyeDirVec)
{
    float3 toLightDirVec = -light.LightDirection;
    float NdotL = max(dot(toLightDirVec, normalVec), 0.0f);
    float3 lightStrength = light.LightStrengh * NdotL;
    return BlinnPhong(lightStrength, toLightDirVec, normalVec, toEyeDirVec, mateirial);
}

float3 ComputePointLight(Light light, Material mateirial, float3 posVec, float3 normalVec, float3 toEyeVec)
{
    float3 toLightVec = light.LightPosition - posVec;
    float d = length(toLightVec);
    
    // Too far NO Light
    if (d > light.LightFallOffEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        toLightVec /= d;
        float NdotL = max(dot(toLightVec, normalVec), 0.0f);
        float3 lightStrength = light.LightStrengh * NdotL;
        float att = CalculateAttenuation(d, light.LightFallOffStart, light.LightFallOffEnd);
        lightStrength *= att;
        return BlinnPhong(lightStrength, toLightVec, normalVec, toEyeVec, mateirial);
    }
}

float3 ComputeSpotLight(Light light, Material mateirial, float3 posVec, float3 normalVec, float3 toEyeVec)
{
    float3 toLightVec = light.LightPosition - posVec;
    float d = length(toLightVec);
    
    // Too far NO Light
    if (d > light.LightFallOffEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        toLightVec /= d;
        float NdotL = max(dot(toLightVec, normalVec), 0.0f);
        float3 lightStrength = light.LightStrengh * NdotL;
        float att = CalculateAttenuation(d, light.LightFallOffStart, light.LightFallOffEnd);
        lightStrength *= att;
        
        float spotFactor = pow(max(-dot(toLightVec, light.LightDirection), 0.0f), light.LightSpotPower);
        lightStrength *= spotFactor;
        return BlinnPhong(lightStrength, toLightVec, normalVec, toEyeVec, mateirial);
    }
}

float3 GetNormal(PixelInput Input)
{
    float3 normalWorld = Input.NormalWorld;
    
    if (CBIsUseNormalTexture == 1) // NormalWorld를 교체
    {
        float3 normal = NormalTexture.Sample(PointSampler, Input.UV).rgb;
        normal = 2.0 * normal - 1.0; // 범위 조절 [-1.0, 1.0]

        // OpenGL 용 노멀맵일 경우에는 y 방향을 뒤집어줍니다.
        // normal.y = invertNormalMapY ? -normal.y : normal.y;
        
        float3 N = normalWorld;
        float3 T = normalize(Input.TangentWorld - dot(Input.TangentWorld, N) * N);
        float3 B = cross(N, T);
        
        // matrix는 float4x4, 여기서는 벡터 변환용이라서 3x3 사용
        float3x3 TBN = float3x3(T, B, N);
        normalWorld = normalize(mul(normal, TBN));
    }
    
    return normalWorld;
}


float4 main(PixelInput Input) : SV_TARGET
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
    return CBIsUseDiffuseTexture == 1 ? float4(color, 1.0) * DiffuseTexture.Sample(PointSampler, Input.UV) : float4(color, 1.0);
    //return float4(Input.Color, 1.0);
    
}
