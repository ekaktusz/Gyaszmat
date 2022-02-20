#include "MapLayer.h"

MapLayer::Chunk::Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
	const sf::Vector2f& position, const sf::Vector2f& tileCount, const sf::Vector2u& tileSize,
	std::size_t rowSize, TextureResource& tr,
	const std::map<std::uint32_t, tmx::Tileset::Tile>& animTiles) :
	m_animTiles(animTiles)
{
	setPosition(position);
	layerOpacity = static_cast<sf::Uint8>(layer.getOpacity() / 1.f * 255.f);
	sf::Color vertColour = sf::Color(200, 200, 200, layerOpacity);
	auto offset = layer.getOffset();
	layerOffset.x = offset.x;
	layerOffset.x = offset.y;
	chunkTileCount.x = tileCount.x;
	chunkTileCount.y = tileCount.y;
	mapTileSize = tileSize;
	const auto& tileIDs = layer.getTiles();

	// go through the tiles and create all arrays (for latter manipulation)
	for (const auto& ts : tilesets)
	{
		if (ts->getImagePath().empty())
		{
			tmx::Logger::log("This example does not support Collection of Images tilesets",
				tmx::Logger::Type::Info);
			tmx::Logger::log(
				"Chunks using " + ts->getName() + " will not be created", tmx::Logger::Type::Info);
			continue;
		}
		m_chunkArrays.emplace_back(
			std::make_unique<ChunkArray>(*tr.find(ts->getImagePath())->second, *ts));
	}
	std::size_t xPos = static_cast<std::size_t>(position.x / tileSize.x);
	std::size_t yPos = static_cast<std::size_t>(position.y / tileSize.y);
	for (auto y = yPos; y < yPos + tileCount.y; ++y)
	{
		for (auto x = xPos; x < xPos + tileCount.x; ++x)
		{
			auto idx = (y * rowSize + x);
			m_chunkTileIDs.emplace_back(tileIDs[idx]);
			m_chunkColors.emplace_back(vertColour);
		}
	}
	generateTiles(true);
}

void MapLayer::Chunk::generateTiles(bool registerAnimation)
{
	if (registerAnimation)
	{
		m_activeAnimations.clear();
	}
	for (const auto& ca : m_chunkArrays)
	{
		sf::Uint32 idx = 0;
		std::size_t xPos = static_cast<std::size_t>(getPosition().x / mapTileSize.x);
		std::size_t yPos = static_cast<std::size_t>(getPosition().y / mapTileSize.y);
		for (auto y = yPos; y < yPos + chunkTileCount.y; ++y)
		{
			for (auto x = xPos; x < xPos + chunkTileCount.x; ++x)
			{
				if (idx < m_chunkTileIDs.size() && m_chunkTileIDs[idx].ID >= ca->m_firstGID
					&& m_chunkTileIDs[idx].ID <= ca->m_lastGID)
				{
					if (registerAnimation
						&& m_animTiles.find(m_chunkTileIDs[idx].ID) != m_animTiles.end())
					{
						AnimationState as;
						as.animTile = m_animTiles[m_chunkTileIDs[idx].ID];
						as.startTime = sf::milliseconds(0);
						as.tileCords = sf::Vector2u(x, y);
						m_activeAnimations.push_back(as);
					}

					sf::Vector2f tileOffset(x * mapTileSize.x,
						(float)y * mapTileSize.y + mapTileSize.y - ca->tileSetSize.y);

					auto idIndex = m_chunkTileIDs[idx].ID - ca->m_firstGID;
					sf::Vector2f tileIndex(
						idIndex % ca->tsTileCount.x, idIndex / ca->tsTileCount.x);
					tileIndex.x *= ca->tileSetSize.x;
					tileIndex.y *= ca->tileSetSize.y;
					Tile tile = {
#ifndef __ANDROID__
						sf::Vertex(tileOffset - getPosition(), m_chunkColors[idx], tileIndex),
						sf::Vertex(
							tileOffset - getPosition() + sf::Vector2f(ca->tileSetSize.x, 0.f),
							m_chunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, 0.f)),
						sf::Vertex(tileOffset - getPosition()
								+ sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y),
							m_chunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y)),
						sf::Vertex(
							tileOffset - getPosition() + sf::Vector2f(0.f, ca->tileSetSize.y),
							m_chunkColors[idx],
							tileIndex + sf::Vector2f(0.f, ca->tileSetSize.y))
#endif
#ifdef __ANDROID__
							sf::Vertex(tileOffset - getPosition(), m_chunkColors[idx], tileIndex),
						sf::Vertex(
							tileOffset - getPosition() + sf::Vector2f(ca->tileSetSize.x, 0.f),
							m_chunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, 0.f)),
						sf::Vertex(tileOffset - getPosition()
								+ sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y),
							m_chunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y)),
						sf::Vertex(tileOffset - getPosition(), m_chunkColors[idx], tileIndex),
						sf::Vertex(
							tileOffset - getPosition() + sf::Vector2f(0.f, ca->tileSetSize.y),
							m_chunkColors[idx],
							tileIndex + sf::Vector2f(0.f, ca->tileSetSize.y)),
						sf::Vertex(tileOffset - getPosition()
								+ sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y),
							m_chunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y))
#endif
					};
					doFlips(m_chunkTileIDs[idx].flipFlags,
						&tile[0].texCoords,
						&tile[1].texCoords,
						&tile[2].texCoords,
						&tile[3].texCoords);
					ca->addTile(tile);
				}
				idx++;
			}
		}
	}
}

std::vector<MapLayer::AnimationState>& MapLayer::Chunk::getActiveAnimations()
{
	return this->m_activeAnimations;
}

tmx::TileLayer::Tile MapLayer::Chunk::getTile(int x, int y) const
{
	return m_chunkTileIDs[calcIndexFrom(x, y)];
}

void MapLayer::Chunk::setTile(int x, int y, tmx::TileLayer::Tile tile, bool refresh)
{
	m_chunkTileIDs[calcIndexFrom(x, y)] = tile;
	maybeRegenerate(refresh);
}

sf::Color MapLayer::Chunk::getColor(int x, int y) const
{
	return m_chunkColors[calcIndexFrom(x, y)];
}

void MapLayer::Chunk::setColor(int x, int y, sf::Color color, bool refresh)
{
	m_chunkColors[calcIndexFrom(x, y)] = color;
	maybeRegenerate(refresh);
}

void MapLayer::Chunk::maybeRegenerate(bool refresh)
{
	if (refresh)
	{
		for (const auto& ca : m_chunkArrays)
		{
			ca->reset();
		}
		generateTiles();
	}
}

int MapLayer::Chunk::calcIndexFrom(int x, int y) const
{
	return x + y * chunkTileCount.x;
}

bool MapLayer::Chunk::empty() const
{
	return m_chunkArrays.empty();
}

void MapLayer::Chunk::flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
{
	// Flip Y
	sf::Vector2f tmp = *v0;
	v0->y = v2->y;
	v1->y = v2->y;
	v2->y = tmp.y;
	v3->y = v2->y;
}

void MapLayer::Chunk::flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
{
	// Flip X
	sf::Vector2f tmp = *v0;
	v0->x = v1->x;
	v1->x = tmp.x;
	v2->x = v3->x;
	v3->x = v0->x;
}

void MapLayer::Chunk::flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
{
	// Diagonal flip
	sf::Vector2f tmp = *v1;
	v1->x = v3->x;
	v1->y = v3->y;
	v3->x = tmp.x;
	v3->y = tmp.y;
}

void MapLayer::Chunk::doFlips(
	std::uint8_t bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
{
	// 0000 = no change
	// 0100 = vertical = swap y axis
	// 1000 = horizontal = swap x axis
	// 1100 = horiz + vert = swap both axes = horiz+vert = rotate 180 degrees
	// 0010 = diag = rotate 90 degrees right and swap x axis
	// 0110 = diag+vert = rotate 270 degrees right
	// 1010 = horiz+diag = rotate 90 degrees right
	// 1110 = horiz+vert+diag = rotate 90 degrees right and swap y axis
	if (!(bits & tmx::TileLayer::FlipFlag::Horizontal)
		&& !(bits & tmx::TileLayer::FlipFlag::Vertical)
		&& !(bits & tmx::TileLayer::FlipFlag::Diagonal))
	{
		// Shortcircuit tests for nothing to do
		return;
	}
	else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal)
		&& (bits & tmx::TileLayer::FlipFlag::Vertical)
		&& !(bits & tmx::TileLayer::FlipFlag::Diagonal))
	{
		// 0100
		flipY(v0, v1, v2, v3);
	}
	else if ((bits & tmx::TileLayer::FlipFlag::Horizontal)
		&& !(bits & tmx::TileLayer::FlipFlag::Vertical)
		&& !(bits & tmx::TileLayer::FlipFlag::Diagonal))
	{
		// 1000
		flipX(v0, v1, v2, v3);
	}
	else if ((bits & tmx::TileLayer::FlipFlag::Horizontal)
		&& (bits & tmx::TileLayer::FlipFlag::Vertical)
		&& !(bits & tmx::TileLayer::FlipFlag::Diagonal))
	{
		// 1100
		flipY(v0, v1, v2, v3);
		flipX(v0, v1, v2, v3);
	}
	else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal)
		&& !(bits & tmx::TileLayer::FlipFlag::Vertical)
		&& (bits & tmx::TileLayer::FlipFlag::Diagonal))
	{
		// 0010
		flipD(v0, v1, v2, v3);
	}
	else if (!(bits & tmx::TileLayer::FlipFlag::Horizontal)
		&& (bits & tmx::TileLayer::FlipFlag::Vertical)
		&& (bits & tmx::TileLayer::FlipFlag::Diagonal))
	{
		// 0110
		flipX(v0, v1, v2, v3);
		flipD(v0, v1, v2, v3);
	}
	else if ((bits & tmx::TileLayer::FlipFlag::Horizontal)
		&& !(bits & tmx::TileLayer::FlipFlag::Vertical)
		&& (bits & tmx::TileLayer::FlipFlag::Diagonal))
	{
		// 1010
		flipY(v0, v1, v2, v3);
		flipD(v0, v1, v2, v3);
	}
	else if ((bits & tmx::TileLayer::FlipFlag::Horizontal)
		&& (bits & tmx::TileLayer::FlipFlag::Vertical)
		&& (bits & tmx::TileLayer::FlipFlag::Diagonal))
	{
		// 1110
		flipY(v0, v1, v2, v3);
		flipX(v0, v1, v2, v3);
		flipD(v0, v1, v2, v3);
	}
}

void MapLayer::Chunk::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	states.transform *= getTransform();
	for (const auto& a : m_chunkArrays)
	{
		rt.draw(*a, states);
	}
}