#pragma once
#include <unordered_map>
#include <memory>
#include "AnimationData.h"

namespace jh
{

class AnimationDataManager final
{
public:
	static AnimationDataManager& GetInstance()
	{
		static AnimationDataManager instance;
		return instance;
	}

	AnimationDataManager(const AnimationDataManager& other) = delete;
	AnimationDataManager& operator=(const AnimationDataManager& other) = delete;

	bool InsertAnimationData(const std::string& key, std::unique_ptr<jh::graphics::AnimationData> animDataUniquePtr);

	jh::graphics::AnimationData* GetAnimDataOrNull(const std::string& animDataKey);

private:
	AnimationDataManager() = default;
	~AnimationDataManager()
	{
		mAnimationDataMap.clear();
	}

private:
	std::unordered_map<std::string, std::unique_ptr<jh::graphics::AnimationData>> mAnimationDataMap;

};

}


