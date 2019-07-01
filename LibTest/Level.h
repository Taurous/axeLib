#pragma once

#include <string>
#include <vector>
#include <allegro5/allegro.h>

#include "Command.h"

constexpr auto INVALID_TILE = -1;

struct Tilemap
{
	ALLEGRO_BITMAP *bmp;
	int bmp_width;
	int bmp_height;
	int tiles_wide;
	int tiles_high;
	int tile_size;

	std::string tilemap_file_name;
};

class Level
{
public:
	Level(std::string name, int width, int height, uint8_t layers);
	Level();
	~Level();

	// Delete copy and move constructors

	int getIndexFromCoords(int x, int y, int layer);
	int getTile(int x, int y, int layer);
	void setTile(int x, int y, int layer, short tile);
	void setTile(int index, short tile);

	bool load(std::string path);
	void save();
	void save(std::string path);

	void setTilemap(std::shared_ptr<Tilemap> tmap)
	{
		tilemap = std::move(tmap);
	}

	int getWidth() { return width; }
	int getHeight() { return height; }
	int getNumLayers() { return num_layers; }
	int getTileArraySize() { return tile_array_size; }

private:
	uint16_t width;
	uint16_t height;

	uint8_t num_layers;
	int tile_array_size;

	std::shared_ptr<Tilemap> tilemap;

	std::string file_name;
	std::string level_name;

	std::vector<int16_t> tile_array;
	std::vector<bool> collision_array;
};

Tilemap *loadTilemap(std::string file_name, int tile_size, int tiles_wide, int tiles_high);
Tilemap *destroyTilemap(Tilemap *tilemap);

class SetTileCommand : public Command
{
public:
	SetTileCommand(Level *level, int x, int y, int layer, short tile);
	~SetTileCommand();

	void redo();
	void undo();

private:
	short prev_tile;
	short new_tile;
	int index;

	Level *lv;
};

class ClearTileCommand : public Command
{
public:
	ClearTileCommand(Level *level, int x, int y, int layer);
	~ClearTileCommand();

	void redo();
	void undo();

private:
	short prev_tile;
	int index;

	Level *lv;
};