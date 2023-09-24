#pragma once
#include "Common.h"
#include <directxtk/SimpleMath.h>

namespace jh
{

class Camera;
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

	void SetCamera(Camera& camera) { assert(mpCamera == nullptr); mpCamera = &camera; }
	Camera& GetCamera() { assert(mpCamera != nullptr); return *mpCamera; }

	DirectX::SimpleMath::Vector3& GetRotation();

private:
	CameraManager() = default;
	~CameraManager() = default;

private:
	Camera* mpCamera = nullptr;
};

}

