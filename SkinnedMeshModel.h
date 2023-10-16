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

	//void InitAnimationDataAndStructuredBuffer(jh::graphics::AnimationData* pAnimData);

	void Render(const std::string& clipKey, const int frame);

	//jh::graphics::AnimationData& GetAnimData() { return *mpAnimData; }
//
//private:
//	void prepareBoneTransfromMatrices(const std::string& clipKey, const int frame);
//private:
//	jh::graphics::AnimationData* mpAnimData = nullptr;
//	jh::graphics::DynamicStructuredBuffer<DirectX::SimpleMath::Matrix> mBoneTransformMatrices;
};

}

