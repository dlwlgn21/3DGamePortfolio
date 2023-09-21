#pragma once
#include "GraphicsCommon.h"

namespace jh::graphics
{

class ConstantGPUBuffer
{
public:
	ConstantGPUBuffer(const eCBType eType);
	~ConstantGPUBuffer();

	bool Create(const UINT bufferByteSize);
	void UpdateBuffer(void* pData);

private:
	const eCBType meCBType;
	D3D11_BUFFER_DESC mDesc;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mcpGPUBuffer;
};

}

