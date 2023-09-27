#include "Light.h"
#include "GameObject.h"
#include "Transform.h"
#include "LightingManager.h"

using namespace jh::enums;
using namespace DirectX::SimpleMath;

namespace jh
{
Light::Light()
	: Component(eComponentType::LIGHT)
{
	mLightInfo.LightStrengh = Vector3(0.0f, 0.0f, 0.0f);
	mLightInfo.LightDirection = Vector3(0.0f, 0.0f, 0.0f);
	mLightInfo.LightPosition = Vector3(0.0f, 0.0f, 0.0f);
	mLightInfo.LightFallOffStart = 0.0f;
	mLightInfo.LightFallOffEnd = 0.0f;
	mLightInfo.LightSpotPower = 0.0f;
}

void Light::Update()
{
	mLightInfo.LightPosition = GetOwner().GetTransform().GetPosition();
}

void Light::InitLight(const eLightType eType)
{
	mLightInfo.LightStrengh = Vector3(1.0f, 1.0f, 1.0f);
	mLightInfo.LightDirection = Vector3(0.0f, 0.0f, 1.0f);
	mLightInfo.LightPosition = GetOwner().GetTransform().GetPosition();
	switch (eType)
	{
	case eLightType::DIRECTIONAL:
	{
		mLightInfo.LightFallOffStart = 0.0f;
		mLightInfo.LightFallOffEnd = 0.0f;
		mLightInfo.LightSpotPower = 0.0f;
		break;
	}
	case eLightType::POINT:
	{
		mLightInfo.LightFallOffStart = 1.0f;
		mLightInfo.LightFallOffEnd = 10.0f;
		mLightInfo.LightSpotPower = 0.0f;
		break;
	}
	case eLightType::SPOT:
	{
		mLightInfo.LightFallOffStart = 1.0f;
		mLightInfo.LightFallOffEnd = 10.0f;
		mLightInfo.LightSpotPower = 10.0f;
		break;
	}
	default:
		assert(false);
		break;
	}
	LightingManager::GetInstance().PushLight(*this); 
}

}