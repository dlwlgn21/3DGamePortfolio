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

	// Ÿ�̸Ӱ� �ش� �������� �����ð����� ũ�� ���� �ε����� �̵�
	if (mAccumTimer >= mSpriteSheets[mIndex].Duration)
	{
		mAccumTimer = 0.0f;
		++mIndex;
		if (mIndex >= mSpriteSheets.size())
		{
			mbIsAnimComplete = true;

			// �̰� �����̾߱�???? ����ڵ��� ��.
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
		// // �ؽ�ó ��ǥ�� ����ȭ�� ��ǥ�ϱ� 0~1������ ������ �ٲپ� �־�� ��
		sprite.LeftTop = Vector2((leftTop.x + (seperatingSize.x * i)) / width, leftTop.y / height);
		sprite.Size = Vector2(seperatingSize.x / width, seperatingSize.y / height);
		sprite.Offset = offset;
		sprite.Duration = duration;
		sprite.AtlasImageSize = Vector2(magnification / width, magnification / height);					// PixelShader���� ��Ʋ���� ũ�⸦ �˰� �ϱ� ���ؼ� �־��ִ� ��
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
		// // �ؽ�ó ��ǥ�� ����ȭ�� ��ǥ�ϱ� 0~1������ ������ �ٲپ� �־�� ��
		sprite.LeftTop = Vector2(leftTop.x / width, (leftTop.y + (seperatingSize.y * i)) / height);
		sprite.Size = Vector2(seperatingSize.x / width, seperatingSize.y / height);
		sprite.Offset = offset;
		sprite.Duration = duration;
		sprite.AtlasImageSize = Vector2(magnification / width, magnification / height);					// PixelShader���� ��Ʋ���� ũ�⸦ �˰� �ϱ� ���ؼ� �־��ִ� ��
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


