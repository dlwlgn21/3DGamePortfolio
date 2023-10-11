#pragma once
#include <directxtk/SimpleMath.h>
#include "Model.h"
#include "AnimationData.h"
#include "DynamicStructuredBuffer.h"

namespace jh::graphics
{

class SkinnedMeshModel final : public Model
{
public:
	SkinnedMeshModel();
	virtual ~SkinnedMeshModel() = default;

	void InitAnimationDataAndStructuredBuffer(jh::graphics::AnimationData& animData);


	void Render(const int clipIndex, const int frame);

	jh::graphics::AnimationData& GetAnimData() { return mAnimData; }

private:
	void updateBoneTransfromMatrices(const int clipIndex, const int frame);
private:
	jh::graphics::AnimationData mAnimData;
	jh::graphics::DynamicStructuredBuffer<DirectX::SimpleMath::Matrix> mBoneTransformMatrices;
};

}

