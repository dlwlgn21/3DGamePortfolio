#pragma once
#include "GameObject.h"

namespace jh
{

class MoveableGameObject : public GameObject
{
public:
	MoveableGameObject();
	virtual ~MoveableGameObject() = default;

protected:
	virtual void initRenderer() = 0;
	virtual void initScript() = 0;
	virtual void initAnimator() = 0;
	virtual void initPhysics() = 0;
};



}


