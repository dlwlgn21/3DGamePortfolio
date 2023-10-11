#pragma once
#include "GraphicResource.h"
#include "GraphicDeviceDX11.h"

namespace jh::graphics
{

class Mesh : public GraphicResource
{
public:
	Mesh();
	~Mesh();
	void Render();
	void DebugNormalRender();
	template<class T>
	void InitVertexIndexBuffer(std::vector<T>& vetices, std::vector<UINT>& indicies);

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mcpVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mcpIndexBuffer = nullptr;

	UINT mIndexCount = 0;
	UINT mStride = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> mcpDebugNormalVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mcpDebugNormalIndexBuffer = nullptr;
	UINT mDebugNormalIndexCount = 0;
};

template<class T>
void Mesh::InitVertexIndexBuffer(std::vector<T>& vetices, std::vector<UINT>& indicies)
{
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.ByteWidth = static_cast<UINT>(sizeof(T) * vetices.size());
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	vbDesc.StructureByteStride = static_cast<UINT>(sizeof(T));

	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.ByteWidth = static_cast<UINT>(sizeof(UINT) * indicies.size());
	ibDesc.Usage = D3D11_USAGE_DYNAMIC;
	ibDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	ibDesc.StructureByteStride = static_cast<UINT>(sizeof(UINT));
	mIndexCount = static_cast<UINT>(indicies.size());
	mStride = vbDesc.StructureByteStride;

	auto& gd = GraphicDeviceDX11::GetInstance().GetDeivce();
	{
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = vetices.data();
		HRESULT hr = gd.CreateBuffer(
			&vbDesc,
			&subData,
			mcpVertexBuffer.ReleaseAndGetAddressOf()
		);
		if (FAILED(hr))
		{
			assert(false);
			return;
		}
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
		subData.pSysMem = indicies.data();
		hr = gd.CreateBuffer(
			&ibDesc,
			&subData,
			mcpIndexBuffer.ReleaseAndGetAddressOf()
		);
		if (FAILED(hr))
		{
			assert(false);
			return;
		}
	}

	std::vector<T> normalVertices;
	std::vector<UINT> normalIndices;
	for (UINT i = 0; i < vetices.size(); i++) {

		T v = vetices[i];
		v.UV.x = 0.0f; // 시작점 표시
		normalVertices.push_back(v);

		v.UV.x = 1.0f; // 끝점 표시
		normalVertices.push_back(v);

		normalIndices.push_back(static_cast<UINT>(2 * i));
		normalIndices.push_back(static_cast<UINT>(2 * i + 1));
	}
	vbDesc.ByteWidth = static_cast<UINT>(sizeof(T) * normalVertices.size());
	ibDesc.ByteWidth = static_cast<UINT>(sizeof(UINT) * normalIndices.size());
	mDebugNormalIndexCount = static_cast<UINT>(normalIndices.size());

	{
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = normalVertices.data();
		HRESULT hr = gd.CreateBuffer(
			&vbDesc,
			&subData,
			mcpDebugNormalVertexBuffer.ReleaseAndGetAddressOf()
		);
		if (FAILED(hr))
		{
			assert(false);
			return;
		}
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
		subData.pSysMem = normalIndices.data();
		hr = gd.CreateBuffer(
			&ibDesc,
			&subData,
			mcpDebugNormalIndexBuffer.ReleaseAndGetAddressOf()
		);
		if (FAILED(hr))
		{
			assert(false);
			return;
		}
	}

}

}

