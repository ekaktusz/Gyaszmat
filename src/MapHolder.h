#pragma once

#include "pch.h"
#include "ResourceHolder.h"
#include "ResourceTypes.h"

class MapHolder : public ResourceHolder<tmx::Map, res::Map>
{
public:
	void load(res::Map id, const std::filesystem::path& filePath) override
	{
		std::unique_ptr<tmx::Map> map = std::make_unique<tmx::Map>();
		if (!map->load(filePath.string()))
		{
			SPDLOG_ERROR("Failed to parse tiled map: " + filePath.string());
			return;
		}
		SPDLOG_INFO("Map loaded succesfully: " + filePath.string());
		this->insert(id, std::move(map));
	}
};
