#include <directxtk/SimpleMath.h>
#include <directxtk/DDSTextureLoader.h> 
#include <directxtk/WICTextureLoader.h>

#include "GraphicsPSOManager.h"
#include "GraphicDeviceDX11.h"
#include "GeomatryGenerator.h"
#include "AnimationDataManager.h"

#include "GraphicsDatas.h"
#include "D3DUtills.h"

#include "Model.h"
#include "SkinnedMeshModel.h"
#include "MeshData.h"
#include "ResourcesManager.h"
#include "GraphicsKeyContainer.h"


#include "DebugHelper.h"
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
		initModels();
		initEnv();
	}

	void GraphicsPSOManager::Release()
	{
		//mcpPointBorderSampler.Reset();
		//mcpPointWrapSampler.Reset();

		//mcpWireRS.Reset();
		//mcpSolidRS.Reset();
		//mcpBox2DebugDrawRS.Reset();
		//mspConstantBuffers.clear();

		//mCubeMapping;
		//mDepthOnlyPSO;
		//mCubeMapPSO;
		//mBasicPSO;
		//mDebugDrawNormalPSO;
		//mSkinnedBasicPSO;
		//mBasicEnvNoNormalPSO;
		//mDebugDrawWorldCoordPSO;

		mcpShadowMappingSampler.Reset();
		mcpPointWrapSampler.Reset();
		mcpPointBorderSampler.Reset();
		mspConstantBuffers.clear();

		mcpBox2DebugDrawRS.Reset();
		mcpWireRS.Reset();
		mcpSolidRS.Reset();


		mspCatsleMeshs.clear();
		mspCatsleTextures.clear();
	}

	void GraphicsPSOManager::initMesh()
	{
		using namespace jh::graphics;

		auto& geoGenerator = GeomatryGenerator::GetInstance();
		Mesh* pBoxMesh = ResourcesManager::InsertOrNull<Mesh>(keys::BOX_MESH_KEY, std::make_unique<jh::graphics::Mesh>());
		std::vector<Vertex3D> boxVertices;
		std::vector<UINT> indices;
		geoGenerator.MakeBox(boxVertices, indices);
		pBoxMesh->InitVertexIndexBuffer<Vertex3D>(boxVertices, indices);

		// Ground
		{
			Mesh* pGroundMesh = ResourcesManager::InsertOrNull<Mesh>(keys::GROUND_MESH_KEY, std::make_unique<jh::graphics::Mesh>());
			std::vector<Vertex3D> groundVertices;
			std::vector<UINT> groundIndices;
			geoGenerator.MakeSquare(groundVertices, groundIndices, 200.0f);
			pGroundMesh->InitVertexIndexBuffer<Vertex3D>(groundVertices, groundIndices);
		}

		Mesh* pSpehereMesh = ResourcesManager::InsertOrNull<Mesh>(keys::SPEHERE_MESH_KEY, std::make_unique<jh::graphics::Mesh>());
		std::vector<Vertex3D> sphereVertices;
		indices.clear();
		geoGenerator.MakeSphere(sphereVertices, indices, 0.2f, 10, 10);
		pSpehereMesh->InitVertexIndexBuffer<Vertex3D>(sphereVertices, indices);

		{
			std::vector<Vertex3D> wcVertices;
			std::vector<UINT> wcIndices;
			Mesh* pWcMesh = ResourcesManager::InsertOrNull<Mesh>(keys::WORLD_COORD_MESH_KEY, std::make_unique<jh::graphics::Mesh>());
			geoGenerator.MakeWorldCoordinate(wcVertices, wcIndices, 5.0f);
			pWcMesh->InitVertexIndexBuffer<Vertex3D>(wcVertices, wcIndices);

		}


		{
			//Mesh* pMonkeyMesh = ResourcesManager::InsertOrNull<Mesh>(keys::MONKEY_MESH_KEY, std::make_unique<jh::graphics::Mesh>());
			//std::vector<MeshData> meshDatas = geoGenerator.ReadFromFile("D:\\3DGamePortfolioJH\\Assets\\", "Monkey.obj");
			//pMonkeyMesh->InitVertexIndexBuffer(meshDatas[0].Vertices, meshDatas[0].Indices);
		}

#pragma region FEMALE_SOLDER
		{
			//std::string relativePath = "Assets\\Characters\\armored-female-future-soldier\\";
			//std::vector<MeshData> meshDatas = geoGenerator.ReadFromFile(relativePath, "angel_armor.fbx");
			//assert(meshDatas.size() == 1);
			//std::vector<jh::graphics::Mesh*> pMeshs;
			//pMeshs.reserve(meshDatas.size());

			//std::string diffuseFullPath = relativePath + "angel_armor_albedo.jpg";
			//std::string normalFullPath = relativePath + "angel_armor_normal.jpg";

			//meshDatas[0].DiffuseTextureFileFullPath = std::wstring(diffuseFullPath.begin(), diffuseFullPath.end());
			//meshDatas[0].NormalTextureFileFullPath = std::wstring(normalFullPath.begin(), normalFullPath.end());

			//ResourcesManager::InsertOrNull<Mesh>(keys::FEMALE_SOLDER_MESH, std::make_unique<jh::graphics::Mesh>())
			//	->InitVertexIndexBuffer(meshDatas[0].Vertices, meshDatas[0].Indices);
			//std::cout << diffuseFullPath << std::endl;
			//std::cout << normalFullPath << std::endl;
			//loadAndInsertTexture(eTextureType::DIFFUSE, keys::FEMALE_SOLDER_DIFFUSE_TEXTURE, meshDatas[0].DiffuseTextureFileFullPath);
			//loadAndInsertTexture(eTextureType::NORMAL, keys::FEMALE_SOLDER_NORMAL_TEXTURE, meshDatas[0].NormalTextureFileFullPath);
		}
#pragma endregion
	}
	void GraphicsPSOManager::initMaterials()
	{
		insertMaterial(keys::BASIC_3D_MATERIAL_KEY, mBasicPSO);
		insertMaterial(keys::WORLD_COORD_MATERIAL, mDebugDrawWorldCoordPSO);
		insertMaterial(keys::GROUND_MATERIAL_KEY, mBasicPSO);
#pragma region FEMALE_SOLDER
		{
			//insertMaterial(keys::FEMALE_SOLDER_MATERIAL, mBasicPSO, "");
			//Texture* pDiffuseTex = ResourcesManager::Find<Texture>(keys::FEMALE_SOLDER_DIFFUSE_TEXTURE);
			//Texture* pNormalTex = ResourcesManager::Find<Texture>(keys::FEMALE_SOLDER_NORMAL_TEXTURE);
			//Material* pMat = ResourcesManager::Find<Material>(keys::FEMALE_SOLDER_MATERIAL);
			//pMat->SetTexture(eTextureType::DIFFUSE, pDiffuseTex);
			//pMat->SetTexture(eTextureType::NORMAL, pNormalTex);
		}
#pragma endregion


#pragma region BARNY

#pragma endregion


		insertMaterial(keys::CUBE_MAP_MATERIAL, mCubeMapPSO);

		// Normal
		{
			Texture* p3DBasicNormalTex = ResourcesManager::Find<Texture>(keys::BASIC_3D_NORMAL_TEXTURE_KEY);
			Material* pMat = ResourcesManager::Find<Material>(keys::BASIC_3D_MATERIAL_KEY);
		}
	}

	void GraphicsPSOManager::initModels()
	{
#pragma region DEBUG_BOX
		{
			Model* pBoxModel = ResourcesManager::InsertOrNull<jh::graphics::Model>(keys::BOX_MODEL, std::make_unique<Model>());
			std::vector<Mesh*> pMeshes;
			pMeshes.push_back(ResourcesManager::Find<Mesh>(keys::BOX_MESH_KEY));
			pBoxModel->SetMeshes(pMeshes);
			pBoxModel->SetMaterial(*ResourcesManager::Find<Material>(keys::BASIC_3D_MATERIAL_KEY));
		}
#pragma endregion

#pragma region GROUND
		{
			Model* pGroundModel = ResourcesManager::InsertOrNull<jh::graphics::Model>(keys::GROUND_MODEL, std::make_unique<Model>());
			std::vector<Mesh*> pMeshes;
			pMeshes.push_back(ResourcesManager::Find<Mesh>(keys::GROUND_MESH_KEY));
			pGroundModel->SetMeshes(pMeshes);
			pGroundModel->SetMaterial(*ResourcesManager::Find<Material>(keys::GROUND_MATERIAL_KEY));
		}
#pragma endregion



#pragma region DEBUG_SPEHRE
		{
			Model* pSpehereModel = ResourcesManager::InsertOrNull<jh::graphics::Model>(keys::SPEHERE_MODEL_KEY, std::make_unique<Model>());
			std::vector<Mesh*> pMeshes;
			pMeshes.push_back(ResourcesManager::Find<Mesh>(keys::SPEHERE_MESH_KEY));
			pSpehereModel->SetMeshes(pMeshes);
			pSpehereModel->SetMaterial(*ResourcesManager::Find<Material>(keys::BASIC_3D_MATERIAL_KEY));
		}
#pragma endregion

#pragma region DEBUG_WORLD_COORD
		{
			Model* pWorldCoordModel = ResourcesManager::InsertOrNull<jh::graphics::Model>(keys::WORLD_COORD_MODEL, std::make_unique<Model>());
			std::vector<Mesh*> pMeshes;
			pMeshes.push_back(ResourcesManager::Find<Mesh>(keys::WORLD_COORD_MESH_KEY));
			pWorldCoordModel->SetMeshes(pMeshes);
			pWorldCoordModel->SetMaterial(*ResourcesManager::Find<Material>(keys::WORLD_COORD_MATERIAL));
		}
#pragma endregion

#pragma region FEMALE_SOLDER
		{
			//Model* pSolderModel = ResourcesManager::InsertOrNull<jh::graphics::Model>(keys::FEMALE_SOLDER_MODEL, std::make_unique<Model>());
			//std::vector<Material*> pMaterials;
			//std::vector<Mesh*> pMeshs;
			//pMeshs.reserve(1);
			//pMaterials.reserve(1);
			//pMaterials.push_back(ResourcesManager::Find<Material>(keys::FEMALE_SOLDER_MATERIAL));
			//pMeshs.push_back(ResourcesManager::Find<Mesh>(keys::FEMALE_SOLDER_MESH));
			//pSolderModel->SetMaterials(pMaterials);
			//pSolderModel->SetMeshes(pMeshs);
		}
#pragma endregion

#pragma region PLAYER
		{
			auto& geoGenerator = GeomatryGenerator::GetInstance();
			auto& animDataManager = AnimationDataManager::GetInstance();
			std::string relativePath = "Assets\\Characters\\TwoHand\\";
			std::vector<std::string> animClipNames =
			{
				"Idle.fbx",
				"Walk.fbx",
				"BackWalk.fbx",
				"StrafeRight.fbx",
				"StrafeLeft.fbx",
				"Roll.fbx",
				"Kick.fbx",
				"Slash_1.fbx",
				"Slash_2.fbx",
				"Slash_3.fbx",
				"Hitted.fbx"
			};

			//std::vector<std::string> animClipNames =
			//{
			//	"PirateIdle.fbx",
			//	"PirateWalk.fbx",
			//	"BackWalk.fbx",
			//	"PirateStrafeRight.fbx",
			//	"PirateStrafeLeft.fbx",
			//	"Roll.fbx",
			//	"Kick.fbx",
			//	"Slash_1.fbx",
			//	"Slash_2.fbx",
			//	"Slash_3.fbx"
			//};

			auto [meshes, pAnim] = geoGenerator.ReadFBXFile(relativePath, "CharacterTPose.fbx", AnimationDataManager::BASIC_CHARACTER_MORTION_ANIM_DATA_KEY);
			//auto [meshes, pAnim] = geoGenerator.ReadFBXFile(relativePath, "PirateCharacterTPose.fbx", AnimationDataManager::BASIC_CHARACTER_MORTION_ANIM_DATA_KEY);

			ResourcesManager::InsertOrNull<Mesh>(keys::BARNY_MESH_1, std::make_unique<jh::graphics::Mesh>())
				->InitVertexIndexBuffer<jh::graphics::SkinnedVertex>(meshes[0].SkinnedVertices, meshes[0].Indices);
			ResourcesManager::InsertOrNull<Mesh>(keys::BARNY_MESH_2, std::make_unique<jh::graphics::Mesh>())
				->InitVertexIndexBuffer<jh::graphics::SkinnedVertex>(meshes[1].SkinnedVertices, meshes[1].Indices);
			loadAndInsertTexture(eTextureType::DIFFUSE, keys::BARNY_DIFFUSE_TEXTURE, meshes[0].DiffuseTextureFileFullPath);
			loadAndInsertTexture(eTextureType::NORMAL, keys::BARNY_NORMAL_TEXTURE, meshes[0].NormalTextureFileFullPath);
			
			pAnim->ClipMap.reserve(animClipNames.size());
			animDataManager.GetInstance().InsertAnimationClips(eAnimClipKeyContainerType::PLAYER, animClipNames);
			for (auto& name : animClipNames)
			{
				geoGenerator.ParseAnimationClip(relativePath, name, AnimationDataManager::BASIC_CHARACTER_MORTION_ANIM_DATA_KEY, name);
			}

			std::vector<Mesh*> pMeshs;
			pMeshs.reserve(2);
			insertMaterial(keys::BARNY_SKINNNED_MATERIAL_1, mSkinnedBasicPSO);
			Texture* pDiffuseTex = ResourcesManager::Find<Texture>(keys::BARNY_DIFFUSE_TEXTURE);
			Texture* pNormalTex = ResourcesManager::Find<Texture>(keys::BARNY_NORMAL_TEXTURE);
			Material* pMat1 = ResourcesManager::Find<Material>(keys::BARNY_SKINNNED_MATERIAL_1);
			pMeshs.push_back(ResourcesManager::Find<Mesh>(keys::BARNY_MESH_1));
			pMeshs.push_back(ResourcesManager::Find<Mesh>(keys::BARNY_MESH_2));
			for (UINT i = 0; i < pMeshs.size(); ++i)
			{
				pMeshs[i]->SetTextures(pDiffuseTex, pNormalTex);
			}
			// For Debugging
			AnimationData* pReadingData = animDataManager.GetAnimDataOrNull(AnimationDataManager::BASIC_CHARACTER_MORTION_ANIM_DATA_KEY);
			SkinnedMeshModel* pBarnySkinnedModel = static_cast<SkinnedMeshModel*>(ResourcesManager::InsertOrNull<jh::graphics::Model>(keys::BARNY_SKINNED_MODEL, std::make_unique<SkinnedMeshModel>()));
			pBarnySkinnedModel->SetMeshes(pMeshs);
			pBarnySkinnedModel->SetMaterial(*ResourcesManager::Find<Material>(keys::BARNY_SKINNNED_MATERIAL_1));
		}

#pragma endregion

#pragma region MONSTER_MUTANT
		{
			auto& geoGenerator = GeomatryGenerator::GetInstance();
			auto& animDataManager = AnimationDataManager::GetInstance();
			std::string relativePath = "Assets\\Characters\\Monster\\Mutant\\";
			std::vector<std::string> animClipNames =
			{
				"MutantIdle.fbx",
				"MutntWalk.fbx",
				"MutantRun.fbx",
				"MutantRoaring.fbx",
				"MutantSlash.fbx",
				"MutantJumpAttackInplace.fbx",
			};

			auto [meshes, pAnim] = geoGenerator.ReadFBXFile(relativePath, "MutantTPose.fbx", AnimationDataManager::BASIC_MONSTER_MORTION_ANIM_DATA_KEY);

			ResourcesManager::InsertOrNull<Mesh>(keys::MUTANT_MESH_1, std::make_unique<jh::graphics::Mesh>())
				->InitVertexIndexBuffer<jh::graphics::SkinnedVertex>(meshes[0].SkinnedVertices, meshes[0].Indices);
			loadAndInsertTexture(eTextureType::DIFFUSE, keys::MUTANT_DIFFUSE_TEXTURE, meshes[0].DiffuseTextureFileFullPath);
			loadAndInsertTexture(eTextureType::NORMAL, keys::MUTANT_NORMAL_TEXTURE, meshes[0].NormalTextureFileFullPath);
			pAnim->ClipMap.reserve(animClipNames.size());
			animDataManager.GetInstance().InsertAnimationClips(eAnimClipKeyContainerType::MONSTER, animClipNames);
			for (auto& name : animClipNames)
			{
				geoGenerator.ParseAnimationClip(relativePath, name, AnimationDataManager::BASIC_MONSTER_MORTION_ANIM_DATA_KEY, name);
			}

			std::vector<Mesh*> pMeshs;
			pMeshs.reserve(1);
			insertMaterial(keys::MUTANT_SKINNNED_MATERIAL_1, mSkinnedBasicPSO);
			Texture* pDiffuseTex = ResourcesManager::Find<Texture>(keys::MUTANT_DIFFUSE_TEXTURE);
			Texture* pNormalTex = ResourcesManager::Find<Texture>(keys::MUTANT_NORMAL_TEXTURE);
			Material* pMat1 = ResourcesManager::Find<Material>(keys::MUTANT_SKINNNED_MATERIAL_1);
			pMeshs.push_back(ResourcesManager::Find<Mesh>(keys::MUTANT_MESH_1));
			pMeshs[0]->SetTextures(pDiffuseTex, pNormalTex);
			SkinnedMeshModel* pMutantSkinnedModel = static_cast<SkinnedMeshModel*>(ResourcesManager::InsertOrNull<jh::graphics::Model>(keys::MUTANT_SKINNED_MODEL, std::make_unique<SkinnedMeshModel>()));
			pMutantSkinnedModel->SetMeshes(pMeshs);
			pMutantSkinnedModel->SetMaterial(*ResourcesManager::Find<Material>(keys::MUTANT_SKINNNED_MATERIAL_1));
		}
#pragma endregion

	}


	void GraphicsPSOManager::initShaders()
	{
		const int FLOAT4_BYTE = 16;
		const int FLOAT3_BYTE = 12;
		const int FLOAT2_BYTE = 8;
		auto& device = GraphicDeviceDX11::GetInstance().GetDeivce();
#pragma region BASIC_3D_PSO_AND_ENV_PSO
		{
			vector<D3D11_INPUT_ELEMENT_DESC> basicIEs = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, FLOAT3_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, FLOAT3_BYTE + FLOAT3_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, FLOAT3_BYTE + FLOAT3_BYTE + FLOAT2_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			D3D11Utils::CreateVertexShaderAndInputLayout(
				device,
				L"Basic3DVS.hlsl",
				basicIEs,
				mBasicPSO.mcpVertexShader,
				mBasicPSO.mcpInputLayout
			);

			D3D11Utils::CreatePixelShader(
				device,
				L"Basic3DPS.hlsl",
				mBasicPSO.mcpPixelShader
			);


			D3D11Utils::CreateVertexShaderAndInputLayout(
				device,
				L"DebugWorldCoordinateVS.hlsl",
				basicIEs,
				mDebugDrawWorldCoordPSO.mcpVertexShader,
				mDebugDrawWorldCoordPSO.mcpInputLayout
			);

			D3D11Utils::CreatePixelShader(
				device,
				L"DebugWorldCoordinatePS.hlsl",
				mDebugDrawWorldCoordPSO.mcpPixelShader
			);

			mBasicEnvNoNormalPSO.mcpInputLayout = mBasicPSO.mcpInputLayout;
			mBasicEnvNoNormalPSO.mcpVertexShader = mBasicPSO.mcpVertexShader;

			D3D11Utils::CreatePixelShader(
				device,
				L"BasicEnvNoNormalPS.hlsl",
				mBasicEnvNoNormalPSO.mcpPixelShader
			);

			D3D11Utils::CreateVertexShaderAndInputLayout(
				device,
				L"DepthOnlyVS.hlsl",
				basicIEs,
				mDepthOnlyPSO.mcpVertexShader,
				mDepthOnlyPSO.mcpInputLayout
			);
			D3D11Utils::CreatePixelShader(
				device,
				L"DepthOnlyPS.hlsl",
				mDepthOnlyPSO.mcpPixelShader
			);
		}
#pragma endregion


#pragma region SKINNED_PSO
		{
			vector<D3D11_INPUT_ELEMENT_DESC> skinnedIEs =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, FLOAT3_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, FLOAT3_BYTE + FLOAT3_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, FLOAT3_BYTE + FLOAT3_BYTE + FLOAT2_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, FLOAT3_BYTE + FLOAT3_BYTE + FLOAT2_BYTE + FLOAT3_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BLENDWEIGHT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, FLOAT3_BYTE + FLOAT3_BYTE + FLOAT2_BYTE + FLOAT3_BYTE + FLOAT4_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, FLOAT3_BYTE + FLOAT3_BYTE + FLOAT2_BYTE + FLOAT3_BYTE + FLOAT4_BYTE + FLOAT4_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BLENDINDICES", 1, DXGI_FORMAT_R8G8B8A8_UINT, 0,  FLOAT3_BYTE + FLOAT3_BYTE + FLOAT2_BYTE + FLOAT3_BYTE + FLOAT4_BYTE + FLOAT4_BYTE + FLOAT3_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			D3D11Utils::CreateVertexShaderAndInputLayout(
				device,
				L"BasicAnimation3DVS.hlsl",
				skinnedIEs,
				mSkinnedBasicPSO.mcpVertexShader,
				mSkinnedBasicPSO.mcpInputLayout
			);
			D3D11Utils::CreatePixelShader(
				device,
				L"Basic3DPS.hlsl",
				mSkinnedBasicPSO.mcpPixelShader
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
				device,
				L"CubeMappingVS.hlsl",
				cubeMappingIEs,
				mCubeMapping.cpVertexShader,
				mCubeMapping.cpInputLayout
			);

			D3D11Utils::CreatePixelShader(
				device,
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
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, FLOAT3_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, FLOAT3_BYTE + FLOAT3_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, FLOAT3_BYTE + FLOAT3_BYTE + FLOAT2_BYTE,
					D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			D3D11Utils::CreateVertexShaderAndInputLayout(
				device,
				L"DebugNormalVS.hlsl",
				basicIEs,
				mDebugDrawNormalPSO.mcpVertexShader,
				mDebugDrawNormalPSO.mcpInputLayout
			);

			D3D11Utils::CreatePixelShader(
				device,
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
		if (FAILED(hr)) { assert(false); }

		// Shadow
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = 0;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.BorderColor[0] = 100.0f; // Å« Z°ª
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		hr = graphics::GraphicDeviceDX11::GetInstance().GetDeivce().CreateSamplerState(
			&samplerDesc, mcpShadowMappingSampler.GetAddressOf()
		);
		if (FAILED(hr)) { assert(false); }

		graphics::GraphicDeviceDX11::GetInstance().GetDeivceContext().PSSetSamplers(
			0,
			1,
			mcpPointBorderSampler.GetAddressOf()
		);

	}

	void GraphicsPSOManager::initTextures()
	{
		loadAndInsertTexture(eTextureType::DIFFUSE, keys::BASIC_3D_DIFFUSE_BOX_TEXTURE_KEY, L"D:\\3DGamePortfolioJH\\Assets\\Textures\\brickwall.png");
		loadAndInsertTexture(eTextureType::NORMAL, keys::BASIC_3D_NORMAL_TEXTURE_KEY, L"D:\\3DGamePortfolioJH\\Assets\\Textures\\brickwall_normal.png");
	}



	void GraphicsPSOManager::loadAndInsertTexture(const eTextureType eType, const std::string& key, const std::wstring& fileName)
	{
		Texture* pTex = ResourcesManager::Load<Texture>(key, fileName);
		pTex->SetTextureType(eType);
	}

	void GraphicsPSOManager::insertMaterial(const std::string& materialKey, GraphicsPSO& pso)
	{
		Material* pMaterial = ResourcesManager::InsertOrNull<Material>(materialKey, std::make_unique<Material>());
		pMaterial->SetPSO(pso);
	}
	void GraphicsPSOManager::initEnv()
	{
		auto& geoGenerator = GeomatryGenerator::GetInstance();
#pragma region CATSLE
		{
			std::string relativePath = "Assets\\Env\\DungeonTiles\\";
			std::vector<MeshData> meshDatas = geoGenerator.ReadFromFile(relativePath, "DungeonTiles.fbx");
			mspCatsleMeshs.reserve(meshDatas.size());
			mspCatsleTextures.reserve(meshDatas.size());
			mspCatsleMeshs.resize(mspCatsleMeshs.capacity());
			mspCatsleTextures.resize(mspCatsleMeshs.capacity());

			std::cout << "DungeonTiles size : " << meshDatas.size() << std::endl;
			for (UINT i = 0; i < meshDatas.size(); ++i)
			{
				if (meshDatas[i].DiffuseTextureFileFullPath == L"")
				{
					continue;
				}
				std::vector<Vertex3D> vertices;
				std::vector<UINT> indices;
				//std::string cha(meshDatas[i].DiffuseTextureFileFullPath.begin(), meshDatas[i].DiffuseTextureFileFullPath.end());
				//std::cout << i << " " << cha << std::endl;

				// Mesh
				mspCatsleMeshs[i] = std::make_unique<jh::graphics::Mesh>();
				mspCatsleMeshs[i]->InitVertexIndexBuffer<Vertex3D>(meshDatas[i].Vertices, meshDatas[i].Indices);

				// Texture
				mspCatsleTextures[i] = std::make_unique<jh::graphics::Texture>();
				mspCatsleTextures[i]->Load(meshDatas[i].DiffuseTextureFileFullPath);
				mspCatsleTextures[i]->SetTextureType(eTextureType::DIFFUSE);
				mspCatsleMeshs[i]->SetTextures(mspCatsleTextures[i].get(), nullptr);
			}
		}


		{
			Model* pCatsleModel = ResourcesManager::InsertOrNull<jh::graphics::Model>(keys::CATSLE_MODEL_KEY, std::make_unique<Model>());
			std::vector<Mesh*> pMeshes;
			std::vector<Material*> pMaterials;
			pMeshes.reserve(mspCatsleMeshs.size());
			insertMaterial(keys::CATSLE_MATERIAL_KEY, mBasicEnvNoNormalPSO);
			for (UINT i = 0; i < mspCatsleMeshs.size(); ++i)
			{
				if (mspCatsleMeshs[i] == nullptr)
				{
					continue;
				}
				pMeshes.push_back(mspCatsleMeshs[i].get());
			}

			pCatsleModel->SetMeshes(pMeshes);
			pCatsleModel->SetMaterial(*ResourcesManager::Find<Material>(keys::CATSLE_MATERIAL_KEY));
		}
#pragma endregion
	}
	void GraphicsPSOManager::initPipelineStates()
	{
		mBasicPSO.mcpRS = mcpSolidRS;
		mBasicPSO.mcpSampler = mcpPointBorderSampler;

		mDepthOnlyPSO.mcpSampler = mcpShadowMappingSampler;

		mBasicEnvNoNormalPSO.mPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mBasicEnvNoNormalPSO.mcpRS = mcpSolidRS;
		mBasicEnvNoNormalPSO.mcpSampler = mcpPointWrapSampler;

		mSkinnedBasicPSO.mcpRS = mcpSolidRS;
		mSkinnedBasicPSO.mcpSampler = mcpPointWrapSampler;

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
			L"D:\\3DGamePortfolioJH\\Assets\\skybox\\fantasyCubemap.dds",
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
			assert(false);
		}
		hr = DirectX::CreateDDSTextureFromFileEx(
			&gd.GetDeivce(),
			L"D:\\3DGamePortfolioJH\\Assets\\skybox\\fantasyCubeMapDiffuseIBL.dds",
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			D3D11_RESOURCE_MISC_TEXTURECUBE,
			DirectX::DX11::DDS_LOADER_FLAGS(false),
			(ID3D11Resource**)cpTexture.GetAddressOf(),
			mCubeMapping.cpCubeMapDiffuseIBLSRV.ReleaseAndGetAddressOf(),
			nullptr
		);
		if (FAILED(hr))
		{
			assert(false);
		}

		hr = DirectX::CreateDDSTextureFromFileEx(
			&gd.GetDeivce(),
			L"D:\\3DGamePortfolioJH\\Assets\\skybox\\fantasyCubeMapSpecularIBL.dds",
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			D3D11_RESOURCE_MISC_TEXTURECUBE,
			DirectX::DX11::DDS_LOADER_FLAGS(false),
			(ID3D11Resource**)cpTexture.GetAddressOf(),
			mCubeMapping.cpCubeMapSepcularIBLSRV.ReleaseAndGetAddressOf(),
			nullptr
		);
		if (FAILED(hr))
		{
			assert(false);
		}

		std::vector<Vertex3D> vertices;
		std::vector<UINT> indices;

		GeomatryGenerator::GetInstance().MakeBox(vertices, indices, 100.0f);
		std::reverse(indices.begin(), indices.end());
		pCubeMesh->InitVertexIndexBuffer<Vertex3D>(vertices, indices);
	}

}