#pragma once
#include "Common.h"
#include "GraphicResource.h"

namespace jh::graphics
{

class ResourcesManager final
{
public:
	template<typename T>
	static T* Find(const std::string& key)
	{
		auto iter = mResources.find(key);

		if (iter != mResources.end()) { return static_cast<T*>(iter->second.get()); }

		return nullptr;
	}

	template<typename T>
	static T* InsertOrNull(const std::string& key, std::unique_ptr<T> resource)
	{
		assert(resource != nullptr || key == "");
		if (resource == nullptr || key == "") { return nullptr; }
		auto res = mResources.insert(std::make_pair(key, std::move(resource)));
		if (res.second == false)
		{
			assert(false);
		}
		return static_cast<T*>(Find<T>(key));
	}


	template<typename T>
	static T* Load(const std::string& key, const std::wstring& path)
	{
		T* retPResource = Find<T>(key);
		if (retPResource != nullptr) { return retPResource; }

		// IF NOT FOUND Resource
		std::unique_ptr<T> spResource = std::make_unique<T>();
		if (FAILED(spResource->Load(path)))
		{
			assert(false);
			return nullptr;
		}
		spResource->SetKey(key);
		spResource->SetPath(path);
		mResources.insert(std::make_pair(key, std::move(spResource)));

		return static_cast<T*>(Find<T>(key));
	}

private:
	static std::unordered_map<std::string, std::unique_ptr<GraphicResource>> mResources;
};

}