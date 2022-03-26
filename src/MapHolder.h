#pragma once
#include "ResourceHolder.h"
#include "ResourceTypes.h"
#include "pch.h"

class MapHolder : public ResourceHolder<tmx::Map, res::Map>
{
public:
	void load(res::Map id, const std::filesystem::path& filePath) override
	{
		tmx::Map* map = new tmx::Map();
		if (!map->load(filePath.string()))
		{
			SPDLOG_ERROR("Failed to parse tiled map: " + filePath.string());
			return;
		}
		SPDLOG_INFO("Map loaded succesfully: " + filePath.string());
		this->insert(id, map);
	}
};
