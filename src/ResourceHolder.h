#pragma once

#include "pch.h"

// Class for holding resources based on the SFML book (highly modified tho)
// Abstract, we should inherit from it. Load resource into a map and stores it.
// In theory it should be memory safe. If asset not found will produce runtime error.

template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
	ResourceHolder() = default;
	virtual void load(Identifier id, const std::filesystem::path& filePath) = 0;
	Resource& get(Identifier id) const;
protected:
	void insert(Identifier id, std::unique_ptr<Resource> resource);

	std::map<Identifier, std::unique_ptr<Resource>> resourceMap;
};

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insert(Identifier id, std::unique_ptr<Resource> resource)
{
	resourceMap.try_emplace(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = resourceMap.find(id);
	if (found == resourceMap.end())
	{
		SPDLOG_ERROR("Cant find resource by id: " + std::to_string((int) id));
	}
	return *found->second;
}
