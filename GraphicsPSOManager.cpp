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
	initTextures();
	initMaterials();
	initPipelineStates();
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
	std::vector<UINT> indices;
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.ByteWidth = sizeof(Vertex2D) * 3;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));

	ibDesc.ByteWidth = sizeof(UINT) * static_cast<UINT>(indices.size());
	ibDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	ibDesc.CPUAccessFlags = 0;

	mspTriangleMesh = std::make_unique<Mesh>();

	mspTriangleMesh->InitVertexIndexBuffer(
		vbDesc,
		geoGenerator.GetTriangle().data(),
		static_cast<UINT>(sizeof(Vertex2D)),
		ibDesc,
		indices.data(),
		static_cast<UINT>(indices.size())
	);

	indices.clear();
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	D3D11_BUFFER_DESC squreVBDesc;
	ZeroMemory(&squreVBDesc, sizeof(D3D11_BUFFER_DESC));
	squreVBDesc.ByteWidth = sizeof(Vertex2D) * 4;
	squreVBDesc.Usage = D3D11_USAGE_DYNAMIC;
	squreVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	squreVBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	ibDesc.ByteWidth = sizeof(UINT) * static_cast<UINT>(indices.size());

	mspSqureMesh = std::make_unique<Mesh>();

	mspSqureMesh->InitVertexIndexBuffer(
		squreVBDesc,
		geoGenerator.GetSquare().data(),
		static_cast<UINT>(sizeof(Vertex2D)),
		ibDesc,
		indices.data(),
		static_cast<UINT>(indices.size())
	);

	mspBox2DebugMesh = std::make_unique<Box2DebugMesh>();
}
void GraphicsPSOManager::initShaders()
{
#pragma region BASIC_2D_PSO
	{
		vector<D3D11_INPUT_ELEMENT_DESC> basicIEs =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3,
				D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		D3D11Utils::CreateVertexShaderAndInputLayout(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"Basic2DVS.hlsl",
			basicIEs,
			mBasicPSO.mcpVertexShader,
			mBasicPSO.mcpInputLayout
		);

		D3D11Utils::CreatePixelShader(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"Basic2DPS.hlsl",
			mBasicPSO.mcpPixelShader
		);
	}
#pragma endregion

#pragma region BOX2D_DEBUG_PSO
	{
		vector<D3D11_INPUT_ELEMENT_DESC> debugIEs =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		D3D11Utils::CreateVertexShaderAndInputLayout(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"DebugBox2DVS.hlsl",
			debugIEs,
			mBox2DebugPSO.mcpVertexShader,
			mBox2DebugPSO.mcpInputLayout
		);

		D3D11Utils::CreatePixelShader(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"DebugBox2DPS.hlsl",
			mBox2DebugPSO.mcpPixelShader
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

	mspConstantBuffers[static_cast<UINT>(eCBType::DEBUG_TRANSFORM)] = std::make_unique<jh::graphics::ConstantGPUBuffer>(eCBType::DEBUG_TRANSFORM);
	mspConstantBuffers[static_cast<UINT>(eCBType::DEBUG_TRANSFORM)]->Create(sizeof(DebugTransformConstantCPUBuffer));

	mspConstantBuffers[static_cast<UINT>(eCBType::ANIMATION)] = std::make_unique<jh::graphics::ConstantGPUBuffer>(eCBType::ANIMATION);
	mspConstantBuffers[static_cast<UINT>(eCBType::ANIMATION)]->Create(sizeof(AnimationConstantCPUBuffer));
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
	loadAndInsertTexture(eTextureType::SPRITE_SHHET_TEXTURE, keys::PLAYER_SPRITE_SHEET_TEXTURE_KEY, L"D:\\2DGamePortfolio\\Assets\\Textures\\PLAYER_SpriteSheet_90x37.png");
}
void GraphicsPSOManager::initMaterials()
{
	insertMaterial(keys::BASIC_2D_MATERIAL_KEY, mBasicPSO);
	Texture* pTex = ResourcesManager::Find<Texture>(keys::PLAYER_SPRITE_SHEET_TEXTURE_KEY);
	assert(pTex != nullptr);
	ResourcesManager::Find<Material>(keys::BASIC_2D_MATERIAL_KEY)->SetTexture(
		pTex->GetTextureType(),
		pTex
	);
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
	ResourcesManager::Insert<Material>(key, pMaterial);
}
void GraphicsPSOManager::initPipelineStates()
{
	mBasicPSO.mcpRS = mcpWireRS;
	//mBasicPSO.mcpRS = mcpSolidRS;
	//mBasicPSO.mcpSampler = mcpPointBorderSampler;
	mBox2DebugPSO.mcpRS = mcpBox2DebugDrawRS;
}

}