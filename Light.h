#pragma once
#include "Component.h"
#include "GraphicsDatas.h"

namespace jh
{

enum class eLightType
{
	DIRECTIONAL,
	POINT,
	SPOT
};

class Light final : public Component
{
public:
	friend class LightingManager;
	Light();
	virtual ~Light() = default;
	void Update() override;
	void InitLight(const eLightType eType);
private:
	jh::graphics::GrapicsLight mLightInfo;
};

}

