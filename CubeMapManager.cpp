#include <directxtk/SimpleMath.h>

#include "CubeMapManager.h"
#include "GraphicsPSOManager.h"
#include "ResourcesManager.h"
#include "GraphicsKeyContainer.h"
#include "GraphicDeviceDX11.h"


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
		auto& psoManager = GraphicsPSOManager::GetInstance();
		auto& gd = GraphicDeviceDX11::GetInstance().GetDeivceContext();
		auto& gpuBuffer = psoManager.GetConstantBuffer(eCBType::TRANSFORM);
		TransformConstantCPUBuffer cpuBuffer;
		ZeroMemory(&cpuBuffer, sizeof(TransformConstantCPUBuffer));
		cpuBuffer.WorldMat = Matrix::Identity; 
		cpuBuffer.ViewMat = Camera::GetGpuViewMatrixRow().Transpose();
		cpuBuffer.ProjectionMat = Camera::GetGpuProjectionMatrixRow().Transpose();
		gpuBuffer.UpdateBuffer(&cpuBuffer);

		mpMaterial->SetPipeline();
		ID3D11ShaderResourceView* pSrvs[2] =
		{
			psoManager.mCubeMapping.cpCubeMapDiffuseIBLSRV.Get(),
			psoManager.mCubeMapping.cpCubeMapSepcularIBLSRV.Get()
		};
		gd.PSSetShaderResources(0, 2, pSrvs);
		mpMesh->Render();

	}
}