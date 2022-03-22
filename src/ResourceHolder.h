#pragma once

#include "pch.h"

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
	void load(Identifier id, const std::filesystem::path& filePath);
	const Resource& get(Identifier id) const;
private:
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::filesystem::path& filePath)
{
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filePath.string()))
		SPDLOG_ERROR("Failed to load " + filePath.string());

	// If loading successful, insert resource to map
	mResourceMap.try_emplace(id, std::move(resource));
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	if (found == mResourceMap.end())
	{
		SPDLOG_ERROR("Cant find resource by id: " + std::to_string((int) id));
	}
	return *found->second;
}
