#include "AnimationDataManager.h"

namespace jh
{
	const std::string AnimationDataManager::BASIC_CHARACTER_MORTION_ANIM_DATA_KEY = "BasicMotionKey";


bool AnimationDataManager::InsertAnimationData(const std::string& key, std::unique_ptr<jh::graphics::AnimationData> animDataUniquePtr)
{
	assert(key != "");
	auto res = mAnimationDataMap.insert(std::make_pair(key, std::move(animDataUniquePtr)));
	if (res.second == false)
	{
		assert(false);
	}
	return true;
}

jh::graphics::AnimationData* AnimationDataManager::GetAnimDataOrNull(const std::string& animDataKey)
{
	auto iter = mAnimationDataMap.find(animDataKey);

	if (iter != mAnimationDataMap.end())  
	{ 
		return iter->second.get();  
	}
	else 
	{ 
		return nullptr; 
	}
}

}