#pragma once
#include "GraphicResource.h"

namespace jh::graphics
{
	class Mesh : public GraphicResource
	{
	public:
		Mesh();
		~Mesh();
		void Render();
		
		void InitVertexIndexBuffer(D3D11_BUFFER_DESC& VBDesc, void* pVertexInitData, UINT stride, D3D11_BUFFER_DESC& IBDesc, void* pIndexInitData, UINT indexCount);

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mcpVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mcpIndexBuffer;
		UINT mIndexCount;
		UINT mStride;
	};
}
