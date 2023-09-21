#include "MoveableGameObject.h"

namespace jh
{


class Player final : public MoveableGameObject
{
public:
	Player();
	virtual ~Player() = default;

	static const std::string IDLE_ANIM_KEY;
	static const std::string RUN_ANIM_KEY;
	static const std::string PLAYER_ROLLING_KEY;
	static const std::string PLAYER_DASH_KEY;
	static const std::string ATTACK_1_ANIM_KEY;
	static const std::string ATTACK_2_ANIM_KEY;
	static const std::string ATTACK_3_ANIM_KEY;
	static const std::string ATTACK_PUSH_ANIM_KEY;
	static const std::string PLAYER_HITTED_KEY;

private:
	void initRenderer() override;
	void initScript() override;
	void initAnimator() override;
	void initPhysics() override;

};


}
