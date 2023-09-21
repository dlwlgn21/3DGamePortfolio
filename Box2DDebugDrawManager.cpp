#include "Box2DDebugDrawManager.h"
#include <directxtk/SimpleMath.h>
#include "GraphicsDatas.h"
#include "GraphicsPSOManager.h"
#include "GraphicDeviceDX11.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;
using namespace jh::graphics;

namespace jh
{

void Box2DDebugDrawManager::Initialize()
{
	Box2DDebugDrawManager::GetInstance().SetFlags(b2Draw::e_shapeBit);
}
void Box2DDebugDrawManager::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	drawPolygon(vertices, vertexCount, color);
}


void Box2DDebugDrawManager::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	drawPolygon(vertices, vertexCount, color);
}

void Box2DDebugDrawManager::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
}


void Box2DDebugDrawManager::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
}
void Box2DDebugDrawManager::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
}
void Box2DDebugDrawManager::DrawTransform(const b2Transform& xf)
{
}
void Box2DDebugDrawManager::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
}


void Box2DDebugDrawManager::drawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	assert(vertexCount == 4);
	auto& pso = GraphicsPSOManager::GetInstance();
	auto& gdc = GraphicDeviceDX11::GetInstance().GetDeivceContext();
	auto& box2DPSO = pso.mBox2DebugPSO;
	std::vector<Box2DVertex> boxVertices;
	boxVertices.reserve(vertexCount);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		Box2DVertex vertex;
		vertex.Pos.x = vertices[i].x;
		vertex.Pos.y = vertices[i].y;
		vertex.Pos.z = -0.5f;

		vertex.Color.x = color.r;
		vertex.Color.y = color.g;
		vertex.Color.z = color.b;
		boxVertices.push_back(vertex);
	}

	auto& cb = pso.GetConstantBuffer(eCBType::DEBUG_TRANSFORM);
	DebugTransformConstantCPUBuffer cpuBuffer;
	ZeroMemory(&cpuBuffer, sizeof(DebugTransformConstantCPUBuffer));
	cpuBuffer.ViewMat = Camera::GetGpuViewMatrixRow().Transpose();
	cpuBuffer.ProjectionMat = Camera::GetGpuProjectionMatrixRow().Transpose();
	cb.UpdateBuffer(&cpuBuffer);
	pso.GetBox2DebugMesh().UpdateVertexBuffer(boxVertices);


	gdc.IASetInputLayout(box2DPSO.mcpInputLayout.Get());
	gdc.IASetPrimitiveTopology(box2DPSO.mPrimitiveTopology);

	gdc.RSSetState(box2DPSO.mcpRS.Get());

	gdc.VSSetShader(box2DPSO.mcpVertexShader.Get(), nullptr, 0);
	gdc.PSSetShader(box2DPSO.mcpPixelShader.Get(), nullptr, 0);

	pso.GetBox2DebugMesh().Render();
}

}

