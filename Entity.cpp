#include "Entity.h"

namespace jh
{
	Entity::Entity()
		: mID(reinterpret_cast<UINT32>(this))
		, mName()
	{
	}
}