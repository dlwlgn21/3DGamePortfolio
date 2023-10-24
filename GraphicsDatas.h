#pragma once
#include <directxtk/SimpleMath.h>

static constexpr const UINT MAX_LIGHTS = 3;
static constexpr const UINT SIMD_ALIGN_SIZE = 16;
static constexpr const UINT TRANSFORM_CONSTANT_BUFFER_SLOT = 0;
static constexpr const UINT LIGHTING_CONSTANT_BUFFER_SLOT = 1;

namespace jh::graphics
{
	struct Vertex2D
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector2 UV;
	};

	struct Vertex3D
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector3 Normal;
		DirectX::SimpleMath::Vector2 UV;
		DirectX::SimpleMath::Vector3 TangentModel;
	};

	struct Vertex3DCollider
	{
		DirectX::SimpleMath::Vector3 Pos;
	};

	struct SkinnedVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector3 Normal;
		DirectX::SimpleMath::Vector2 UV;
		DirectX::SimpleMath::Vector3 TangentModel;

		float BlendWeights[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; // ���ؽ��� ���� �����ӿ� ������ �޴� ����ġ.
		uint8_t BoneIndicies[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };  // ���ؽ��� ������ �޴� ���� �ε���

		// ĳ���� �ϳ��� ǥ���ϴ� Bone�� ���� �ִ� 256����� ����. �׷��� uint8_t. ���� 5~60�� ������.
		// fbx���Ͽ��� �̷� ������ ������ �Ǿ��־ �װ� �о���� ������ ������ �ؾ���.
	};


	struct alignas(SIMD_ALIGN_SIZE) GrapicsMaterial
	{
		DirectX::SimpleMath::Vector3  MaterialAmbient;
		float						  MaterialShininess;
		DirectX::SimpleMath::Vector4  MaterialDiffuse;
		DirectX::SimpleMath::Vector4  MaterialSpecular;
	};

	struct alignas(SIMD_ALIGN_SIZE) GrapicsLight
	{
		DirectX::SimpleMath::Vector3	LightStrengh;
		float							LightFallOffStart;
		DirectX::SimpleMath::Vector3	LightDirection;
		float							LightFallOffEnd;
		DirectX::SimpleMath::Vector3	LightPosition;
		float							LightSpotPower;
	};

	struct alignas(SIMD_ALIGN_SIZE) TransformConstantCPUBuffer
	{
		DirectX::SimpleMath::Matrix WorldMat;
		DirectX::SimpleMath::Matrix WolrdInvTransposedMat;
		DirectX::SimpleMath::Matrix ViewMat;
		DirectX::SimpleMath::Matrix ProjectionMat;
	};

	struct alignas(SIMD_ALIGN_SIZE) LighthingConstantCPUBuffer
	{
		DirectX::SimpleMath::Vector4	CBEyeWorld;
		int								CBIsUseDiffuseTexture;
		int								CBIsUseAmbientTexture;
		int								CBIsUseSpecularTexture;
		int								CBIsUseNormalTexture;
		GrapicsMaterial					CBMaterial;
		GrapicsLight					CBLight[MAX_LIGHTS];
	};

	enum class eCBType
	{
		TRANSFORM,
		LIGHTING,
		COUNT
	};

	enum class eShaderStage
	{
		VS,
		DS,
		HS,
		GS,
		PS,
		CS,
		COUNT
	};

	enum class eTextureType
	{
		DIFFUSE,
		AMBIENT,
		SPECULAR,
		NORMAL,
		COUNT
	};
}