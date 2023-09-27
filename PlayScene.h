#pragma once
#include "Scene.h"

namespace jh
{
	class PlayScene final : public Scene
	{
	public:
		PlayScene();
		virtual ~PlayScene() = default;

		void Initialize() override;
		void Update() override;

	private:
		void initCamera();
		void initPlayer();
		void initWorldCoord();
		void initGround();
		void initLight();
	};
}


