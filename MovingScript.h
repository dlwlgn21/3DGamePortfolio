#pragma once
#include "Script.h"
#include <box2d/b2_body.h>

namespace jh
{
	enum class eMovingState
	{
		LEFT,
		RIGHT,
		COUNT,
	};

	class MovingScript final : public Script
	{
	public:
		MovingScript();
		virtual ~MovingScript() = default;
		 
		void Initialize() override;
		void Update() override;

	private:
		float mMovementSpeed;
		b2Body* mpB2Body = nullptr;
		eMovingState meMovingState;
	};
}