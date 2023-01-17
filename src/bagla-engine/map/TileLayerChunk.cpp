#include "TileLayer.h"

TileLayer::Chunk::Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
	const sf::Vector2f& position, const sf::Vector2f& tileCount, const sf::Vector2u& tileSize,
	std::size_t rowSize, TextureResource& tr,
	const std::map<std::uint32_t, tmx::Tileset::Tile>& animTiles) :
	m_AnimTiles(animTiles)
{
	setPosition(position);
	m_LayerOpacity = static_cast<sf::Uint8>(layer.getOpacity() / 1.f * 255.f);
	sf::Color vertColour = sf::Color(200, 200, 200, m_LayerOpacity);
	auto offset = layer.getOffset();
	m_LayerOffset.x = offset.x;
	m_LayerOffset.x = offset.y;
	m_ChunkTileCount.x = tileCount.x;
	m_ChunkTileCount.y = tileCount.y;
	m_MapTileSize = tileSize;
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
		m_ChunkArrays.emplace_back(
			std::make_unique<ChunkArray>(*tr.find(ts->getImagePath())->second, *ts));
	}
	std::size_t xPos = static_cast<std::size_t>(position.x / tileSize.x);
	std::size_t yPos = static_cast<std::size_t>(position.y / tileSize.y);
	for (auto y = yPos; y < yPos + tileCount.y; ++y)
	{
		for (auto x = xPos; x < xPos + tileCount.x; ++x)
		{
			auto idx = (y * rowSize + x);
			m_ChunkTileIDs.emplace_back(tileIDs[idx]);
			m_ChunkColors.emplace_back(vertColour);
		}
	}
	generateTiles(true);
}

void TileLayer::Chunk::generateTiles(bool registerAnimation)
{
	if (registerAnimation)
	{
		m_ActiveAnimations.clear();
	}
	for (const auto& ca : m_ChunkArrays)
	{
		sf::Uint32 idx = 0;
		std::size_t xPos = static_cast<std::size_t>(getPosition().x / m_MapTileSize.x);
		std::size_t yPos = static_cast<std::size_t>(getPosition().y / m_MapTileSize.y);
		for (auto y = yPos; y < yPos + m_ChunkTileCount.y; ++y)
		{
			for (auto x = xPos; x < xPos + m_ChunkTileCount.x; ++x)
			{
				if (idx < m_ChunkTileIDs.size() && m_ChunkTileIDs[idx].ID >= ca->firstGID
					&& m_ChunkTileIDs[idx].ID <= ca->lastGID)
				{
					if (registerAnimation
						&& m_AnimTiles.find(m_ChunkTileIDs[idx].ID) != m_AnimTiles.end())
					{
						AnimationState as;
						as.animTile = m_AnimTiles[m_ChunkTileIDs[idx].ID];
						as.startTime = sf::milliseconds(0);
						as.tileCords = sf::Vector2u(x, y);
						m_ActiveAnimations.push_back(as);
					}

					sf::Vector2f tileOffset(x * m_MapTileSize.x,
						(float)y * m_MapTileSize.y + m_MapTileSize.y - ca->tileSetSize.y);

					auto idIndex = m_ChunkTileIDs[idx].ID - ca->firstGID;
					sf::Vector2f tileIndex(
						idIndex % ca->tileCount.x, idIndex / ca->tileCount.x);
					tileIndex.x *= ca->tileSetSize.x;
					tileIndex.y *= ca->tileSetSize.y;
					Tile tile = {
#ifndef __ANDROID__
						sf::Vertex(tileOffset - getPosition(), m_ChunkColors[idx], tileIndex),
						sf::Vertex(
							tileOffset - getPosition() + sf::Vector2f(ca->tileSetSize.x, 0.f),
							m_ChunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, 0.f)),
						sf::Vertex(tileOffset - getPosition()
								+ sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y),
							m_ChunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y)),
						sf::Vertex(
							tileOffset - getPosition() + sf::Vector2f(0.f, ca->tileSetSize.y),
							m_ChunkColors[idx],
							tileIndex + sf::Vector2f(0.f, ca->tileSetSize.y))
#endif
#ifdef __ANDROID__
							sf::Vertex(tileOffset - getPosition(), m_ChunkColors[idx], tileIndex),
						sf::Vertex(
							tileOffset - getPosition() + sf::Vector2f(ca->tileSetSize.x, 0.f),
							m_ChunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, 0.f)),
						sf::Vertex(tileOffset - getPosition()
								+ sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y),
							m_ChunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y)),
						sf::Vertex(tileOffset - getPosition(), m_ChunkColors[idx], tileIndex),
						sf::Vertex(
							tileOffset - getPosition() + sf::Vector2f(0.f, ca->tileSetSize.y),
							m_ChunkColors[idx],
							tileIndex + sf::Vector2f(0.f, ca->tileSetSize.y)),
						sf::Vertex(tileOffset - getPosition()
								+ sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y),
							m_ChunkColors[idx],
							tileIndex + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y))
#endif
					};
					doFlips(m_ChunkTileIDs[idx].flipFlags,
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

std::vector<TileLayer::AnimationState>& TileLayer::Chunk::getActiveAnimations()
{
	return m_ActiveAnimations;
}

tmx::TileLayer::Tile TileLayer::Chunk::getTile(int x, int y) const
{
	return m_ChunkTileIDs[calcIndexFrom(x, y)];
}

void TileLayer::Chunk::setTile(int x, int y, tmx::TileLayer::Tile tile, bool refresh)
{
	m_ChunkTileIDs[calcIndexFrom(x, y)] = tile;
	maybeRegenerate(refresh);
}

sf::Color TileLayer::Chunk::getColor(int x, int y) const
{
	return m_ChunkColors[calcIndexFrom(x, y)];
}

void TileLayer::Chunk::setColor(int x, int y, sf::Color color, bool refresh)
{
	m_ChunkColors[calcIndexFrom(x, y)] = color;
	maybeRegenerate(refresh);
}

void TileLayer::Chunk::maybeRegenerate(bool refresh)
{
	if (refresh)
	{
		for (const auto& ca : m_ChunkArrays)
		{
			ca->reset();
		}
		generateTiles();
	}
}

int TileLayer::Chunk::calcIndexFrom(int x, int y) const
{
	return x + y * m_ChunkTileCount.x;
}

bool TileLayer::Chunk::empty() const
{
	return m_ChunkArrays.empty();
}

void TileLayer::Chunk::flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
{
	// Flip Y
	sf::Vector2f tmp = *v0;
	v0->y = v2->y;
	v1->y = v2->y;
	v2->y = tmp.y;
	v3->y = v2->y;
}

void TileLayer::Chunk::flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
{
	// Flip X
	sf::Vector2f tmp = *v0;
	v0->x = v1->x;
	v1->x = tmp.x;
	v2->x = v3->x;
	v3->x = v0->x;
}

void TileLayer::Chunk::flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
{
	// Diagonal flip
	sf::Vector2f tmp = *v1;
	v1->x = v3->x;
	v1->y = v3->y;
	v3->x = tmp.x;
	v3->y = tmp.y;
}

void TileLayer::Chunk::doFlips(
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

void TileLayer::Chunk::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	states.transform *= getTransform();
	for (const auto& a : m_ChunkArrays)
	{
		rt.draw(*a, states);
	}
}

void TileLayer::Chunk::ChunkArray::reset()
{
	m_Vertices.clear();
}

void TileLayer::Chunk::ChunkArray::addTile(const Chunk::Tile& tile)
{
	for (const auto& v : tile)
	{
		m_Vertices.push_back(v);
	}
}

sf::Vector2u  TileLayer::Chunk::ChunkArray::getTextureSize() const
{
	return m_Texture.getSize();
}

TileLayer::Chunk::ChunkArray::ChunkArray(const sf::Texture& t, const tmx::Tileset& ts) : m_Texture(t)
{
	auto texSize = getTextureSize();
	tileSetSize = ts.getTileSize();
	tileCount.x = texSize.x / tileSetSize.x;
	tileCount.y = texSize.y / tileSetSize.y;
	firstGID = ts.getFirstGID();
	lastGID = ts.getLastGID();
}