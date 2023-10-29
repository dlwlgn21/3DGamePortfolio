#pragma once
#include "GraphicsCommon.h"
#include <directxtk/PrimitiveBatch.h>
#include <directxtk/VertexTypes.h>
#include <DirectXCollision.h>
#include <directxtk/Effects.h>
#include <directxtk/CommonStates.h>
#include <directxtk/DirectXHelpers.h>
#include "GraphicDeviceDX11.h"
#include "CameraManager.h"
#include "Camera.h"

using VertexType = DirectX::VertexPositionColor;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace jh::graphics;

namespace jh
{
class ColliderDebugRenderer final
{
public:
	static ColliderDebugRenderer& GetInstance()
	{
		static ColliderDebugRenderer instance;
		return instance;
	}

	ColliderDebugRenderer(const ColliderDebugRenderer& other) = delete;
	ColliderDebugRenderer& operator=(const ColliderDebugRenderer& other) = delete;

	void BoundingBoxRender(const DirectX::BoundingBox& box, const XMVECTORF32& colors);
	void OrientedBoxRender(const DirectX::BoundingOrientedBox& box, const XMVECTORF32& colors);
	void SphereRender(const DirectX::BoundingSphere& sphere, const XMVECTORF32& colors);
	void RayRender(const DirectX::SimpleMath::Vector3& origin, const DirectX::SimpleMath::Vector3& direction, const XMVECTORF32& colors);
private: 
	void setPipeline();
	void clearPipeline();
private:
	ColliderDebugRenderer()
	{
		auto& gd = GraphicDeviceDX11::GetInstance();
		m_states = std::make_unique<CommonStates>(&gd.GetDeivce());
		m_effect = std::make_unique<BasicEffect>(&gd.GetDeivce());
		m_effect->SetVertexColorEnabled(true);

		HRESULT hr;

		// For ColliderRendering
		{
			void const* shaderByteCode;
			size_t byteCodeLength;

			m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

			hr = gd.GetDeivce().CreateInputLayout(
					VertexPositionColor::InputElements, 
					VertexPositionColor::InputElementCount,
					shaderByteCode, 
					byteCodeLength,
					m_inputLayout.ReleaseAndGetAddressOf()
			);
		}
		if (FAILED(hr)) { assert(false); }
		m_batch = std::make_unique<PrimitiveBatch<VertexType>>(&gd.GetDeivceContext());
	}
	~ColliderDebugRenderer() = default;

private:
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};


}

