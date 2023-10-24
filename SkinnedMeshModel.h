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
	void Render();
};

}

