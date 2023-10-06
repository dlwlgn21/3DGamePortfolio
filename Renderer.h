#pragma once
#include "Component.h"
#include "Model.h"

namespace jh
{
class Renderer : public Component
{
public:
	Renderer(const jh::enums::eRenererComponentType eRendererType);
	virtual ~Renderer() = default;

	jh::enums::eRenererComponentType GetType() const { return meRendererType; }

	void SetModel(jh::graphics::Model* pModel) { assert(pModel != nullptr); mpModel = pModel; }

protected:
	const jh::enums::eRenererComponentType		meRendererType;
	jh::graphics::Model*						mpModel;
};


}