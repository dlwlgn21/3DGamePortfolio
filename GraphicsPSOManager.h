#pragma once
#include "GraphicPSO.h"
#include "Mesh.h"
#include "ConstantGPUBuffer.h"

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

	Mesh& GetTriangleMesh() { return *mspTriangleMesh.get(); }
	Mesh& GetSqureMesh() { return *mspSqureMesh.get(); }
	Mesh& GetBoxMesh() { return *mspBoxMesh.get(); }
	Mesh& GetSpeherMesh() { return *mspSphereMesh.get(); }

	Mesh* GetTriangleMeshPtr() { return mspTriangleMesh.get(); }
	Mesh* GetSqureMeshPtr() { return mspSqureMesh.get(); }
	Mesh* GetBoxMeshPtr() { return mspBoxMesh.get(); }
	Mesh* GetSpeherMeshPtr() { return mspSphereMesh.get(); }

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>& GetRSWire() { return mcpWireRS; }

	ConstantGPUBuffer& GetConstantBuffer(const eCBType eType) { assert(mspConstantBuffers[static_cast<UINT>(eType)] != nullptr); return *mspConstantBuffers[static_cast<UINT>(eType)].get(); }

private:
	void initMesh();
	void initShaders();
	void initRS();
	void initConstantBuffers();
	void initSamplers();
	void initTextures();
	void initMaterials();
	void initPipelineStates();

	void loadAndInsertTexture(const eTextureType, const std::string& key, const std::wstring& fileName);
	void insertMaterial(const std::string& key, GraphicsPSO& pso);

private:
	GraphicsPSOManager() = default;
	~GraphicsPSOManager() = default;

public:
	GraphicsPSO											mBasicPSO;
	GraphicsPSO											mDebugDrawNormalPSO;
private:
	std::unique_ptr<Mesh>								mspTriangleMesh;
	std::unique_ptr<Mesh>								mspSqureMesh;
	std::unique_ptr<Mesh>								mspBoxMesh;
	std::unique_ptr<Mesh>								mspSphereMesh;

	std::vector<std::unique_ptr<ConstantGPUBuffer>>		mspConstantBuffers;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			mcpPointBorderSampler;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			mcpPointWrapSampler;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		mcpSolidRS;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		mcpWireRS;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		mcpBox2DebugDrawRS;


};

}