#pragma once
#include "Renderer.h"
#include "BoneAnimator.h"

namespace jh
{

class SkinnedMeshRenderer final : public Renderer
{
public:
	SkinnedMeshRenderer();
	virtual ~SkinnedMeshRenderer() = default;

	//void Update() override;
	void Render() override;

//private:
//	BoneAnimator::eCharacterAnimState meState = BoneAnimator::eCharacterAnimState::IDLE;
//	std::vector<std::string> mClipKeys;
};


}


