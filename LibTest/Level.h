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

// was in the middle of restructuring level.cpp to use uint16

class Level
{
public:
	Level(std::string name, int width, int height, uint8_t layers);
	Level(std::string file_name);
	~Level();

	// Delete copy and move constructors

	int getIndexFromCoords(int x, int y, int layer);
	int getTile(int x, int y, int layer);
	void setTile(int x, int y, int layer, int16_t tile);

	void load(std::string path);
	void save(std::string path);

private:
	uint16_t width;
	uint16_t height;

	uint8_t num_layers;
	uint8_t collision_layer;
	int tile_array_size;

	std::shared_ptr<Tilemap> tilemap;

	std::string file_name;
	std::string level_name;

	std::vector<int16_t> tile_array;
};

Tilemap *loadTilemap(std::string file_name, int tile_size, int tiles_wide, int tiles_high);
Tilemap *destroyTilemap(Tilemap *tilemap);

class SetTileCommand : public Command
{
public:
	SetTileCommand(Level *w, int x, int y, int layer, short tile);
	~SetTileCommand();

	void redo();
	void undo();

private:
	short prev_tile;
	short new_tile;
	int index;

	Level *Level;
};

class ClearTileCommand : public Command
{
public:
	ClearTileCommand(Level *w, int x, int y, int layer, short tile);
	~ClearTileCommand();

	void redo();
	void undo();

private:
	short prev_tile;
	int index;

	Level *Level;
};