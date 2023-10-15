#pragma once
#include "Component.h"
#include "AnimationData.h"
#include "DynamicStructuredBuffer.h"

namespace jh
{

class BoneAnimator final : public Component
{
public:
	BoneAnimator();
	virtual ~BoneAnimator() = default;

	void Update();
	void FixedUpdate();

	void InitAnimationData(jh::graphics::AnimationData* pAnimData);

private:
	void prepareBoneTransfromMatrices(const int clipIndex, const int frame);
private:
	jh::graphics::AnimationData* mpAnimData = nullptr;
	jh::graphics::DynamicStructuredBuffer<DirectX::SimpleMath::Matrix> mBoneTransformMatrices;
};

}

