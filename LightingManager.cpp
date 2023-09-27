#include "LightingManager.h"
#include "D3DApp.h"
#include "GraphicsPSOManager.h"
#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
using namespace jh::graphics;
using namespace DirectX::SimpleMath;

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

void LightingManager::FixedUpdate()
{
	updateConstantBuffer();  
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
void LightingManager::updateConstantBuffer()
{
	auto& gpuBuffer = GraphicsPSOManager::GetInstance().GetConstantBuffer(eCBType::LIGHTING);
	Vector3 camPos = mpCamera->GetOwner().GetTransform().GetPosition();
	jh::graphics::LighthingConstantCPUBuffer cpuBuffer;
	ZeroMemory(&cpuBuffer, sizeof(LighthingConstantCPUBuffer));

	for (auto& l : mpLights)
	{
		cpuBuffer.CBEyeWorld = Vector4(camPos.x, camPos.y, camPos.z, 0.0f);
		if (mbIsUseTexture) {cpuBuffer.CBIsUseTexture = 1;}
		else                {cpuBuffer.CBIsUseTexture = 0;}

		cpuBuffer.CBLight[0] = l->mLightInfo;
		cpuBuffer.CBMaterial = mMaterial;
	}

	gpuBuffer.UpdateBuffer(&cpuBuffer);
}

}
