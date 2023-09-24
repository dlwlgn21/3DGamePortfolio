#include "GraphicPSO.h"

namespace jh::graphics
{
	GraphicsPSO::~GraphicsPSO()
	{
		mcpRS.Reset();
		mcpSampler.Reset();
		mcpInputLayout.Reset();
		mcpPixelShader.Reset();
		mcpVertexShader.Reset();
	}
	void GraphicsPSO::operator=(const GraphicsPSO& pso)
	{
		mcpVertexShader = pso.mcpVertexShader;
		mcpPixelShader = pso.mcpPixelShader;
		mcpInputLayout = pso.mcpInputLayout;
		mcpSampler = pso.mcpSampler;
		mcpRS = pso.mcpRS;
		mPrimitiveTopology = pso.mPrimitiveTopology; 
	}
}
