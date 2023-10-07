#define _CRT_SECURE_NO_WARNINGS
#include "LightingManager.h"
#include "D3DApp.h"
#include "GraphicsPSOManager.h"
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"


using namespace jh::graphics;
using namespace DirectX::SimpleMath;             

static constexpr const int DIFFUSE_TEXTURE_SLOT = 0;
static constexpr const int NORMAL_TEXTURE_SLOT = 3;

namespace jh
{

void LightingManager::Initialize()
{
	mpLights.reserve(MAX_LIGHTS);
	mMaterial.MaterialAmbient = DirectX::SimpleMath::Vector3(0.2f, 0.2f, 0.2f);
	mMaterial.MaterialShininess = 100.0f; 
	mMaterial.MaterialDiffuse = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f); 
	mMaterial.MaterialSpecular = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);
}

void LightingManager::SetCamera(Camera& camera)
{
	assert(mpCamera == nullptr);
	mpCamera = &camera;
}
void LightingManager::PushLight(Light& light)
{
	mpLights.push_back(&light);
}
void LightingManager::UpdateConstantBuffer()
{
	auto& gpuBuffer = GraphicsPSOManager::GetInstance().GetConstantBuffer(eCBType::LIGHTING);
	Vector3 camPos = mpCamera->GetOwner().GetTransform().GetPosition();
	jh::graphics::LighthingConstantCPUBuffer cpuBuffer;
	ZeroMemory(&cpuBuffer, sizeof(LighthingConstantCPUBuffer));
	for (auto& l : mpLights)
	{
		cpuBuffer.CBEyeWorld = Vector4(camPos.x, camPos.y, camPos.z, 0.0f);

		if (mbIsUseDiffseTexture)	
		{
			cpuBuffer.CBIsUseDiffuseTexture = 1; 
		}
		else						
		{
			cpuBuffer.CBIsUseDiffuseTexture = 0;
		}
		if (mbIsUseNormalTexture)	
		{
			cpuBuffer.CBIsUseNormalTexture = 1; 
		}
		else						
		{
			cpuBuffer.CBIsUseNormalTexture = 0;
		}

		cpuBuffer.CBLight[1] = l->mLightInfo;
		cpuBuffer.CBMaterial = mMaterial; 
	}

	gpuBuffer.UpdateBuffer(&cpuBuffer);
}

}
