#pragma once

// Does not support multi-byte filenames

#include <string>
#include <vector>

#include <allegro5/allegro.h>

#include <axeLib/util/Logger.h>

#include "Command.h"

constexpr auto INVALID_TILE = -1;

struct Tilemap
{
	ALLEGRO_BITMAP *bmp;
	unsigned int bmp_width;
	unsigned int bmp_height;
	unsigned int tiles_wide;
	unsigned int tiles_high;
	unsigned int tile_size;

	int max_tile;

	std::string file_name;
};

class Level
{
public:
	Level(std::string name, int width, int height, uint8_t layers);
	Level();
	~Level();

	// Delete copy and move constructors

	const int getIndexFromCoords(int x, int y, int layer) const;
	const int getTile(int x, int y, int layer) const;
	void setTile(int x, int y, int layer, short tile);
	void setTile(int index, short tile);

	bool load(std::string path);
	void save(std::string path);

	const int getWidth() const { return width; } // Width of the level in tiles
	const int getHeight() const { return height; } // Height of the level in tiles
	const int getNumLayers() const { return num_layers; }
	const int getTileArraySize() const { return tile_array_size; }

	//Tilemap Functions

	bool loadTilemap(std::string path, int tile_size);
	ALLEGRO_BITMAP *getAllegroBitmap() { return tilemap.bmp; }

	void drawTile(int x, int y, short tile, float scale = 1.f);
	void drawTilemap(int x, int y, float scale = 1.f); // Draw entire tilemap is single bitmap

	const int getTilemapWidth() const { return tilemap.bmp_width; } // Width of the tilemap in pixels
	const int getTilemapHeight() const { return tilemap.bmp_height; } // Height of the tilemap in pixels

	const int getTileSize() const { return tilemap.tile_size; }
	const int getTilesWide() const { return tilemap.tiles_wide; } // Width of the tilemap in tiles
	const int getTilesHigh() const { return tilemap.tiles_high; } // Height of the tilemap in tiles

	const std::string &getName() const { return name; }

	void clear();

private:
	uint16_t width;
	uint16_t height;

	uint8_t num_layers;
	int tile_array_size;

	Tilemap tilemap;
	std::string name;

	std::vector<int16_t> tile_array;
	std::vector<bool> collision_array;
};

class SetTileCommand : public Command
{
public:
	SetTileCommand(std::shared_ptr<Level> level, int x, int y, int layer, short tile);
	SetTileCommand(SetTileCommand && other);
	~SetTileCommand();

	void redo();
	void undo();

private:
	short prev_tile;
	short new_tile;
	int index;

	std::shared_ptr<Level> lv;
};
class ClearTileCommand : public Command
{
public:
	ClearTileCommand(std::shared_ptr<Level> level, int x, int y, int layer);
	ClearTileCommand(ClearTileCommand && other);
	~ClearTileCommand();

	void redo();
	void undo();

private:
	short prev_tile;
	int index;

	std::shared_ptr<Level> lv;
};