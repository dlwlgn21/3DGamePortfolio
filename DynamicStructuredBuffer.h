#pragma once
#include "GraphicDeviceDX11.h"

namespace jh::graphics
{

template<typename T>
class DynamicStructuredBuffer
{
public:
    DynamicStructuredBuffer() = default;
    ~DynamicStructuredBuffer()
    {
        mcpSRV.Reset();
        mcpGPUBuffer.Reset();
    }

    void Create(const UINT numElements)
    {
        createBuffer();
    }
    void UpdateGPUBuffer(const UINT textureSlotNumber)
    {
		D3D11_MAPPED_SUBRESOURCE mappedSrc;
		ZeroMemory(&mappedSrc, sizeof(D3D11_MAPPED_SUBRESOURCE));
		auto& context = GraphicDeviceDX11::GetInstance().GetDeivceContext();
		HRESULT hr = context.Map(
			mcpGPUBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedSrc
		);
		if (FAILED(hr))
		{
			assert(false);
			return;
		}
		memcpy(mappedSrc.pData, mCPUBuffer.data(), mDesc.ByteWidth);
		context.Unmap(mcpGPUBuffer.Get(), 0);
        context.VSSetShaderResources(textureSlotNumber, 1, mcpSRV.GetAddressOf());
    }

    std::vector<T>& GetCPUBuffer()      { return mCPUBuffer; }
    const auto GetBuffer()              { return mcpGPUBuffer.Get(); }
    const auto GetSRV()                 { return mcpSRV.Get(); }
    const auto GetAddressOfSRV()        { return mcpSRV.GetAddressOf(); }

private:
    void createBuffer()
    {
        assert(!mCPUBuffer.empty());
        auto& device = GraphicDeviceDX11::GetInstance().GetDeivce();
        ZeroMemory(&mDesc, sizeof(D3D11_BUFFER_DESC));
        mDesc.Usage = D3D11_USAGE_DYNAMIC;
        mDesc.ByteWidth = static_cast<UINT>(mCPUBuffer.size() * sizeof(T));
        mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        mDesc.StructureByteStride = static_cast<UINT>(sizeof(T)); // 인덱싱을 하기 위해서, 각각의 element 크기를 알게 하는 것.
        mDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        HRESULT hr = device.CreateBuffer(&mDesc, NULL, mcpGPUBuffer.GetAddressOf());

        if (FAILED(hr)) { assert(false); }
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.BufferEx.NumElements = static_cast<UINT>(mCPUBuffer.size());
        hr = device.CreateShaderResourceView(mcpGPUBuffer.Get(), &srvDesc, mcpSRV.GetAddressOf());
        if (FAILED(hr)) { assert(false); }
    }

private:
    std::vector<T> mCPUBuffer;
    D3D11_BUFFER_DESC mDesc;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mcpGPUBuffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mcpSRV;
};

}

