#include "Animation.h"
#include "Texture.h"
#include "Time.h"
#include "GraphicsPSOManager.h"
#include "Animator.h"

using namespace DirectX::SimpleMath;

static constexpr const UINT SPRITE_MAX_COUNT = 16;
static constexpr const UINT FLIPED = 1;
static constexpr const UINT NO_FLIPED = 0;

namespace jh::graphics
{

Animation::Animation()
{
	mSpriteSheets.reserve(SPRITE_MAX_COUNT);
}

void Animation::Update()
{
	if (mbIsAnimComplete) { return; }

	mAccumTimer += Time::DeltaTime();

	// 타이머가 해당 프레임의 유지시간보다 크면 다음 인덱스로 이동
	if (mAccumTimer >= mSpriteSheets[mIndex].Duration)
	{
		mAccumTimer = 0.0f;
		++mIndex;
		if (mIndex >= mSpriteSheets.size())
		{
			mbIsAnimComplete = true;

			// 이게 무슨이야기???? 방어코드라고 함.
			mIndex = static_cast<UINT>(mSpriteSheets.size() - 1);
		}
	}
}

void Animation::UpdateConstantBuffer()
{
	AnimationConstantCPUBuffer cpuBuffer;
	ZeroMemory(&cpuBuffer, sizeof(AnimationConstantCPUBuffer));
	cpuBuffer.LeftTop = mSpriteSheets[mIndex].LeftTop;
	cpuBuffer.Offset = mSpriteSheets[mIndex].Offset;
	cpuBuffer.Size = mSpriteSheets[mIndex].Size;
	cpuBuffer.AtlasImageSize = mSpriteSheets[mIndex].AtlasImageSize;
	if (mbIsHorizontalFlip)
	{
		cpuBuffer.IsFlip = FLIPED;
	}
	else
	{
		cpuBuffer.IsFlip = NO_FLIPED;
	}
	auto& animGPUBuffer = GraphicsPSOManager::GetInstance().GetConstantBuffer(eCBType::ANIMATION);
	animGPUBuffer.UpdateBuffer(&cpuBuffer);
}


void Animation::Create(const std::string& animKey, Texture& atalsImage, const DirectX::SimpleMath::Vector2& leftTop, const DirectX::SimpleMath::Vector2& seperatingSize, const DirectX::SimpleMath::Vector2& offset, const UINT spriteCount, const float duration, const float magnification)
{
	mAnimKey = animKey;
	float width = static_cast<float>(atalsImage.GetWidth());
	float height = static_cast<float>(atalsImage.GetHeight());

	for (UINT i = 0; i < spriteCount; ++i)
	{
		Sprite sprite;
		// // 텍스처 좌표는 정규화된 좌표니까 0~1사이의 값으로 바꾸어 주어야 함
		sprite.LeftTop = Vector2((leftTop.x + (seperatingSize.x * i)) / width, leftTop.y / height);
		sprite.Size = Vector2(seperatingSize.x / width, seperatingSize.y / height);
		sprite.Offset = offset;
		sprite.Duration = duration;
		sprite.AtlasImageSize = Vector2(magnification / width, magnification / height);					// PixelShader에서 아틀라스의 크기를 알게 하기 위해서 넣어주는 값
		mSpriteSheets.push_back(sprite);
	}
}

void Animation::CreateVerticalAnimation(const std::string& animKey, Texture& atlasImage, const DirectX::SimpleMath::Vector2& leftTop, const DirectX::SimpleMath::Vector2& seperatingSize, const DirectX::SimpleMath::Vector2& offset, const UINT spriteCount, const float duration, const float magnification)
{
	mAnimKey = animKey;
	float width = static_cast<float>(atlasImage.GetWidth());
	float height = static_cast<float>(atlasImage.GetHeight());

	for (UINT i = 0; i < spriteCount; ++i)
	{
		Sprite sprite;
		// // 텍스처 좌표는 정규화된 좌표니까 0~1사이의 값으로 바꾸어 주어야 함
		sprite.LeftTop = Vector2(leftTop.x / width, (leftTop.y + (seperatingSize.y * i)) / height);
		sprite.Size = Vector2(seperatingSize.x / width, seperatingSize.y / height);
		sprite.Offset = offset;
		sprite.Duration = duration;
		sprite.AtlasImageSize = Vector2(magnification / width, magnification / height);					// PixelShader에서 아틀라스의 크기를 알게 하기 위해서 넣어주는 값
		mSpriteSheets.push_back(sprite);
	}
}



void Animation::Reset()
{
	mAccumTimer = 0.0f;
	mIndex = 0;
	mbIsAnimComplete = false;
}
}


