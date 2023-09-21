#include "Mesh.h"
#include "GraphicDeviceDX11.h"

using namespace jh::enums;

namespace jh::graphics
{
	Mesh::Mesh()
		: GraphicResource(eResourceType::MESH)
		, mcpVertexBuffer()
		, mcpIndexBuffer()
		, mStride(0)
		, mIndexCount(0)
	{

	}

	Mesh::~Mesh()
	{
		mcpIndexBuffer.Reset();
		mcpVertexBuffer.Reset();
	}

	void Mesh::InitVertexIndexBuffer(D3D11_BUFFER_DESC& VBDesc, void* pVertexInitData, UINT stride, D3D11_BUFFER_DESC& IBDesc, void* pIndexInitData, UINT indexCount)
	{
		auto& gd = GraphicDeviceDX11::GetInstance().GetDeivce();
		mStride = stride;
		mIndexCount = indexCount;
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = pVertexInitData;
		HRESULT hr = gd.CreateBuffer(
			&VBDesc,
			&subData,
			mcpVertexBuffer.ReleaseAndGetAddressOf()
		);
		if (FAILED(hr))
		{
			assert(false);
			return;
		}
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
		subData.pSysMem = pIndexInitData;
		hr = gd.CreateBuffer(
			&IBDesc,
			&subData,
			mcpIndexBuffer.ReleaseAndGetAddressOf()
		);
		if (FAILED(hr))
		{
			assert(false);
			return;
		}
	}

	void Mesh::Render()
	{
		assert(mStride != 0);
		auto& gdc = GraphicDeviceDX11::GetInstance().GetDeivceContext();
		UINT offset = 0;
		gdc.IASetVertexBuffers(
			0,
			1,
			mcpVertexBuffer.GetAddressOf(),
			&mStride,
			&offset
		);
		gdc.IASetIndexBuffer(
			mcpIndexBuffer.Get(),
			DXGI_FORMAT::DXGI_FORMAT_R32_UINT,
			0
		);
		gdc.DrawIndexed(mIndexCount, 0, 0);
	}

}