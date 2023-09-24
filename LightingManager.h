#pragma once
#include "Common.h"
#include "Light.h"
#include "GraphicsDatas.h"

namespace jh
{
	
class Camera;
class LightingManager final
{
public:
	static LightingManager& GetInstance()
	{
		static LightingManager instance;
		return instance;
	}

	LightingManager(const LightingManager& other) = delete;
	LightingManager& operator=(const LightingManager& other) = delete;

	void Initialize();
	void FixedUpdate();
	void SetCamera(Camera& camera);
	void PushLight(Light& light);

	bool* GetIsUseTexture() { return &mbIsUseTexture; }

private:
	void updateConstantBuffer();

private:
	LightingManager() = default;
	~LightingManager() = default;

private:
	std::vector<Light*>					mpLights;
	jh::graphics::GrapicsMaterial		mMaterial = { };
	Camera*								mpCamera = nullptr;
	bool								mbIsUseTexture = false;
};

}

