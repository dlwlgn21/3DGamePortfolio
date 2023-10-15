#pragma once
#include <unordered_map>
#include <memory>
#include "AnimationData.h"

namespace jh
{

enum class eAnimClipKeyContainerType
{
	PLAYER,
	COUNT
};

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
	void InsertAnimationClips(const eAnimClipKeyContainerType eKeyContainerType, const std::vector<std::string>& clipKeys);

	const std::vector<std::string>& GetAnimationCilpKeys(const eAnimClipKeyContainerType eKeyContainerType);

	jh::graphics::AnimationData* GetAnimDataOrNull(const std::string& animDataKey);

public:
	static const std::string BASIC_CHARACTER_MORTION_ANIM_DATA_KEY;

private:
	AnimationDataManager()
	{
		mAnimationClipsKeysContainer.reserve(static_cast<UINT>(eAnimClipKeyContainerType::COUNT));
		mAnimationClipsKeysContainer.resize(mAnimationClipsKeysContainer.capacity());
	}
	~AnimationDataManager()
	{
		mAnimationDataMap.clear();
		mAnimationClipsKeysContainer.clear();
	}

private:
	std::unordered_map<std::string, std::unique_ptr<jh::graphics::AnimationData>> mAnimationDataMap;
	std::vector<std::vector<std::string>> mAnimationClipsKeysContainer;
};

}


