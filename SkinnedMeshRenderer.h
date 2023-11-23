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
};


}


