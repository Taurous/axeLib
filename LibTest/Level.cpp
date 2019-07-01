#include "Level.h"
#include <fstream>
#include <iostream>

constexpr auto VERSION = 0x00020000;

Level::Level(std::string name, int width, int height, uint8_t layers) :
	tile_array(width * height * layers, INVALID_TILE)
{
	file_name = name + ".bin";
	level_name = name;

	this->width = width;
	this->height = height;
	this->num_layers = layers;

	tile_array_size = width * height * num_layers;

	std::cout << "Array size: " << tile_array.size() << std::endl;

	for (auto &t : tile_array)
	{
		if (t != INVALID_TILE) std::cout << "Data was not set correctly!\n";
	}

	std::cout << std::endl;
}

Level::Level(std::string path)
{
	std::ifstream map_file(file_name, std::ios::binary | std::ios::ate);

	if (map_file.is_open())
	{
		int32_t magic;
		int32_t version;
		int32_t f_size;
		int32_t offset_to_data;

		int16_t width;
		int16_t height;
		int8_t num_layers;
		std::string name;

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

		if (version != VERSION)
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

		map_file.read((char*)name, sizeof(char) * 32);

		lv = createLevel((char*)name, width, height, num_layers, tilemap);

		map_file.seekg(offset_to_data);

		map_file.read((char*)lv->tiles, sizeof(char) * width * height * num_layers);

		map_file.close();
	}

}

int Level::getIndexFromCoords(int x, int y, int layer)
{
	int index = INVALID_TILE;

	if (layer < num_layers)
	{
		index = (y * width + x) + (width * height * layer);
	}

	return index;
}
void setTile(Level *w, int x, int y, int layer, int16_t tile)
{
	int t = getTileIndex(w, x, y, layer);

	if (t >= 0 && t < w->tile_array_size)
	{
		w->tile_array[t] = tile;
	}
}

void saveLevel(Level *lv)
{
	std::ofstream map_file(lv->file_name, std::ios::binary|std::ios::trunc);

	if (map_file.is_open())
	{
		int32_t magic = 0x00455841; // = "AXE\0"
		int32_t version = VERSION;
		int32_t offset_to_data = 0xC4;
		int32_t file_size = 0;
		
		map_file << magic;
		//map_file.write((char*)&magic, sizeof(magic));

		map_file << version;
		//map_file.write((char*)&version, sizeof(version));

		map_file << file_size << offset_to_data;
		//map_file.write((char*)&file_size, sizeof(file_size)); // 4 Bytes to be filled in with file size;
		//map_file.write((char*)&offset_to_data, sizeof(offset_to_data));

		map_file << lv->width << lv->height << lv->num_layers;
		//map_file.write((char *)&lv->width, sizeof(lv->width));
		//map_file.write((char *)&lv->height, sizeof(lv->height));
		//map_file.write((char *)&lv->num_layers, sizeof(int));

		map_file << lv->level_name.size() << lv->level_name;
		//map_file.write(lv->name, sizeof(char) * name_size);

		map_file.seekp(offset_to_data);

		map_file.write((char*)lv->tiles, sizeof(int16_t) * lv->tile_array_size);

		file_size = map_file.tellp();

		map_file.seekp(8);

		map_file.write((char *)&file_size, sizeof(file_size));

		map_file.close();
	}
}

Level *destroyLevel(Level *w)
{
	if (w)
	{
		delete[] w->tiles;

		delete w;
	}

	return nullptr;
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

Tilemap *destroyTilemap(Tilemap *tilemap)
{
	if (tilemap->bmp) al_destroy_bitmap(tilemap->bmp);

	delete tilemap;

	return nullptr;
}

SetTileCommand::SetTileCommand(Level *w, int x, int y, int layer, char tile) : Command(),
Level(w), prev_tile(-1), new_tile(-1)
{
	index = getTileIndex(Level, x, y, layer);
	new_tile = tile;
	prev_tile = Level->tiles[index];

	redo();
}

SetTileCommand::~SetTileCommand()
{

}

void SetTileCommand::redo()
{
	std::cout << "Setting Tile Index " << index << " to tile " << int(new_tile) << std::endl;
	Level->tiles[index] = new_tile;
}

void SetTileCommand::undo()
{
	Level->tiles[index] = prev_tile;
}

ClearTileCommand::ClearTileCommand(Level *w, int x, int y, int layer, char tile) : Command(),
Level(w), prev_tile(-1)
{
	index = getTileIndex(Level, x, y, layer);
	prev_tile = Level->tiles[index];

	redo();
}

ClearTileCommand::~ClearTileCommand()
{

}

void ClearTileCommand::redo()
{
	Level->tiles[index] = -1;
}

void ClearTileCommand::undo()
{
	Level->tiles[index] = prev_tile;
}