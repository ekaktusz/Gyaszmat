#pragma once

#include "pch.h"
#include "MapLayerNames.h"

class TileLayer final : public sf::Drawable
{
public:
	TileLayer(const TileLayer&) = delete;
	TileLayer(const tmx::Map& map, MapLayerNames::TileLayerName idx);
	~TileLayer() = default;
	
	TileLayer& operator=(const TileLayer&) = delete;

	const sf::FloatRect& getGlobalBounds() const;
	tmx::TileLayer::Tile getTile(int tileX, int tileY);
	sf::Color getColor(int tileX, int tileY);

	void setTile(int tileX, int tileY, tmx::TileLayer::Tile tile, bool refresh = true);
	void setColor(int tileX, int tileY, sf::Color color, bool refresh = true);

	void update(sf::Time elapsed);

private: 
	void createChunks(const tmx::Map& map, const tmx::TileLayer& layer);
	void updateVisibility(const sf::View& view) const;
	virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

private:
	// increasing m_chunkSize by 4; fixes render problems when mapsize != chunksize
	// sf::Vector2f m_chunkSize = sf::Vector2f(1024.f, 1024.f);
	sf::Vector2f m_ChunkSize = sf::Vector2f(512.f, 512.f);
	sf::Vector2u m_ChunkCount;
	sf::Vector2u m_MapTileSize; // general Tilesize of Map
	sf::FloatRect m_GlobalBounds;

	using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;
	TextureResource m_TextureResource;

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
		sf::Color getColor(int x, int y) const;

		void setTile(int x, int y, tmx::TileLayer::Tile tile, bool refresh);
		void setColor(int x, int y, sf::Color color, bool refresh);

		void maybeRegenerate(bool refresh);

		int calcIndexFrom(int x, int y) const;
		bool empty() const;

		void flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
		void flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
		void flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
		void doFlips(std::uint8_t bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);

	private:
		virtual void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

	private:
		class ChunkArray final : public sf::Drawable
		{
		public:
			explicit ChunkArray(const sf::Texture& t, const tmx::Tileset& ts);

			~ChunkArray() = default;
			ChunkArray(const ChunkArray&) = delete;
			ChunkArray& operator=(const ChunkArray&) = delete;

			void reset();
			void addTile(const Chunk::Tile& tile);
			sf::Vector2u getTextureSize() const;

		public:
			using Ptr = std::unique_ptr<ChunkArray>;
			tmx::Vector2u tileSetSize;
			sf::Vector2u tileCount;
			std::uint32_t firstGID, lastGID;

		private:
			const sf::Texture& m_Texture;
			std::vector<sf::Vertex> m_Vertices;
			void draw(sf::RenderTarget& rt, sf::RenderStates states) const override
			{
				states.texture = &m_Texture;
				#ifndef __ANDROID__
				rt.draw(m_Vertices.data(), m_Vertices.size(), sf::Quads, states);
				#endif
				#ifdef __ANDROID__
				rt.draw(m_Vertices.data(), m_Vertices.size(), sf::Triangles, states);
				#endif
			}
		};

		sf::Uint8 m_LayerOpacity;		 // opacity of the layer
		sf::Vector2f m_LayerOffset;	 // Layer offset

		sf::Vector2u m_MapTileSize;	 // general Tilesize of Map
		sf::Vector2f m_ChunkTileCount; // chunk tilecount
		std::vector<tmx::TileLayer::Tile> m_ChunkTileIDs; // stores all tiles in this chunk for later manipulation
		std::vector<sf::Color> m_ChunkColors; // stores colors for extended color effects

		std::map<std::uint32_t, tmx::Tileset::Tile> m_AnimTiles; // animation catalogue
		std::vector<AnimationState> m_ActiveAnimations; // Animations to be done in this chunk
		std::vector<ChunkArray::Ptr> m_ChunkArrays;
	};

	std::vector<Chunk::Ptr> m_Chunks;
	mutable std::vector<Chunk*> m_VisibleChunks;
	
	Chunk::Ptr& getChunkAndTransform(int x, int y, sf::Vector2u& chunkRelative)
	{
		uint32_t chunkX = floor((x * m_MapTileSize.x) / m_ChunkSize.x);
		uint32_t chunkY = floor((y * m_MapTileSize.y) / m_ChunkSize.y);
		chunkRelative.x = ((x * m_MapTileSize.x) - chunkX * m_ChunkSize.x) / m_MapTileSize.x;
		chunkRelative.y = ((y * m_MapTileSize.y) - chunkY * m_ChunkSize.y) / m_MapTileSize.y;
		return m_Chunks[chunkX + static_cast<std::vector<Chunk::Ptr, std::allocator<Chunk::Ptr>>::size_type>(chunkY) * m_ChunkCount.x];
	}
};