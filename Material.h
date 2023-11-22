#pragma once
#include "GraphicResource.h"
#include "GraphicPSO.h"

namespace jh::graphics
{

class Material : public GraphicResource
{
public:
	Material();
	virtual ~Material() = default;
	void SetPSO(GraphicsPSO& pso) { mpGraphicsPSO = &pso; }
	void SetPipeline();

protected:
	GraphicsPSO*			mpGraphicsPSO;
};

}