#include <directxtk/SimpleMath.h>
#include "GraphicsPSOManager.h"
#include "GraphicDeviceDX11.h"
#include "GraphicsDatas.h"
#include "D3DUtills.h"
#include "GeomatryGenerator.h"

#include "Texture.h"
#include "Material.h"
#include "ResourcesManager.h"
#include "GraphicsKeyContainer.h"

using namespace DirectX::SimpleMath;

namespace jh::graphics
{

void GraphicsPSOManager::Initialize()
{
	initMesh();
	initShaders(); 
	initRS();
	initConstantBuffers();
	initSamplers();
	initPipelineStates();
	initTextures();
	initMaterials();
}

void GraphicsPSOManager::Release()
{
	mcpPointBorderSampler.Reset();
	mcpPointWrapSampler.Reset();

	mcpWireRS.Reset();
	mcpSolidRS.Reset();
	mcpBox2DebugDrawRS.Reset();

	mspConstantBuffers.clear();
}

void GraphicsPSOManager::initMesh()
{
	auto& geoGenerator = GeomatryGenerator::GetInstance();
	mspBoxMesh = std::make_unique<Mesh>();
	std::vector<Vertex3D> boxVertices;
	std::vector<UINT> indices;
	geoGenerator.MakeBox(boxVertices, indices);
	mspBoxMesh->InitVertexIndexBuffer<Vertex3D>(boxVertices, indices);

	mspSphereMesh = std::make_unique<Mesh>();
	std::vector<Vertex3D> sphereVertices;
	indices.clear();
	geoGenerator.MakeSphere(sphereVertices, indices, 1.0f, 10, 10);
	mspSphereMesh->InitVertexIndexBuffer<Vertex3D>(sphereVertices, indices);

}
void GraphicsPSOManager::initShaders()
{
#pragma region BASIC_3D_PSO
	{
		vector<D3D11_INPUT_ELEMENT_DESC> basicIEs = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		D3D11Utils::CreateVertexShaderAndInputLayout(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"Basic3DVS.hlsl",
			basicIEs,
			mBasicPSO.mcpVertexShader,
			mBasicPSO.mcpInputLayout
		);

		D3D11Utils::CreatePixelShader(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"Basic3DPS.hlsl",
			mBasicPSO.mcpPixelShader
		);
	}
#pragma endregion

#pragma region DEBUG_DRAW_NORMAL_PSO
	{
		vector<D3D11_INPUT_ELEMENT_DESC> basicIEs = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		D3D11Utils::CreateVertexShaderAndInputLayout(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"DebugNormalVS.hlsl",
			basicIEs,
			mDebugDrawNormalPSO.mcpVertexShader,
			mDebugDrawNormalPSO.mcpInputLayout
		);

		D3D11Utils::CreatePixelShader(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"DebugNormalPS.hlsl",
			mDebugDrawNormalPSO.mcpPixelShader
		);
	}
#pragma endregion


}
void GraphicsPSOManager::initRS()
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthClipEnable = true;
	rsDesc.MultisampleEnable = false;
	hr = GraphicDeviceDX11::GetInstance().GetDeivce().CreateRasterizerState(
		&rsDesc,
		mcpSolidRS.ReleaseAndGetAddressOf()
	);
	if (FAILED(hr))
	{
		assert(false);
		return;
	}

	rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	hr = GraphicDeviceDX11::GetInstance().GetDeivce().CreateRasterizerState(
		&rsDesc,
		mcpWireRS.ReleaseAndGetAddressOf()
	);
	if (FAILED(hr))
	{
		assert(false);
		return;
	}


	rsDesc.FrontCounterClockwise = true;
	hr = GraphicDeviceDX11::GetInstance().GetDeivce().CreateRasterizerState(
		&rsDesc,
		mcpBox2DebugDrawRS.ReleaseAndGetAddressOf()
	);
	if (FAILED(hr))
	{
		assert(false);
		return;
	}
}

void GraphicsPSOManager::initConstantBuffers()
{
	mspConstantBuffers.reserve(static_cast<UINT>(eCBType::COUNT));
	mspConstantBuffers.resize(mspConstantBuffers.capacity());

	mspConstantBuffers[static_cast<UINT>(eCBType::TRANSFORM)] = std::make_unique<jh::graphics::ConstantGPUBuffer>(eCBType::TRANSFORM);
	mspConstantBuffers[static_cast<UINT>(eCBType::TRANSFORM)]->Create(sizeof(TransformConstantCPUBuffer));

	mspConstantBuffers[static_cast<UINT>(eCBType::LIGHTING)] = std::make_unique<jh::graphics::ConstantGPUBuffer>(eCBType::LIGHTING);
	mspConstantBuffers[static_cast<UINT>(eCBType::LIGHTING)]->Create(sizeof(LighthingConstantCPUBuffer));
}

void GraphicsPSOManager::initSamplers()
{
	HRESULT hr;
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	hr = GraphicDeviceDX11::GetInstance().GetDeivce().CreateSamplerState(
		&samplerDesc,
		mcpPointBorderSampler.ReleaseAndGetAddressOf()
	);
	if (FAILED(hr))
	{
		assert(false);
	}
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hr = graphics::GraphicDeviceDX11::GetInstance().GetDeivce().CreateSamplerState(
		&samplerDesc,
		mcpPointWrapSampler.ReleaseAndGetAddressOf()
	);
	graphics::GraphicDeviceDX11::GetInstance().GetDeivceContext().PSSetSamplers(
		0,
		1,
		mcpPointBorderSampler.GetAddressOf()
	);

	if (FAILED(hr))
	{
		assert(false);
	}
}

void GraphicsPSOManager::initTextures()
{
	loadAndInsertTexture(eTextureType::DIFFUSE, keys::BASIC_3D_DIFFUSE_BOX_TEXTURE_KEY, L"D:\\3DGamePortfolioJH\\Assets\\Textures\\brickwall.jpg");
}
void GraphicsPSOManager::initMaterials()
{
	insertMaterial(keys::BASIC_3D_MATERIAL_KEY, mBasicPSO);
}

void GraphicsPSOManager::loadAndInsertTexture(const eTextureType eType,const std::string& key, const std::wstring& fileName)
{
	Texture* pTex = ResourcesManager::Load<Texture>(key, fileName);
	pTex->SetTextureType(eType);
}

void GraphicsPSOManager::insertMaterial(const std::string& key, GraphicsPSO& pso)
{
	Material* pMaterial = new Material();
	pMaterial->SetPSO(pso);
	pMaterial->SetTexture(eTextureType::DIFFUSE, ResourcesManager::Find<Texture>(keys::BASIC_3D_DIFFUSE_BOX_TEXTURE_KEY));
	ResourcesManager::Insert<Material>(key, pMaterial);
}
void GraphicsPSOManager::initPipelineStates()
{
	mBasicPSO.mcpRS = mcpSolidRS;
	mBasicPSO.mcpSampler = mcpPointBorderSampler;

	mDebugDrawNormalPSO.mPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	mDebugDrawNormalPSO.mcpRS = mcpSolidRS;
}
}