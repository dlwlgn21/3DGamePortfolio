#include "ResourcesManager.h"

namespace jh::graphics
{
	std::unordered_map<std::string, std::unique_ptr<GraphicResource>> ResourcesManager::mResources;
}