#pragma once
#include "Common.h"

namespace jh
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity() = default;

		void SetName(const std::string& name)	{ mName = name; }
		const std::string& GetName() const		{ return mName; }
		const UINT32 GetID() const				{ return mID; }

	private:
		std::string mName;
		const UINT32 mID;
	};
}

