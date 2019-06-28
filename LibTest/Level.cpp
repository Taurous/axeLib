#include "Level.h"

#include <sstream>
#include <fstream>

int getTile(World *w, int x, int y, int layer)
{
	return (y * w->width + x) + (w->width * w->height * layer);
}

World *createWorld(const char *name, int width, int height, short layers, Tilemap *tilemap)
{
	World *world;

	world = new World();

	std::stringstream ss;
	ss << std::string(name) << ".bin";
	world->file_name = ss.str();

	world->name = new char[32];
	memcpy(world->name, name, 32);

	world->width = width;
	world->height = height;

	world->num_layers = layers;

	world->tilemap = tilemap;

	std::size_t world_size = width * height * layers;

	world->tiles = new char[world_size];

	memset(world->tiles, -1, world_size);

	return world;
}

World *loadWorld(std::string file_name, Tilemap *tilemap)
{
	World *world = nullptr;

	std::ifstream map_file(file_name, std::ios::binary|std::ios::ate);

	if (map_file.is_open())
	{
		int magic;
		int version;
		int f_size;
		int offset_to_data;

		int width;
		int height;
		int num_layers;
		char name[32];

		std::streamsize file_size = map_file.tellg();
		map_file.seekg(0);

		map_file.read((char*)&magic, sizeof(magic));

		if (magic != 0x00455841)
		{
			// log, file type unknown
			map_file.close();
			return nullptr;
		}

		map_file.read((char*)&version, 4);

		if (version != 0x00010000)
		{
			// log, version mismatch
			map_file.close();
			return nullptr;
		}

		map_file.read((char*)&f_size, 4);

		if (f_size != file_size)
		{
			// log, size wrong, file edited?
			map_file.close();
			return nullptr;
		}

		map_file.read((char*)&offset_to_data, 4);
		map_file.read((char*)&width, sizeof(width));
		map_file.read((char*)&height, sizeof(height));
		map_file.read((char*)&num_layers, sizeof(num_layers));

		map_file.read(name, sizeof(char)*32);

		world = createWorld(name, width, height, num_layers, tilemap);

		map_file.seekg(offset_to_data);

		map_file.read(world->tiles, sizeof(char) * width * height * num_layers);

		map_file.close();
	}
	
	return world;
}

void saveWorld(World *world)
{
	std::ofstream map_file(world->file_name, std::ios::binary|std::ios::trunc);

	if (map_file.is_open())
	{
		int magic = 0x00455841;
		int version = 0x00010000;
		int offset_to_data = 0x80;
		int file_size = 0;
		
		map_file.write((char *)&magic, sizeof(magic));

		map_file.write((char *)&version, sizeof(version));

		map_file.write((char *)&file_size, sizeof(int)); // 4 Bytes to be filled in with file size;
		map_file.write((char*)&offset_to_data, sizeof(offset_to_data));

		map_file.write((char *)&world->width, sizeof(world->width));
		map_file.write((char *)&world->height, sizeof(world->height));
		map_file.write((char *)&world->num_layers, sizeof(int));

		map_file.write(world->name, sizeof(char)*32);

		map_file.seekp(offset_to_data);

		map_file.write(world->tiles, sizeof(char) * (world->width * world->height * world->num_layers));

		file_size = map_file.tellp();

		map_file.seekp(8);

		map_file.write((char *)&file_size, sizeof(file_size));

		map_file.close();
	}
}

void destroyWorld(World *w)
{
	if (w)
	{
		delete[] w->tiles;
		delete[] w->name;

		delete w;
	}
}

Tilemap *loadTilemap(std::string file_name, int tile_size, int tiles_wide, int tiles_high)
{
	Tilemap *t = new Tilemap();

	t->bmp = al_load_bitmap(file_name.c_str());

	if (!t->bmp)
	{
		delete t;
		return nullptr;
	}

	t->width = al_get_bitmap_width(t->bmp);
	t->height = al_get_bitmap_height(t->bmp);

	t->tile_size = tile_size;
	t->tiles_wide = tiles_wide;
	t->tiles_high = tiles_high;

	return t;
}

void destroyTilemap(Tilemap *tilemap)
{
	if (tilemap->bmp) al_destroy_bitmap(tilemap->bmp);

	delete tilemap;
}