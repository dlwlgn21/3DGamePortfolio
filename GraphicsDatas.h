#pragma once
#include <directxtk/SimpleMath.h>

static constexpr const UINT SIMD_ALIGN_SIZE = 16;
static constexpr const UINT TRANSFORM_CONSTANT_BUFFER_SLOT = 0;
static constexpr const UINT ANIMATION_CONSTANT_BUFFER_SLOT = 1;
static constexpr const UINT DEBUG_TRANSFORM_CONSTANT_BUFFER_SLOT = 0;

namespace jh::graphics
{
	struct Vertex2D
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector2 UV;
	};

	struct Vertex3D
	{
		DirectX::SimpleMath::Vector4 Pos;
		DirectX::SimpleMath::Vector4 Color;
		DirectX::SimpleMath::Vector2 UV;
		DirectX::SimpleMath::Vector3 Tangent;
		DirectX::SimpleMath::Vector3 Binormal;
		DirectX::SimpleMath::Vector3 Normal;
	};

	struct Box2DVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector3 Color;
	};

	struct alignas(SIMD_ALIGN_SIZE) TransformConstantCPUBuffer
	{
		DirectX::SimpleMath::Matrix WorldMat;
		DirectX::SimpleMath::Matrix WolrdInvTransposedMat;
		DirectX::SimpleMath::Matrix ViewMat;
		DirectX::SimpleMath::Matrix ProjectionMat;
	};

	struct alignas(SIMD_ALIGN_SIZE) DebugTransformConstantCPUBuffer
	{
		DirectX::SimpleMath::Matrix ViewMat;
		DirectX::SimpleMath::Matrix ProjectionMat;
	};

	struct alignas(SIMD_ALIGN_SIZE) AnimationConstantCPUBuffer
	{
		DirectX::SimpleMath::Vector2 LeftTop;
		DirectX::SimpleMath::Vector2 Size;
		DirectX::SimpleMath::Vector2 Offset;
		DirectX::SimpleMath::Vector2 AtlasImageSize;
		UINT						 IsFlip = 0;
	};

	enum class eCBType
	{
		TRANSFORM,
		DEBUG_TRANSFORM,
		ANIMATION,
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
		NON_ANIMATION_TEXTURE,
		SPRITE_SHHET_TEXTURE,
		COUNT
	};
}