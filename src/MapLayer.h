#pragma once

#include "pch.h"

class MapLayer final : public sf::Drawable
{
public:
	MapLayer(const MapLayer&) = delete;
	MapLayer(const tmx::Map& map, std::size_t idx);
	~MapLayer() = default;
	
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

		Chunk(const Chunk&) = delete;
		Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
			const sf::Vector2f& position, const sf::Vector2f& tileCount,
			const sf::Vector2u& tileSize, std::size_t rowSize, TextureResource& tr,
			const std::map<std::uint32_t, tmx::Tileset::Tile>& animTiles);
		~Chunk() = default;

		Chunk& operator=(const Chunk&) = delete;

		void generateTiles(bool registerAnimation = false);

		std::vector<AnimationState>& getActiveAnimations();

		tmx::TileLayer::Tile getTile(int x, int y) const;
		void setTile(int x, int y, tmx::TileLayer::Tile tile, bool refresh);

		sf::Color getColor(int x, int y) const;
		void setColor(int x, int y, sf::Color color, bool refresh);

		void maybeRegenerate(bool refresh);

		int calcIndexFrom(int x, int y) const;
		bool empty() const;

		void flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
		void flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
		void flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
		void doFlips(std::uint8_t bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2,
			sf::Vector2f* v3);

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

		void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	};

	std::vector<Chunk::Ptr> m_chunks;
	mutable std::vector<Chunk*> m_visibleChunks;
	
	Chunk::Ptr& getChunkAndTransform(int x, int y, sf::Vector2u& chunkRelative)
	{
		uint32_t chunkX = floor((x * m_MapTileSize.x) / m_chunkSize.x);
		uint32_t chunkY = floor((y * m_MapTileSize.y) / m_chunkSize.y);
		chunkRelative.x = ((x * m_MapTileSize.x) - chunkX * m_chunkSize.x) / m_MapTileSize.x;
		chunkRelative.y = ((y * m_MapTileSize.y) - chunkY * m_chunkSize.y) / m_MapTileSize.y;
		return m_chunks[chunkX + static_cast<std::vector<Chunk::Ptr, std::allocator<Chunk::Ptr>>::size_type>(chunkY) * m_chunkCount.x];
	}

	void createChunks(const tmx::Map& map, const tmx::TileLayer& layer);

	void updateVisibility(const sf::View& view) const;

	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};