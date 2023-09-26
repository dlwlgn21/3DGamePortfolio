#include "Texture.h"
#include "GraphicDeviceDX11.h"

using namespace DirectX;
using namespace jh::enums;

namespace jh::graphics
{
	Texture::Texture()
		: GraphicResource(eResourceType::TEXTURE)
		, mcpTexture(nullptr)
		, mcpSRV(nullptr)
		, mTextureDesc{}
		, meType(eTextureType::COUNT)
	{
	}

	Texture::~Texture()
	{
		mcpSRV.Reset();
		mcpTexture.Reset();
	}

	void Texture::Clear(const UINT startSlot)
	{
		ID3D11ShaderResourceView* pSrvNullPtr = nullptr;
		auto& context = GraphicDeviceDX11::GetInstance().GetDeivceContext();
		context.VSSetShaderResources(startSlot, 1, &pSrvNullPtr);
		context.DSSetShaderResources(startSlot, 1, &pSrvNullPtr);
		context.GSSetShaderResources(startSlot, 1, &pSrvNullPtr);
		context.HSSetShaderResources(startSlot, 1, &pSrvNullPtr);
		context.CSSetShaderResources(startSlot, 1, &pSrvNullPtr);
		context.PSSetShaderResources(startSlot, 1, &pSrvNullPtr);
	}


	HRESULT Texture::Load(const std::wstring& filePath)
	{
		loadFile(filePath);
		createSRVFromLoadedImageFile();
		return S_OK;
	}

	void Texture::InitSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& cpSRV)
	{
		assert(mcpSRV == nullptr);
		mcpSRV = cpSRV;
	}

	void Texture::PSSetSRV()
	{
		auto& gdc = GraphicDeviceDX11::GetInstance().GetDeivceContext();
		switch (meType)
		{
		case eTextureType::DIFFUSE:
			gdc.PSSetShaderResources(0, 1, mcpSRV.GetAddressOf());
			break;
		case eTextureType::SPECULAR:
			gdc.PSSetShaderResources(1, 1, mcpSRV.GetAddressOf());
			break;
		case eTextureType::AMBIENT:
			gdc.PSSetShaderResources(2, 1, mcpSRV.GetAddressOf());
			break;
		default:
			assert(false);
			break;
		}
	}

	void Texture::loadFile(const std::wstring& fullPath)
	{
		wchar_t szExtension[256] = {};
		_wsplitpath_s(fullPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 256);

		std::wstring extension(szExtension);

		if (extension == L".dds" || extension == L".DDS")
		{
			if (FAILED(LoadFromDDSFile(fullPath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
				return;
		}
		else if (extension == L".tga" || extension == L".TGA")
		{
			if (FAILED(LoadFromTGAFile(fullPath.c_str(), nullptr, mImage)))
				return;
		}
		else // WIC (png, jpg, jpeg, bmp )
		{
			if (FAILED(LoadFromWICFile(fullPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
				return;
		}
	}

	void Texture::createSRVFromLoadedImageFile()
	{
		HRESULT hr = DirectX::CreateShaderResourceView(
			&GraphicDeviceDX11::GetInstance().GetDeivce(),
			mImage.GetImages(),
			mImage.GetImageCount(),
			mImage.GetMetadata(),
			mcpSRV.GetAddressOf()
		);
		if (FAILED(hr))
		{
			assert(false);
			return;
		}
		mcpSRV->GetResource(reinterpret_cast<ID3D11Resource**>(mcpTexture.GetAddressOf()));
		mcpTexture->GetDesc(&mTextureDesc);
	}

}