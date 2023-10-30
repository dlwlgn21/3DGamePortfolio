#pragma once
#include "GraphicResource.h"
#include "GraphicPSO.h"

namespace jh::graphics
{
	class Texture;
	class Material : public GraphicResource
	{
	public:
		Material();
		virtual ~Material() = default;
		void ClearPipeline();

		void SetPSO(GraphicsPSO& pso) { mGraphicsPSO = pso; }
		void SetTexture(const eTextureType eType, Texture* pTexture);
		void SetPipeline();

	protected:
		GraphicsPSO			mGraphicsPSO;
		Texture*			mpTextures[static_cast<UINT>(eTextureType::COUNT)];
	};
}