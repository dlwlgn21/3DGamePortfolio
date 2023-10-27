#include "ColliderDebugRenderer.h"
#include <DirectXColors.h>

#include "DebugDraw.h"


using VertexType = DirectX::VertexPositionColor;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace jh::graphics;

namespace jh
{

void ColliderDebugRenderer::setPipeline()
{
	auto& context = GraphicDeviceDX11::GetInstance().GetDeivceContext();
	m_effect->SetView(CameraManager::GetInstance().GetCamera().GetGpuViewMatrixRow());
	m_effect->SetProjection(CameraManager::GetInstance().GetCamera().GetGpuProjectionMatrixRow());
	context.OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context.OMSetDepthStencilState(m_states->DepthNone(), 0);
	context.RSSetState(m_states->CullNone());
	m_effect->Apply(&context);
	context.IASetInputLayout(m_inputLayout.Get());
}

void ColliderDebugRenderer::clearPipeline()
{
	auto& context = GraphicDeviceDX11::GetInstance().GetDeivceContext();
	context.OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	context.OMSetDepthStencilState(nullptr, 0);
	context.RSSetState(nullptr);
	context.IASetInputLayout(nullptr);
}
void ColliderDebugRenderer::TestRender()
{
	m_batch->Begin();
	debug_draw::Draw(m_batch.get(), box, Colors::Red); // BoundingBox
	debug_draw::Draw(m_batch.get(), orientedBox, Colors::Blue); // BoundingOrientedBox
	m_batch->End();
}

void ColliderDebugRenderer::BoundingBoxRender(const DirectX::BoundingBox& box, const XMVECTORF32& colors)
{
	setPipeline();
	m_batch->Begin();
	debug_draw::Draw(m_batch.get(), box, colors);
	m_batch->End();
	clearPipeline();
}
void ColliderDebugRenderer::OrientedBoxRender(const DirectX::BoundingOrientedBox& box, const XMVECTORF32& colors)
{
	setPipeline();
	m_batch->Begin();
	debug_draw::Draw(m_batch.get(), box, colors);
	m_batch->End();
	clearPipeline();
}
void ColliderDebugRenderer::SphereRender(const DirectX::BoundingSphere& sphere, const XMVECTORF32& colors)
{
	setPipeline();
	m_batch->Begin();
	debug_draw::Draw(m_batch.get(), sphere, colors);
	m_batch->End();
	clearPipeline();
}

}