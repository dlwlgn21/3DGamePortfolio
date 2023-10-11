#pragma once
#include "Renderer.h"

namespace jh
{

enum class eCharacterAnimState
{
	IDLE,
	IDLE_TO_WALK,
	WALK_FORWARD,
	WALK_TO_IDLE,
	COUNT
};

class SkinnedMeshRenderer final : public Renderer
{
public:
	SkinnedMeshRenderer();
	virtual ~SkinnedMeshRenderer() = default;

	void Update() override;
	void Render() override;

private:
	eCharacterAnimState meState = eCharacterAnimState::IDLE;
};


}


