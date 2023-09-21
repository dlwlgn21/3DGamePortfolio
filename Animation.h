#pragma once
#include <directxtk/SimpleMath.h>

class Animator;
namespace jh::graphics
{


class Texture;
class Animation final
{
public:
	Animation();
	~Animation() = default;

	struct Sprite
	{
		DirectX::SimpleMath::Vector2 LeftTop;
		DirectX::SimpleMath::Vector2 Size;
		DirectX::SimpleMath::Vector2 Offset;
		DirectX::SimpleMath::Vector2 AtlasImageSize;
		float Duration;

		Sprite()
			: LeftTop(DirectX::SimpleMath::Vector2::Zero)
			, Size(DirectX::SimpleMath::Vector2::Zero)
			, Offset(DirectX::SimpleMath::Vector2::Zero)
			, AtlasImageSize(DirectX::SimpleMath::Vector2::Zero)
			, Duration(0.0f)
		{
		}
	};

	void Update();
	void UpdateConstantBuffer();

	void Create(
		const std::string& animKey,
		Texture& atalsImage,
		const DirectX::SimpleMath::Vector2& leftTop,
		const DirectX::SimpleMath::Vector2& seperatingSize,
		const DirectX::SimpleMath::Vector2& offset,
		const UINT spriteCount,
		const float duration,
		const float magnification
	);

	void CreateVerticalAnimation(
		const std::string& animKey,
		Texture& atlasImage,
		const DirectX::SimpleMath::Vector2& leftTop,
		const DirectX::SimpleMath::Vector2& seperatingSize,
		const DirectX::SimpleMath::Vector2& offset,
		const UINT spriteCount,
		const float duration,
		const float magnification
	);

	void Reset();

	__forceinline bool IsAnimComplete()   const { return mbIsAnimComplete; }
	const std::string& GetAnimationKey() const { return mAnimKey; }
	UINT GetCurrentIndex() const { return mIndex; }
	void SetHorizontalFlip(const bool isFlip) { mbIsHorizontalFlip = isFlip; }

private:
	std::string					mAnimKey;
	std::vector<Sprite>			mSpriteSheets;
	UINT						mIndex = 0;
	float						mAccumTimer = 0.0f;
	bool						mbIsAnimComplete = 0.0f;
	bool						mbIsHorizontalFlip = 0.0f;
};

}

