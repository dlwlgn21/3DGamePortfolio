#pragma once
#include "Component.h"
#include "Animation.h"
#include "Texture.h"

namespace jh
{

class Animator : public Component
{
public:
	struct Events
	{
		struct Event
		{
			void operator=(std::function<void()> func)
			{
				mEvent = std::move(func);
			}

			void operator()()
			{
				if (mEvent != nullptr)
				{
					mEvent();
					return;
				}
				//assert(false);
			}
			std::function<void()> mEvent;

		};
		Event StartEvent;					// �ִϸ��̼��� ���۵� �� ȣ��
		Event CompleteEvent;				// �� �ִϸ��̼��� �� ����Ŭ �������� ���� ȣ��
		Event EndEvent;						// �ִϸ��̼��� ��ü�Ǿ��� ����
	};

	Animator();
	virtual ~Animator();

	void Update() override;
	void UpdateConstantBuffer();

	jh::graphics::Animation* Create(
		const std::string& animKey,
		jh::graphics::Texture& atalsImage,
		const DirectX::SimpleMath::Vector2& leftTop,
		const DirectX::SimpleMath::Vector2& seperatingSize,
		const DirectX::SimpleMath::Vector2& offset,
		const UINT spriteCount,
		const float duration,
		const float magnification
	);

	jh::graphics::Animation* CreateVertical(
		const std::string& animKey,
		jh::graphics::Texture & atalsImage,
		const DirectX::SimpleMath::Vector2& leftTop,
		const DirectX::SimpleMath::Vector2& seperatingSize,
		const DirectX::SimpleMath::Vector2& offset,
		const UINT spriteCount,
		const float duration,
		const float magnification
	);

	void SetCurrentAnimation(jh::graphics::Animation* pAnimation) { assert(pAnimation != nullptr); mpCurrAnimatingAnimation = pAnimation; }
	void PlayAnimation(const std::string& key, const bool bIsLooping);
	void PlayAnimationWithReset(const std::string& key, const bool bIsLooping);

	__forceinline jh::graphics::Animation* GetCurrentAnimatingAnimation() const { assert(mpCurrAnimatingAnimation != nullptr); return mpCurrAnimatingAnimation; }
	__forceinline UINT GetCurrentAnimationIndex() const { return GetCurrentAnimatingAnimation()->GetCurrentIndex(); }
	__forceinline void InitializeCurrAnimation() { assert(mpCurrAnimatingAnimation != nullptr); mpCurrAnimatingAnimation->Reset(); }

	jh::graphics::Animation* FindAnimationOrNull(const std::string& key);
	Events* FindEventsOrNull(const std::string& key);

	std::function<void()>& GetStartEvent(const std::string& key);
	std::function<void()>& GetCompleteEvent(const std::string& key);
	std::function<void()>& GetEndEvent(const std::string& key);

	void SetCurrAnimationHorizontalFlip(const bool bIsFlip);
	void SetActive(const bool isActive) { mbIsActive = isActive; }

	bool IsActive() const { return mbIsActive; }

protected:
	std::unordered_map<std::string, jh::graphics::Animation*>		mAnimationMap;
	std::unordered_map<std::string, Events*>						mEventsMap;
	jh::graphics::Animation*										mpCurrAnimatingAnimation = nullptr;
	bool															mbIsAnimationLooping = false;
	bool															mbIsActive = false;
};
}
