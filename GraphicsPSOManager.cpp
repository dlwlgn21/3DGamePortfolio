#include <directxtk/SimpleMath.h>
#include <directxtk/DDSTextureLoader.h> 
#include <directxtk/WICTextureLoader.h>
#include "GraphicsPSOManager.h"
#include "GraphicDeviceDX11.h"
#include "GraphicsDatas.h"
#include "D3DUtills.h"
#include "GeomatryGenerator.h"

#include "MeshData.h"
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
	initCubeMap();
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
	Mesh* pBoxMesh = ResourcesManager::InsertOrNull<jh::graphics::Mesh>(keys::BOX_MESH_KEY, std::make_unique<jh::graphics::Mesh>());
	std::vector<Vertex3D> boxVertices;
	std::vector<UINT> indices;
	geoGenerator.MakeBox(boxVertices, indices);
	pBoxMesh->InitVertexIndexBuffer<Vertex3D>(boxVertices, indices);

	Mesh* pSpehereMesh = ResourcesManager::InsertOrNull<jh::graphics::Mesh>(keys::SPEHERE_MESH_KEY, std::make_unique<jh::graphics::Mesh>());
	std::vector<Vertex3D> sphereVertices;
	indices.clear();
	geoGenerator.MakeSphere(sphereVertices, indices, 0.2f, 10, 10);
	pSpehereMesh->InitVertexIndexBuffer<Vertex3D>(sphereVertices, indices);

	{
		std::vector<Vertex3D> wcVertices;
		std::vector<UINT> wcIndices;
		Mesh* pWcMesh = ResourcesManager::InsertOrNull<jh::graphics::Mesh>(keys::WORLD_COORD_MESH_KEY, std::make_unique<jh::graphics::Mesh>());
		geoGenerator.MakeWorldCoordinate(wcVertices, wcIndices, 5.0f);
		pWcMesh->InitVertexIndexBuffer<Vertex3D>(wcVertices, wcIndices); 

	}


	{
		Mesh* pMonkeyMesh = ResourcesManager::InsertOrNull<jh::graphics::Mesh>(keys::MONKEY_MESH_KEY, std::make_unique<jh::graphics::Mesh>());
		std::vector<MeshData> meshDatas = geoGenerator.ReadFromFile("D:\\3DGamePortfolioJH\\Assets\\", "Monkey.obj");
		pMonkeyMesh->InitVertexIndexBuffer(meshDatas[0].Vertices, meshDatas[0].Indices);
	}

	{
		std::vector<MeshData> meshDatas = geoGenerator.ReadFromFile("D:\\3DGamePortfolioJH\\Assets\\", "zeldaPosed001.fbx");
		
		ResourcesManager::InsertOrNull<jh::graphics::Mesh>(
			keys::ZELDA1_MESH, std::make_unique<jh::graphics::Mesh>()
		)->InitVertexIndexBuffer(meshDatas[0].Vertices, meshDatas[0].Indices);
		loadAndInsertTexture(eTextureType::DIFFUSE, keys::ZELDA1_TEXTURE, meshDatas[0].TextureFileFullPath);

		ResourcesManager::InsertOrNull<jh::graphics::Mesh>(
			keys::ZELDA2_MESH, std::make_unique<jh::graphics::Mesh>()
			)->InitVertexIndexBuffer(meshDatas[1].Vertices, meshDatas[1].Indices);
		loadAndInsertTexture(eTextureType::DIFFUSE, keys::ZELDA2_TEXTURE, meshDatas[1].TextureFileFullPath);

		ResourcesManager::InsertOrNull<jh::graphics::Mesh>(
			keys::ZELDA3_MESH, std::make_unique<jh::graphics::Mesh>()
			)->InitVertexIndexBuffer(meshDatas[2].Vertices, meshDatas[2].Indices);
		loadAndInsertTexture(eTextureType::DIFFUSE, keys::ZELDA3_TEXTURE, meshDatas[2].TextureFileFullPath);


		ResourcesManager::InsertOrNull<jh::graphics::Mesh>(
			keys::ZELDA4_MESH, std::make_unique<jh::graphics::Mesh>()
			)->InitVertexIndexBuffer(meshDatas[3].Vertices, meshDatas[3].Indices);
		loadAndInsertTexture(eTextureType::DIFFUSE, keys::ZELDA4_TEXTURE, meshDatas[3].TextureFileFullPath);


		ResourcesManager::InsertOrNull<jh::graphics::Mesh>(
			keys::ZELDA5_MESH, std::make_unique<jh::graphics::Mesh>()
			)->InitVertexIndexBuffer(meshDatas[4].Vertices, meshDatas[4].Indices);
		loadAndInsertTexture(eTextureType::DIFFUSE, keys::ZELDA5_TEXTURE, meshDatas[4].TextureFileFullPath);


		ResourcesManager::InsertOrNull<jh::graphics::Mesh>(
			keys::ZELDA6_MESH, std::make_unique<jh::graphics::Mesh>()
			)->InitVertexIndexBuffer(meshDatas[5].Vertices, meshDatas[5].Indices);
		loadAndInsertTexture(eTextureType::DIFFUSE, keys::ZELDA6_TEXTURE, meshDatas[5].TextureFileFullPath);

		ResourcesManager::InsertOrNull<jh::graphics::Mesh>(
			keys::ZELDA7_MESH, std::make_unique<jh::graphics::Mesh>()
			)->InitVertexIndexBuffer(meshDatas[6].Vertices, meshDatas[6].Indices);
		loadAndInsertTexture(eTextureType::DIFFUSE, keys::ZELDA7_TEXTURE, meshDatas[6].TextureFileFullPath);
	}
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


		D3D11Utils::CreateVertexShaderAndInputLayout(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"DebugWorldCoordinateVS.hlsl",
			basicIEs,
			mDebugDrawWorldCoordPSO.mcpVertexShader,
			mDebugDrawWorldCoordPSO.mcpInputLayout
		);

		D3D11Utils::CreatePixelShader(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"DebugWorldCoordinatePS.hlsl",
			mDebugDrawWorldCoordPSO.mcpPixelShader
		);
	}
#pragma endregion


#pragma region CUBE_MAPPING
	{
		vector<D3D11_INPUT_ELEMENT_DESC> cubeMappingIEs = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			 D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		D3D11Utils::CreateVertexShaderAndInputLayout(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"CubeMappingVS.hlsl",
			cubeMappingIEs,
			mCubeMapping.cpVertexShader,
			mCubeMapping.cpInputLayout
		);

		D3D11Utils::CreatePixelShader(
			GraphicDeviceDX11::GetInstance().GetDeivceComPtr(),
			L"CubeMappingPS.hlsl",
			mCubeMapping.cpPixelShader
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

	// CubeMapping
	Texture* pTex = ResourcesManager::InsertOrNull<Texture>(keys::CUBE_MAP_TEXTURE, std::make_unique<Texture>());
	pTex->SetTextureType(eTextureType::DIFFUSE);
	pTex->InitSRV(mCubeMapping.cpCubeMapRSV);
}
void GraphicsPSOManager::initMaterials()
{
	insertMaterial(keys::BASIC_3D_MATERIAL_KEY, mBasicPSO, keys::BASIC_3D_DIFFUSE_BOX_TEXTURE_KEY);
	insertMaterial(keys::WORLD_COORD_MATERIAL, mDebugDrawWorldCoordPSO, "");
	insertMaterial(keys::ZELDA1_MATERIAL, mBasicPSO, keys::ZELDA1_TEXTURE);
	insertMaterial(keys::ZELDA2_MATERIAL, mBasicPSO, keys::ZELDA2_TEXTURE);
	insertMaterial(keys::ZELDA3_MATERIAL, mBasicPSO, keys::ZELDA3_TEXTURE);
	insertMaterial(keys::ZELDA4_MATERIAL, mBasicPSO, keys::ZELDA4_TEXTURE);
	insertMaterial(keys::ZELDA5_MATERIAL, mBasicPSO, keys::ZELDA5_TEXTURE);
	insertMaterial(keys::ZELDA6_MATERIAL, mBasicPSO, keys::ZELDA6_TEXTURE);
	insertMaterial(keys::ZELDA7_MATERIAL, mBasicPSO, keys::ZELDA7_TEXTURE);
	insertMaterial(keys::CUBE_MAP_MATERIAL, mCubeMapPSO, keys::CUBE_MAP_TEXTURE);
}

void GraphicsPSOManager::loadAndInsertTexture(const eTextureType eType,const std::string& key, const std::wstring& fileName)
{
	Texture* pTex = ResourcesManager::Load<Texture>(key, fileName);
	pTex->SetTextureType(eType);
}

void GraphicsPSOManager::insertMaterial(const std::string& materialKey, GraphicsPSO& pso, const std::string& textureKeyOrNull)
{
	Material* pMaterial = ResourcesManager::InsertOrNull<Material>(materialKey, std::make_unique<Material>());
	pMaterial->SetPSO(pso);
	if (textureKeyOrNull.empty())
	{
		return;
	}
	pMaterial->SetTexture(eTextureType::DIFFUSE, ResourcesManager::Find<Texture>(textureKeyOrNull));
}
void GraphicsPSOManager::initPipelineStates()
{
	mBasicPSO.mcpRS = mcpSolidRS;
	mBasicPSO.mcpSampler = mcpPointBorderSampler;

	mDebugDrawNormalPSO.mPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	mDebugDrawNormalPSO.mcpRS = mcpSolidRS;

	mCubeMapPSO.mcpRS = mcpSolidRS;
	mCubeMapPSO.mcpVertexShader = mCubeMapping.cpVertexShader;
	mCubeMapPSO.mcpPixelShader = mCubeMapping.cpPixelShader;
	mCubeMapPSO.mPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mCubeMapPSO.mcpInputLayout = mCubeMapping.cpInputLayout;
	mCubeMapPSO.mcpSampler = mcpPointWrapSampler;

	mDebugDrawWorldCoordPSO.mcpRS = mcpSolidRS;
	mDebugDrawWorldCoordPSO.mPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	mDebugDrawWorldCoordPSO.mcpSampler = mcpPointWrapSampler;
}

void GraphicsPSOManager::initCubeMap()
{
	Mesh* pCubeMesh = ResourcesManager::InsertOrNull<jh::graphics::Mesh>(keys::CUBE_MAP_MESH, std::make_unique<Mesh>());
	mCubeMapping.pMesh = pCubeMesh;
	ComPtr<ID3D11Texture2D> cpTexture;
	auto& gd = jh::graphics::GraphicDeviceDX11::GetInstance();
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
		&gd.GetDeivce(),
		L"D:\\3DGamePortfolioJH\\Assets\\skybox\\skybox.dds",
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		D3D11_RESOURCE_MISC_TEXTURECUBE,
		DirectX::DX11::DDS_LOADER_FLAGS(false),
		(ID3D11Resource**)cpTexture.GetAddressOf(),
		mCubeMapping.cpCubeMapRSV.ReleaseAndGetAddressOf(),
		nullptr
	);
	if (FAILED(hr))
	{
		std::cout << "GraphicsPSOManager::initCubeMap() CreateDDSTextureFromFileEx FAILED" << std::endl;
		assert(false);
	}

	std::vector<Vertex3D> vertices;
	std::vector<UINT> indices;

	GeomatryGenerator::GetInstance().MakeBox(vertices, indices, 20.0f);
	std::reverse(indices.begin(), indices.end());
	pCubeMesh->InitVertexIndexBuffer<Vertex3D>(vertices, indices);
}


}