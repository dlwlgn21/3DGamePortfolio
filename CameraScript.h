#pragma once
#include "Script.h"

namespace jh
{

class Transform;
class CameraScript final : public Script
{
public:
	CameraScript();
	virtual ~CameraScript() = default;

	void Initialize() override;
	void Update() override;
	void MouseUpdate(const float mouseNdcX, const float mouseNdcY);
private:
	float			mMovementSpeed;
	Transform*		mpTransform;
	bool			mbIsPressedSpaceBar;
};



}