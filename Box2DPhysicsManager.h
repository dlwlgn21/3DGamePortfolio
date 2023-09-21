#pragma once
#include "Common.h"

class b2World;
class b2Draw;
namespace jh
{


	class Box2DManager final
	{
	public:
		static Box2DManager& GetInstance()
		{
			static Box2DManager instance;
			return instance;
		}

		Box2DManager(const Box2DManager& other) = delete;
		Box2DManager& operator=(const Box2DManager& other) = delete;

		void Initialize();
		void Update();
		void FixedUpdate();
		void Render();
		void Release();

		b2World& GetB2World();
		const int32_t GetVelocityInterations() const { return mVelocityInterations; }
		const int32_t GetPositionInterations() const { return mPositionInterations; }
	private:
		void updateGameObjectTransformAfterBox2DSimulating();

	private:
		Box2DManager() = default;
		~Box2DManager() = default;

	private:
		b2World* mpB2World = nullptr;
		b2Draw* mpDebugDraw = nullptr;
		const int32_t mVelocityInterations = 6;
		const int32_t mPositionInterations = 2;
		const float mFixedDeltaTime = 1 / 60.0f;
	};
}

