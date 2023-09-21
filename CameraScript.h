#pragma once
#include "Script.h"

namespace jh
{


class CameraScript final : public Script
{
public:
	CameraScript();
	virtual ~CameraScript() = default;

	void Update() override;
private:
	float mMovementSpeed;
};



}