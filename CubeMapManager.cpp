#include <directxtk/SimpleMath.h>

#include "CubeMapManager.h"
#include "GraphicsPSOManager.h"
#include "ResourcesManager.h"
#include "GraphicsKeyContainer.h"



#include "Camera.h"

using namespace jh::graphics;
using namespace DirectX::SimpleMath;

namespace jh
{
	void CubeMapManager::Initialize()
	{
		mpMaterial = ResourcesManager::Find<Material>(keys::CUBE_MAP_MATERIAL);
		mpMesh = ResourcesManager::Find<Mesh>(keys::CUBE_MAP_MESH);
	}


	void CubeMapManager::Render()
	{
		auto& gpuBuffer = GraphicsPSOManager::GetInstance().GetConstantBuffer(eCBType::TRANSFORM);
		TransformConstantCPUBuffer cpuBuffer;
		ZeroMemory(&cpuBuffer, sizeof(TransformConstantCPUBuffer));
		cpuBuffer.WorldMat = Matrix::Identity; 
		cpuBuffer.ViewMat = Camera::GetGpuViewMatrixRow().Transpose();
		cpuBuffer.ProjectionMat = Camera::GetGpuProjectionMatrixRow().Transpose();
		gpuBuffer.UpdateBuffer(&cpuBuffer);

		mpMaterial->SetPipeline();
		mpMesh->Render();

	}
}