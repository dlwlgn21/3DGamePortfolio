#pragma once
#include "GraphicPSO.h"
#include "Mesh.h"
#include "ConstantGPUBuffer.h"
#include "CubeMapping.h"

namespace jh::graphics
{
class GraphicsPSOManager final
{
public:
	static GraphicsPSOManager& GetInstance()
	{
		static GraphicsPSOManager gcm;
		return gcm;
	}
	GraphicsPSOManager(const GraphicsPSOManager& other) = delete;
	GraphicsPSOManager& operator=(const GraphicsPSOManager& other) = delete;


	void Initialize();
	void Release();

	const GraphicsPSO& GetBasicPSO() { return mBasicPSO; }
	GraphicsPSO* GetBasicPSOPtr() { return &mBasicPSO; }

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>& GetRSWire() { return mcpWireRS; }

	ConstantGPUBuffer& GetConstantBuffer(const eCBType eType) { assert(mspConstantBuffers[static_cast<UINT>(eType)] != nullptr); return *mspConstantBuffers[static_cast<UINT>(eType)].get(); }

private:
	void initMesh();
	void initShaders();
	void initCubeMap();
	void initRS();
	void initConstantBuffers();
	void initSamplers();
	void initTextures();
	void initMaterials();
	void initModels();
	void initPipelineStates();

	void loadAndInsertTexture(const eTextureType, const std::string& key, const std::wstring& fileName);
	void insertMaterial(const std::string& materialKey, GraphicsPSO& pso, const std::string& textureKeyOrNull);

private:
	GraphicsPSOManager() = default;
	~GraphicsPSOManager() = default;

public:
	GraphicsPSO											mBasicPSO;
	GraphicsPSO											mDebugDrawNormalPSO;
	GraphicsPSO											mDebugDrawWorldCoordPSO;
	GraphicsPSO											mCubeMapPSO;
	CubeMapping											mCubeMapping;

private:
	std::vector<std::unique_ptr<ConstantGPUBuffer>>		mspConstantBuffers;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			mcpPointBorderSampler;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			mcpPointWrapSampler;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		mcpSolidRS;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		mcpWireRS;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		mcpBox2DebugDrawRS;


};

}