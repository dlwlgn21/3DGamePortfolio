#pragma once
#include "Common.h"
#include <directxtk/SimpleMath.h>

namespace jh
{

class Camera;
class CameraScript;
struct Rotation;
class CameraManager final
{
public:
	static CameraManager& GetInstance()
	{
		static CameraManager instance;
		return instance;
	}

	CameraManager(const CameraManager& other) = delete;
	CameraManager& operator=(const CameraManager& other) = delete;

	void SetCamera(Camera& camera);
	Camera& GetCamera() { assert(mpCamera != nullptr); return *mpCamera; }

	DirectX::SimpleMath::Vector3& GetRotation();
	void OnMouseMove(float mouseNdcX, float mouseNdcY);


private:
	CameraManager() = default;
	~CameraManager() = default;

private:
	Camera* mpCamera = nullptr;
	CameraScript* mpScript = nullptr;
};

}

