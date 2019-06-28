#pragma once

#include <string>
#include <allegro5/allegro.h>

#include "Command.h"

struct Tilemap
{
	ALLEGRO_BITMAP *bmp;
	int width;
	int height;
	int tiles_wide;
	int tiles_high;
	int tile_size;
};

struct World
{
	std::string file_name;
	char *name;

	int width;
	int height;

	short num_layers;
	char *tiles;

	Tilemap *tilemap;
};

int getTileIndex(World *w, int x, int y, int layer);
void setTile(World *w, int x, int y, int layer, char tile);

World *createWorld(const char *name, int width, int height, short layers, Tilemap *tilemap);
World *destroyWorld(World *w);

World *loadWorld(std::string file_name, Tilemap *tilemap);
void saveWorld(World *world);

Tilemap *loadTilemap(std::string file_name, int tile_size, int tiles_wide, int tiles_high);
Tilemap *destroyTilemap(Tilemap *tilemap);

class SetTileCommand : public Command
{
public:
	SetTileCommand(World *w, int x, int y, int layer, char tile);
	~SetTileCommand();

	void redo();
	void undo();

private:
	char prev_tile;
	char new_tile;
	int index;

	World *world;
};

class ClearTileCommand : public Command
{
public:
	ClearTileCommand(World *w, int x, int y, int layer, char tile);
	~ClearTileCommand();

	void redo();
	void undo();

private:
	char prev_tile;
	int index;

	World *world;
};