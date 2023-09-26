#pragma once
#include "GraphicResource.h"
#include <DirectXTex.h>
#include "GraphicsDatas.h"

namespace jh::graphics
{
	class Texture : public GraphicResource
	{
	public:
		Texture();
		virtual ~Texture();
		static void Clear(const UINT startSlot);

		HRESULT Load(const std::wstring& filePath) override;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetSRV() { return mcpSRV; }

		UINT GetHeight() const { return static_cast<UINT>(mTextureDesc.Height); }
		UINT GetWidth() const { return static_cast<UINT>(mTextureDesc.Width); }

		void InitSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& cpSRV);
		void PSSetSRV();
		const eTextureType GetTextureType() const { return meType; }
		void SetTextureType(const eTextureType eType) { meType = eType; }

	private:
		void loadFile(const std::wstring& filePath);
		void createSRVFromLoadedImageFile();

	private:
		DirectX::ScratchImage								mImage;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				mcpTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mcpSRV;
		D3D11_TEXTURE2D_DESC								mTextureDesc;
		eTextureType										meType;
	};
}


