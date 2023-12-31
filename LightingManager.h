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
	void Update();
	void SetCamera(Camera& camera);
	void PushLight(Light& light);
	void UpdateConstantBuffer();
	bool* GetIsUseDuffuseTexture() { return &mbIsUseDiffseTexture; }
	bool* GetIsUseNormalTexture() { return &mbIsUseNormalTexture; }

	Light& GetDirectionalLight() { return *mpLights[0]; }

	DirectX::SimpleMath::Matrix& GetShadowMappingViewMatrixRow();
	DirectX::SimpleMath::Matrix& GetShadowMappingProjectionMatrixRow();


private:

private:
	LightingManager() = default;
	~LightingManager() = default;

private:
	std::vector<Light*>					mpLights;
	jh::graphics::GrapicsMaterial		mMaterial = { };
	Camera*								mpCamera = nullptr;
	bool								mbIsUseDiffseTexture = false;
	bool								mbIsUseNormalTexture = false;
	DirectX::SimpleMath::Matrix			mLightViewMatrix;
	DirectX::SimpleMath::Matrix			mLightProjectionMatrix;
};

}

