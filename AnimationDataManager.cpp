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

void AnimationDataManager::InsertAnimationClips(const eAnimClipKeyContainerType eKeyContainerType, const std::vector<std::string>& clipKeys)
{
	assert(clipKeys.size() != 0);
	UINT index = static_cast<UINT>(eKeyContainerType);
	mAnimationClipsKeysContainer[index].reserve(clipKeys.size());
	for (UINT i = 0; i < clipKeys.size(); ++i)
	{
		mAnimationClipsKeysContainer[index].push_back(clipKeys[i]);
	}
}

const std::vector<std::string>& AnimationDataManager::GetAnimationCilpKeys(const eAnimClipKeyContainerType eKeyContainerType)
{
	assert(mAnimationClipsKeysContainer[static_cast<UINT>(eKeyContainerType)].size() != 0);
	return mAnimationClipsKeysContainer[static_cast<UINT>(eKeyContainerType)];
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