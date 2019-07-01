#include "Level.h"
#include <fstream>
#include <iostream>

constexpr uint64_t RELEASE	= 1;
constexpr uint64_t MAJOR	= 1;
constexpr uint64_t MINOR	= 1;
constexpr auto MAX_LEVEL_NAME = 28; // extension is 4 chars so 28+4 is max size of 32
constexpr int16_t END_OF_ARRAY = 0xBFFF;
constexpr uint32_t magic = 0x0a455841;

const inline uint64_t getVersion()
{
	return (RELEASE << 48) + (MAJOR << 32) + MINOR;
}

Level::Level(std::string name, int width, int height, uint8_t layers) :
	tile_array(width * height * layers, INVALID_TILE), collision_array(width * height, false)
{
	while (name.size() > MAX_LEVEL_NAME)
	{
		name.pop_back();
	}

	file_name = name + ".bin";
	level_name = name;

	this->width = width;
	this->height = height;
	this->num_layers = layers;

	tile_array_size = width * height * num_layers;
}
Level::Level() : width(0), height(0), num_layers(0), tile_array_size(0)
{
	

}
Level::~Level()
{
	// destroy stuff
}

void Level::save()
{
	std::ofstream map_file(file_name, std::ios::binary | std::ios::trunc);

	if (map_file.is_open())
	{
		uint64_t version = getVersion();
		int32_t offset_to_data = 0x60;
		int32_t file_size = 0;

		map_file.write((char*)&magic, sizeof(magic));
		map_file.write((char*)&version, sizeof(version));

		map_file.write((char*)&file_size, sizeof(file_size)); // 4 Bytes to be filled in with file size;
		map_file.write((char*)&offset_to_data, sizeof(offset_to_data));

		map_file.write((char*)&width, sizeof(width));
		map_file.write((char*)&height, sizeof(height));

		size_t name_size = level_name.size();

		map_file.write((char*)&name_size, sizeof(size_t));

		for (auto &c : level_name)
		{
			map_file.write(&c, sizeof(c));
		}

		map_file.seekp(offset_to_data);

		for (auto &t : tile_array)
		{
			map_file.write((char*)&t, sizeof(t));
		}

		map_file.write((char*)&END_OF_ARRAY, sizeof(END_OF_ARRAY));

		file_size = map_file.tellp();

		map_file.seekp(sizeof(magic) + sizeof(version));

		map_file.write((char*)&file_size, sizeof(file_size));

		map_file.close();
	}
}
void Level::save(std::string path)
{

}
bool Level::load(std::string path)
{
	tile_array.clear();
	collision_array.clear();

	std::ifstream map_file(path, std::ios::binary | std::ios::ate);

	if (map_file.is_open())
	{
		uint32_t file_magic;
		uint64_t version;
		int32_t file_size;
		int32_t offset_to_data;
		size_t string_size;

		std::streamsize size = map_file.tellg();
		map_file.seekg(0);

		map_file.read((char*)&file_magic, sizeof(file_magic));

		if (file_magic != magic)
		{
			// log, file type unknown
			map_file.close();
			return false;
		}

		map_file.read((char*)&version, sizeof(version));

		if (version != getVersion())
		{
			// log, version mismatch
			map_file.close();
			return false;
		}

		map_file.read((char*)&file_size, sizeof(file_size));

		if (file_size != size)
		{
			// log, size wrong, file edited?
			map_file.close();
			return false;
		}

		map_file.read((char*)&offset_to_data, sizeof(offset_to_data));
		map_file.read((char*)&width, sizeof(width));
		map_file.read((char*)&height, sizeof(height));

		map_file.read((char*)&string_size, sizeof(string_size));

		char buffer[MAX_LEVEL_NAME+1];
		map_file.read(buffer, string_size);
		buffer[string_size] = '\0';
		level_name = std::string(buffer);

		map_file.seekg(offset_to_data); // Seek to level array data

		while (true)
		{
			int16_t t;
			map_file.read((char*)&t, sizeof(int16_t));

			if (t == END_OF_ARRAY) break; // Need to make sure not to get stuck here

			tile_array.push_back(t);
		}

		num_layers = uint8_t(tile_array.size() / (width * height));

		map_file.close();

		return true;
	}

	return false;
}

int Level::getTile(int x, int y, int layer)
{
	int index = getIndexFromCoords(x, y, layer);

	if (index == INVALID_TILE) return INVALID_TILE;

	return tile_array[index];
}
void Level::setTile(int x, int y, int layer, short tile)
{
	int index = getIndexFromCoords(x, y, layer);

	setTile(index, tile);
}
void Level::setTile(int index, short tile)
{
	if (index >= 0 && index < tile_array_size)
	{
		tile_array[index] = tile;
	}
}

int Level::getIndexFromCoords(int x, int y, int layer)
{
	assert(layer < num_layers);

	return (y * width + x) + (width * height * layer);
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

	t->bmp_width = al_get_bitmap_width(t->bmp);
	t->bmp_height = al_get_bitmap_height(t->bmp);

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

SetTileCommand::SetTileCommand(Level *level, int x, int y, int layer, short tile) :
	Command(), lv(level), index(lv->getIndexFromCoords(x, y, layer)), prev_tile(lv->getTile(x, y, layer)), new_tile(tile)
{
	redo();
}

SetTileCommand::~SetTileCommand()
{

}

void SetTileCommand::redo()
{
	lv->setTile(index, new_tile);
}

void SetTileCommand::undo()
{
	lv->setTile(index, prev_tile);
}

ClearTileCommand::ClearTileCommand(Level *level, int x, int y, int layer) :
	Command(), lv(level), index(lv->getIndexFromCoords(x, y, layer)), prev_tile(lv->getTile(x, y, layer))
{
	redo();
}

ClearTileCommand::~ClearTileCommand()
{

}

void ClearTileCommand::redo()
{
	lv->setTile(index, INVALID_TILE);
}

void ClearTileCommand::undo()
{
	lv->setTile(index, prev_tile);
}