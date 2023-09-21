#include "Animator.h"
#include "Texture.h"

static constexpr const UINT RESEVE_SIZE = 16;

using namespace jh::graphics;
using namespace DirectX::SimpleMath;
using namespace jh::enums;

namespace jh
{

Animator::Animator()
	: Component(eComponentType::ANIMATOR)
{
	mAnimationMap.reserve(RESEVE_SIZE);
	mEventsMap.reserve(RESEVE_SIZE);
}
Animator::~Animator()
{
	for (auto iter = mAnimationMap.begin(); iter != mAnimationMap.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
		}
	}
	for (auto iter = mEventsMap.begin(); iter != mEventsMap.end(); ++iter)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
		}
	}
}

void Animator::Update()
{
	if (mpCurrAnimatingAnimation == nullptr)
	{
		assert(false);
		return;
	}

	if (mpCurrAnimatingAnimation->IsAnimComplete())
	{
		Events* pEvents = FindEventsOrNull(mpCurrAnimatingAnimation->GetAnimationKey());
		if (pEvents == nullptr)
		{
			assert(false);
			return;
		}
		pEvents->CompleteEvent();
		if (mbIsAnimationLooping)
		{
			mpCurrAnimatingAnimation->Reset();
		}
	}

	mpCurrAnimatingAnimation->Update();
	UpdateConstantBuffer();
}

jh::graphics::Animation* Animator::Create(const std::string& animKey, Texture& atlasImage, const DirectX::SimpleMath::Vector2& leftTop, const DirectX::SimpleMath::Vector2& seperatingSize, const DirectX::SimpleMath::Vector2& offset, const UINT spriteCount, const float duration, const float magnification)
{

	Animation* pAnimation = FindAnimationOrNull(animKey);
	if (pAnimation != nullptr) { assert(false); }

	pAnimation = new Animation();
	pAnimation->Create(animKey, atlasImage, leftTop, seperatingSize, offset, spriteCount, duration, magnification);
	mAnimationMap.insert(std::make_pair(animKey, pAnimation));
	Events* pEvents = new Events();
	mEventsMap.insert(std::make_pair(animKey, pEvents));
	return pAnimation;
	return nullptr;
}
jh::graphics::Animation* Animator::CreateVertical(const std::string& animKey, Texture& atlasImage, const DirectX::SimpleMath::Vector2& leftTop, const DirectX::SimpleMath::Vector2& seperatingSize, const DirectX::SimpleMath::Vector2& offset, const UINT spriteCount, const float duration, const float magnification)
{
	Animation* pAnimation = FindAnimationOrNull(animKey);
	if (pAnimation != nullptr) { assert(false); }

	pAnimation = new Animation();
	pAnimation->CreateVerticalAnimation(animKey, atlasImage, leftTop, seperatingSize, offset, spriteCount, duration, magnification);
	mAnimationMap.insert(std::make_pair(animKey, pAnimation));
	Events* pEvents = new Events();
	mEventsMap.insert(std::make_pair(animKey, pEvents));
	return pAnimation;
}
void Animator::PlayAnimation(const std::string& key, const bool bIsLooping)
{
	Animation* pPrevAnim = mpCurrAnimatingAnimation;
	if (pPrevAnim == FindAnimationOrNull(key))
	{
		return;
	}

	Events* pEvents = nullptr;
	if (pPrevAnim != nullptr)
	{
		Events* pEvents = FindEventsOrNull(pPrevAnim->GetAnimationKey());
		if (pEvents == nullptr)
		{
			return;
		}
	}
	if (pEvents != nullptr)
	{
		pEvents->EndEvent();
	}

	mpCurrAnimatingAnimation = FindAnimationOrNull(key);
	if (mpCurrAnimatingAnimation == nullptr)
	{
		assert(false); return;
	}
	//mCurrAnimatingAnimation->Reset();
	mbIsAnimationLooping = bIsLooping;

	pEvents = FindEventsOrNull(mpCurrAnimatingAnimation->GetAnimationKey());
	if (pEvents == nullptr)
	{
		return;
	}

	pEvents->StartEvent();
}
void Animator::PlayAnimationWithReset(const std::string& key, const bool bIsLooping)
{
	Animation* pPrevAnim = mpCurrAnimatingAnimation;
	Events* pEvents = nullptr;

	if (pPrevAnim != nullptr)
	{
		Events* pEvents = FindEventsOrNull(pPrevAnim->GetAnimationKey());
		if (pEvents == nullptr)
		{
			return;
		}
	}
	if (pEvents != nullptr)
	{
		pEvents->EndEvent();
	}

	mpCurrAnimatingAnimation = FindAnimationOrNull(key);
	if (mpCurrAnimatingAnimation == nullptr)
	{
		assert(false); return;
	}
	mpCurrAnimatingAnimation->Reset();
	mbIsAnimationLooping = bIsLooping;

	pEvents = FindEventsOrNull(mpCurrAnimatingAnimation->GetAnimationKey());
	if (pEvents == nullptr)
	{
		return;
	}
	pEvents->StartEvent();
}
void Animator::UpdateConstantBuffer()
{
	if (mpCurrAnimatingAnimation == nullptr)
	{
		assert(false);
		return;
	}

	mpCurrAnimatingAnimation->UpdateConstantBuffer();
}

jh::graphics::Animation* Animator::FindAnimationOrNull(const std::string& key)
{
	auto iter = mAnimationMap.find(key);
	if (iter == mAnimationMap.end())
	{
		return nullptr;
	}
	return iter->second;
}
Animator::Events* Animator::FindEventsOrNull(const std::string& key)
{
	auto iter = mEventsMap.find(key);
	if (iter == mEventsMap.end())
	{
		return nullptr;
	}
	return iter->second;
}
std::function<void()>& Animator::GetStartEvent(const std::string& key)
{
	Events* pEvents = FindEventsOrNull(key);
	if (pEvents == nullptr)
	{
		assert(false);
	}
	return pEvents->StartEvent.mEvent;
}
std::function<void()>& Animator::GetCompleteEvent(const std::string& key)
{
	Events* pEvents = FindEventsOrNull(key);
	if (pEvents == nullptr)
	{
		assert(false);
	}
	return pEvents->CompleteEvent.mEvent;
}
std::function<void()>& Animator::GetEndEvent(const std::string& key)
{
	Events* pEvents = FindEventsOrNull(key);
	if (pEvents == nullptr)
	{
		assert(false);
	}
	return pEvents->EndEvent.mEvent;
}
void Animator::SetCurrAnimationHorizontalFlip(const bool bIsFlip)
{
	assert(mpCurrAnimatingAnimation != nullptr);
	mpCurrAnimatingAnimation->SetHorizontalFlip(bIsFlip);
}

}