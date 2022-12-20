#include "TileLayer.h"

TileLayer::TileLayer(const tmx::Map& map, MapLayerNames::TileLayerName name)
{
	int idx = static_cast<int>(name);
	const auto& layers = map.getLayers();
	if (map.getOrientation() == tmx::Orientation::Orthogonal && idx < layers.size()
		&& layers[idx]->getType() == tmx::Layer::Type::Tile)
	{

		// round the chunk size to the nearest tile
		const auto tileSize = map.getTileSize();
		m_ChunkSize.x = std::floor(m_ChunkSize.x / tileSize.x) * tileSize.x;
		m_ChunkSize.y = std::floor(m_ChunkSize.y / tileSize.y) * tileSize.y;
		m_MapTileSize.x = map.getTileSize().x;
		m_MapTileSize.y = map.getTileSize().y;
		const auto& layer = layers[idx]->getLayerAs<tmx::TileLayer>();
		createChunks(map, layer);

		auto mapSize = map.getBounds();
		m_GlobalBounds.width = mapSize.width;
		m_GlobalBounds.height = mapSize.height;
		SPDLOG_INFO("TileLayer: {}", idx);
	}
	else
	{
		SPDLOG_ERROR("Not a valid orthogonal layer, nothing will be drawn.");
	}
}

const sf::FloatRect& TileLayer::getGlobalBounds() const
{
	return m_GlobalBounds;
}

void TileLayer::setTile(int tileX, int tileY, tmx::TileLayer::Tile tile, bool refresh)
{
	sf::Vector2u chunkLocale;
	const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
	selectedChunk->setTile(chunkLocale.x, chunkLocale.y, tile, refresh);
}

tmx::TileLayer::Tile TileLayer::getTile(int tileX, int tileY)
{
	sf::Vector2u chunkLocale;
	const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
	return selectedChunk->getTile(chunkLocale.x, chunkLocale.y);
}

void TileLayer::setColor(int tileX, int tileY, sf::Color color, bool refresh)
{
	sf::Vector2u chunkLocale;
	const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
	selectedChunk->setColor(chunkLocale.x, chunkLocale.y, color, refresh);
}

sf::Color TileLayer::getColor(int tileX, int tileY)
{
	sf::Vector2u chunkLocale;
	const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
	return selectedChunk->getColor(chunkLocale.x, chunkLocale.y);
}

void TileLayer::update(sf::Time elapsed)
{
	for (auto& c : m_VisibleChunks)
	{
		for (AnimationState& as : c->getActiveAnimations())
		{
			as.currentTime += elapsed;

			tmx::TileLayer::Tile tile;
			tile.ID = as.animTile.animation.frames[0].tileID;
			tile.flipFlags = 0; // TODO: get flipFlags from original tmx::TileLayer::Tile

			std::uint32_t animTime = 0;
			for (const auto& frame : as.animTile.animation.frames)
			{
				animTime += frame.duration;
				if (as.currentTime.asMilliseconds() >= animTime)
				{
					tile.ID = frame.tileID;
					if (frame == as.animTile.animation.frames.back())
					{
						as.currentTime = sf::milliseconds(0);
					}
				}
			}

			setTile(as.tileCords.x, as.tileCords.y, tile);
		}
	}
}

void TileLayer::createChunks(const tmx::Map& map, const tmx::TileLayer& layer)
{
	// look up all the tile sets and load the textures
	const auto& tileSets = map.getTilesets();
	const auto& layerIDs = layer.getTiles();
	std::uint32_t maxID = std::numeric_limits<std::uint32_t>::max();
	std::vector<const tmx::Tileset*> usedTileSets;

	for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i)
	{
		for (const auto& tile : layerIDs)
		{
			if (tile.ID >= i->getFirstGID() && tile.ID < maxID)
			{
				usedTileSets.push_back(&(*i));
				break;
			}
		}
		maxID = i->getFirstGID();
	}

	sf::Image fallback;
	fallback.create(2, 2, sf::Color::Magenta);
	for (const auto& ts : usedTileSets)
	{
		const auto& path = ts->getImagePath();
		// std::unique_ptr<sf::Texture> newTexture = std::make_unique<sf::Texture>();
		std::unique_ptr<sf::Texture> newTexture = std::make_unique<sf::Texture>();
		sf::Image img;
		if (!img.loadFromFile(path))
		{
			newTexture->loadFromImage(fallback);
		}
		else
		{
			if (ts->hasTransparency())
			{
				auto transparency = ts->getTransparencyColour();
				img.createMaskFromColor(
					{ transparency.r, transparency.g, transparency.b, transparency.a });
			}
			newTexture->loadFromImage(img);
		}
		m_TextureResource.insert(std::make_pair(path, std::move(newTexture)));
	}

	// calculate the number of chunks in the layer
	// and create each one
	const auto bounds = map.getBounds();
	m_ChunkCount.x = static_cast<sf::Uint32>(std::ceil(bounds.width / m_ChunkSize.x));
	m_ChunkCount.y = static_cast<sf::Uint32>(std::ceil(bounds.height / m_ChunkSize.y));

	sf::Vector2u tileSize(map.getTileSize().x, map.getTileSize().y);

	for (auto y = 0u; y < m_ChunkCount.y; ++y)
	{
		sf::Vector2f tileCount(m_ChunkSize.x / tileSize.x, m_ChunkSize.y / tileSize.y);
		for (auto x = 0u; x < m_ChunkCount.x; ++x)
		{
			// calculate size of each Chunk (clip against map)
			if ((x + 1) * m_ChunkSize.x > bounds.width)
			{
				tileCount.x = (bounds.width - x * m_ChunkSize.x) / map.getTileSize().x;
			}
			if ((y + 1) * m_ChunkSize.y > bounds.height)
			{
				tileCount.y = (bounds.height - y * m_ChunkSize.y) / map.getTileSize().y;
			}
			// m_chunks.emplace_back(std::make_unique<Chunk>(layer, usedTileSets,
			//     sf::Vector2f(x * m_chunkSize.x, y * m_chunkSize.y), tileCount, map.getTileCount().x,
			//     m_textureResource));
			m_Chunks.emplace_back(std::make_unique<Chunk>(layer,
				usedTileSets,
				sf::Vector2f(x * m_ChunkSize.x, y * m_ChunkSize.y),
				tileCount,
				tileSize,
				map.getTileCount().x,
				m_TextureResource,
				map.getAnimatedTiles()));
		}
	}
}

void TileLayer::updateVisibility(const sf::View& view) const
{
	sf::Vector2f viewCorner = view.getCenter();
	viewCorner -= view.getSize() / 2.f;

	int posX = static_cast<int>(std::floor(viewCorner.x / m_ChunkSize.x));
	int posY = static_cast<int>(std::floor(viewCorner.y / m_ChunkSize.y));
	int posX2 = static_cast<int>(std::ceil((viewCorner.x + view.getSize().x) / m_ChunkSize.x));
	int posY2 = static_cast<int>(std::ceil((viewCorner.y + view.getSize().x) / m_ChunkSize.y));

	std::vector<Chunk*> visible;
	for (auto y = posY; y < posY2; ++y)
	{
		for (auto x = posX; x < posX2; ++x)
		{
			std::size_t idx = y * int(m_ChunkCount.x) + x;
			if (idx >= 0u && idx < m_Chunks.size() && !m_Chunks[idx]->empty())
			{
				visible.push_back(m_Chunks[idx].get());
			}
		}
	}

	std::swap(m_VisibleChunks, visible);
}

void TileLayer::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	// calc view coverage and draw nearest chunks
	updateVisibility(rt.getView());
	for (const auto& c : m_VisibleChunks)
	{
		rt.draw(*c, states);
	}
}
