#pragma once
#include <box2d/b2_draw.h>

namespace jh
{

class Box2DDebugDrawManager final : public b2Draw
{
public:
	static Box2DDebugDrawManager& GetInstance()
	{
		static Box2DDebugDrawManager instance;
		return instance;
	}

	Box2DDebugDrawManager(const Box2DDebugDrawManager& other) = delete;
	Box2DDebugDrawManager& operator=(const Box2DDebugDrawManager& other) = delete;

	void Initialize();

	/// Draw a closed polygon provided in CCW order.
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	/// Draw a circle.
	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

	/// Draw a line segment.
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	void DrawTransform(const b2Transform& xf) override;

	/// Draw a point.
	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

private:
	void drawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

private:
	Box2DDebugDrawManager() = default;
	~Box2DDebugDrawManager() = default;
};

}

