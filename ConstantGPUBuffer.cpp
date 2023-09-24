#include "ConstantGPUBuffer.h"
#include "GraphicDeviceDX11.h"

namespace jh::graphics
{


ConstantGPUBuffer::ConstantGPUBuffer(const eCBType eType)
	: meCBType(eType)
	, mDesc({ 0, })
	, mcpGPUBuffer()
{
}
ConstantGPUBuffer::~ConstantGPUBuffer()
{
	mcpGPUBuffer.Reset();
}
bool ConstantGPUBuffer::Create(const UINT bufferByteSize)
{
	mDesc.ByteWidth = bufferByteSize;
	mDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	mDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	mDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mDesc.MiscFlags = 0;
	mDesc.StructureByteStride = 0;

	HRESULT hr = GraphicDeviceDX11::GetInstance().GetDeivce().CreateBuffer(&mDesc, nullptr, mcpGPUBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		assert(false);
		return false;
	}
	return true;
}
void ConstantGPUBuffer::UpdateBuffer(void* pData)
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
	memcpy(mappedSrc.pData, pData, mDesc.ByteWidth);
	context.Unmap(mcpGPUBuffer.Get(), 0);
	switch (meCBType)
	{
	case jh::graphics::eCBType::TRANSFORM:
	{
		context.VSSetConstantBuffers(TRANSFORM_CONSTANT_BUFFER_SLOT, 1, mcpGPUBuffer.GetAddressOf());
		break;
	}
	case jh::graphics::eCBType::LIGHTING:
	{
		context.PSSetConstantBuffers(LIGHTING_CONSTANT_BUFFER_SLOT, 1, mcpGPUBuffer.GetAddressOf());
		break;
	}
	default:
		assert(false);
		break;
	}
}


}