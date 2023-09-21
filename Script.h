#pragma once
#include "Component.h"

namespace jh
{


class Script : public Component
{
public:
	Script();
	virtual ~Script() = default;
	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
};


}

