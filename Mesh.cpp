#include "Mesh.h"
#include "GraphicsPSOManager.h"

using namespace jh::enums;

namespace jh::graphics
{
	Mesh::Mesh()
		: GraphicResource(eResourceType::MESH)
	{

	}

	Mesh::~Mesh()
	{
		mcpIndexBuffer.Reset();
		mcpVertexBuffer.Reset();
		mcpDebugNormalIndexBuffer.Reset();
		mcpDebugNormalVertexBuffer.Reset();
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

	void Mesh::DebugNormalRender()
	{
		assert(mStride != 0);
		auto& gdc = GraphicDeviceDX11::GetInstance().GetDeivceContext();
		auto& PSO = GraphicsPSOManager::GetInstance().mDebugDrawNormalPSO;

		gdc.IASetPrimitiveTopology(PSO.mPrimitiveTopology);
		gdc.RSSetState(PSO.mcpRS.Get());
		gdc.VSSetShader(PSO.mcpVertexShader.Get(), nullptr, 0);
		gdc.PSSetShader(PSO.mcpPixelShader.Get(), nullptr, 0);
		UINT offset = 0;
		gdc.IASetVertexBuffers(
			0,
			1,
			mcpDebugNormalVertexBuffer.GetAddressOf(),
			&mStride,
			&offset
		);
		gdc.IASetIndexBuffer(
			mcpDebugNormalIndexBuffer.Get(),
			DXGI_FORMAT::DXGI_FORMAT_R32_UINT,
			0
		);
		gdc.DrawIndexed(mDebugNormalIndexCount, 0, 0);
	}

}