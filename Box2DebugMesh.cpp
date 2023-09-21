#include "Box2DebugMesh.h"
#include "GraphicDeviceDX11.h"

namespace jh::graphics
{

Box2DebugMesh::Box2DebugMesh()
	: Mesh()
{
	mStride = sizeof(Box2DVertex);
	HRESULT hr;
	auto& gd = GraphicDeviceDX11::GetInstance().GetDeivce();

	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.ByteWidth = sizeof(Box2DVertex) * 4;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	hr = gd.CreateBuffer(
		&vbDesc,
		nullptr,
		mcpVertexBuffer.ReleaseAndGetAddressOf()
	);
	if (FAILED(hr)) { assert(false); }

	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	std::vector<UINT> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	ibDesc.ByteWidth = sizeof(UINT) * static_cast<UINT>(6);
	ibDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	ibDesc.CPUAccessFlags = 0;
	mIndexCount = static_cast<UINT>(indices.size());
	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = indices.data();

	hr = gd.CreateBuffer(
		&ibDesc,
		&subData,
		mcpIndexBuffer.ReleaseAndGetAddressOf()
	);
	if (FAILED(hr)) { assert(false); }
}

void Box2DebugMesh::UpdateVertexBuffer(std::vector<Box2DVertex>& vertices)
{
	assert(vertices.size() == 4);
	auto& gdc = GraphicDeviceDX11::GetInstance().GetDeivceContext();

	D3D11_MAPPED_SUBRESOURCE mappedSrc;
	ZeroMemory(&mappedSrc, sizeof(D3D11_MAPPED_SUBRESOURCE));
	gdc.Map(
		mcpVertexBuffer.Get(), 
		0, 
		D3D11_MAP_WRITE_DISCARD, 
		0, 
		&mappedSrc
	);

	memcpy(mappedSrc.pData, vertices.data(), mStride * vertices.size());
	gdc.Unmap(mcpVertexBuffer.Get(), 0);
}

}