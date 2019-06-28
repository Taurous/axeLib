#pragma once

#include <string>
#include <allegro5/allegro.h>

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

int getTile(World *w, int x, int y, int layer);

World *createWorld(const char *name, int width, int height, short layers, Tilemap *tilemap);
void destroyWorld(World *w);

World *loadWorld(std::string file_name, Tilemap *tilemap);
void saveWorld(World *world);

Tilemap *loadTilemap(std::string file_name, int tile_size, int tiles_wide, int tiles_high);
void destroyTilemap(Tilemap *tilemap);
