#pragma once

#include "pch.h"

class MapLayer final : public sf::Drawable
{
public:
	MapLayer(const tmx::Map& map, std::size_t idx);

	~MapLayer() = default;
	MapLayer(const MapLayer&) = delete;
	MapLayer& operator=(const MapLayer&) = delete;

	const sf::FloatRect& getGlobalBounds() const;
	const std::vector<sf::FloatRect>& getObjectBounds() const;

	void setTile(int tileX, int tileY, tmx::TileLayer::Tile tile, bool refresh = true);

	tmx::TileLayer::Tile getTile(int tileX, int tileY);
	void setColor(int tileX, int tileY, sf::Color color, bool refresh = true);

	sf::Color getColor(int tileX, int tileY);

	void update(sf::Time elapsed);

private:
	// increasing m_chunkSize by 4; fixes render problems when mapsize != chunksize
	// sf::Vector2f m_chunkSize = sf::Vector2f(1024.f, 1024.f);
	sf::Vector2f m_chunkSize = sf::Vector2f(512.f, 512.f);
	sf::Vector2u m_chunkCount;
	sf::Vector2u m_MapTileSize; // general Tilesize of Map
	sf::FloatRect m_globalBounds;

	std::vector<sf::FloatRect> objectBounds;

	using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;
	TextureResource m_textureResource;

	struct AnimationState
	{
		sf::Vector2u tileCords;
		sf::Time startTime;
		sf::Time currentTime;
		tmx::Tileset::Tile animTile;
		std::uint8_t flipFlags;
	};

	class Chunk final : public sf::Transformable, public sf::Drawable
	{
	public:
		using Ptr = std::unique_ptr<Chunk>;

		// the Android OpenGL driver isn't capable of rendering quads,
		// so we need to use two triangles per tile instead
		#ifdef __ANDROID__
		using Tile = std::array<sf::Vertex, 6u>;
		#endif
		#ifndef __ANDROID__
		using Tile = std::array<sf::Vertex, 4u>;
		#endif

		Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
			const sf::Vector2f& position, const sf::Vector2f& tileCount,
			const sf::Vector2u& tileSize, std::size_t rowSize, TextureResource& tr,
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
					tmx::Logger::log("Chunks using " + ts->getName() + " will not be created",
						tmx::Logger::Type::Info);
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

		void generateTiles(bool registerAnimation = false)
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
								sf::Vertex(
									tileOffset - getPosition(), m_chunkColors[idx], tileIndex),
								sf::Vertex(tileOffset - getPosition()
										+ sf::Vector2f(ca->tileSetSize.x, 0.f),
									m_chunkColors[idx],
									tileIndex + sf::Vector2f(ca->tileSetSize.x, 0.f)),
								sf::Vertex(tileOffset - getPosition()
										+ sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y),
									m_chunkColors[idx],
									tileIndex + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y)),
								sf::Vertex(tileOffset - getPosition()
										+ sf::Vector2f(0.f, ca->tileSetSize.y),
									m_chunkColors[idx],
									tileIndex + sf::Vector2f(0.f, ca->tileSetSize.y))
#endif
#ifdef __ANDROID__
									sf::Vertex(
										tileOffset - getPosition(), m_chunkColors[idx], tileIndex),
								sf::Vertex(tileOffset - getPosition()
										+ sf::Vector2f(ca->tileSetSize.x, 0.f),
									m_chunkColors[idx],
									tileIndex + sf::Vector2f(ca->tileSetSize.x, 0.f)),
								sf::Vertex(tileOffset - getPosition()
										+ sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y),
									m_chunkColors[idx],
									tileIndex + sf::Vector2f(ca->tileSetSize.x, ca->tileSetSize.y)),
								sf::Vertex(
									tileOffset - getPosition(), m_chunkColors[idx], tileIndex),
								sf::Vertex(tileOffset - getPosition()
										+ sf::Vector2f(0.f, ca->tileSetSize.y),
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
		~Chunk() = default;
		Chunk(const Chunk&) = delete;
		Chunk& operator=(const Chunk&) = delete;
		std::vector<AnimationState>& getActiveAnimations()
		{
			return m_activeAnimations;
		}
		tmx::TileLayer::Tile getTile(int x, int y) const
		{
			return m_chunkTileIDs[calcIndexFrom(x, y)];
		}
		void setTile(int x, int y, tmx::TileLayer::Tile tile, bool refresh)
		{
			m_chunkTileIDs[calcIndexFrom(x, y)] = tile;
			maybeRegenerate(refresh);
		}
		sf::Color getColor(int x, int y) const
		{
			return m_chunkColors[calcIndexFrom(x, y)];
		}
		void setColor(int x, int y, sf::Color color, bool refresh)
		{
			m_chunkColors[calcIndexFrom(x, y)] = color;
			maybeRegenerate(refresh);
		}
		void maybeRegenerate(bool refresh)
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
		int calcIndexFrom(int x, int y) const
		{
			return x + y * chunkTileCount.x;
		}
		bool empty() const
		{
			return m_chunkArrays.empty();
		}
		void flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
		{
			// Flip Y
			sf::Vector2f tmp = *v0;
			v0->y = v2->y;
			v1->y = v2->y;
			v2->y = tmp.y;
			v3->y = v2->y;
		}

		void flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
		{
			// Flip X
			sf::Vector2f tmp = *v0;
			v0->x = v1->x;
			v1->x = tmp.x;
			v2->x = v3->x;
			v3->x = v0->x;
		}

		void flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3)
		{
			// Diagonal flip
			sf::Vector2f tmp = *v1;
			v1->x = v3->x;
			v1->y = v3->y;
			v3->x = tmp.x;
			v3->y = tmp.y;
		}

		void doFlips(std::uint8_t bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2,
			sf::Vector2f* v3)
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

	private:
		class ChunkArray final : public sf::Drawable
		{
		public:
			using Ptr = std::unique_ptr<ChunkArray>;
			tmx::Vector2u tileSetSize;
			sf::Vector2u tsTileCount;
			std::uint32_t m_firstGID, m_lastGID;
			explicit ChunkArray(const sf::Texture& t, const tmx::Tileset& ts) : m_texture(t)
			{
				auto texSize = getTextureSize();
				tileSetSize = ts.getTileSize();
				tsTileCount.x = texSize.x / tileSetSize.x;
				tsTileCount.y = texSize.y / tileSetSize.y;
				m_firstGID = ts.getFirstGID();
				m_lastGID = ts.getLastGID();
			}

			~ChunkArray() = default;
			ChunkArray(const ChunkArray&) = delete;
			ChunkArray& operator=(const ChunkArray&) = delete;

			void reset()
			{
				m_vertices.clear();
			}
			void addTile(const Chunk::Tile& tile)
			{
				for (const auto& v : tile)
				{
					m_vertices.push_back(v);
				}
			}
			sf::Vector2u getTextureSize() const
			{
				return m_texture.getSize();
			}

		private:
			const sf::Texture& m_texture;
			std::vector<sf::Vertex> m_vertices;
			void draw(sf::RenderTarget& rt, sf::RenderStates states) const override
			{
				states.texture = &m_texture;
#ifndef __ANDROID__
				rt.draw(m_vertices.data(), m_vertices.size(), sf::Quads, states);
#endif
#ifdef __ANDROID__
				rt.draw(m_vertices.data(), m_vertices.size(), sf::Triangles, states);
#endif
			}
		};

		sf::Uint8 layerOpacity;		 // opacity of the layer
		sf::Vector2f layerOffset;	 // Layer offset
		sf::Vector2u mapTileSize;	 // general Tilesize of Map
		sf::Vector2f chunkTileCount; // chunk tilecount
		std::vector<tmx::TileLayer::Tile>
			m_chunkTileIDs; // stores all tiles in this chunk for later manipulation
		std::vector<sf::Color> m_chunkColors; // stores colors for extended color effects
		std::map<std::uint32_t, tmx::Tileset::Tile> m_animTiles; // animation catalogue
		std::vector<AnimationState> m_activeAnimations; // Animations to be done in this chunk
		std::vector<ChunkArray::Ptr> m_chunkArrays;
		void draw(sf::RenderTarget& rt, sf::RenderStates states) const override
		{
			states.transform *= getTransform();
			for (const auto& a : m_chunkArrays)
			{
				rt.draw(*a, states);
			}
		}
	};

	std::vector<Chunk::Ptr> m_chunks;
	mutable std::vector<Chunk*> m_visibleChunks;
	Chunk::Ptr& getChunkAndTransform(int x, int y, sf::Vector2u& chunkRelative)
	{
		uint32_t chunkX = floor((x * m_MapTileSize.x) / m_chunkSize.x);
		uint32_t chunkY = floor((y * m_MapTileSize.y) / m_chunkSize.y);
		chunkRelative.x = ((x * m_MapTileSize.x) - chunkX * m_chunkSize.x) / m_MapTileSize.x;
		chunkRelative.y = ((y * m_MapTileSize.y) - chunkY * m_chunkSize.y) / m_MapTileSize.y;
		return m_chunks[chunkX + chunkY * m_chunkCount.x];
	}
	void createChunks(const tmx::Map& map, const tmx::TileLayer& layer);

	void updateVisibility(const sf::View& view) const;

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};