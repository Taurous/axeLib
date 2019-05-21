#include "SimpleState.h"

SimpleState::SimpleState(axe::StateManager & states, axe::InputHandler & input, axe::EventHandler & events, axe::DrawEngine & draw)
	: AbstractState(states, input, events, draw)
{
	tileset = al_load_bitmap("tileset.png");

	tile_size = 64;
	tiles_wide = 10;
	tiles_high = 10;

	world = { {
		{22,11,11,11,11,11,11,11,11,23},
		{10, 0, 0, 0, 0, 0, 0, 0, 0,12},
		{10, 0, 0, 0, 0, 0, 0, 0, 0,12},
		{10, 0, 0, 0, 2, 3, 0, 0, 0,12},
		{10, 0, 0, 0, 1, 4, 0, 0, 0,12},
		{10, 0, 0, 0, 0, 0, 0, 0, 0,12},
		{10, 0, 0, 0, 0, 0, 0, 0, 0,12},
		{10, 0, 0, 0, 0, 0, 0, 0, 0,12},
		{10, 0, 0, 0, 0, 0, 0, 0, 0,12},
		{21,13,13,13,13,13,13,13,13,20}
		} };

	world_col = { {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
		} };
}

SimpleState::~SimpleState()
{
	
}

void SimpleState::pause() { }
void SimpleState::resume() { }

void SimpleState::handleEvents()
{
	
}
void SimpleState::update(unsigned long long deltaTime)
{
	
}
void SimpleState::draw()
{
	int offset_x = 640;
	int offset_y = 0;

	for (int y = 0; y < tiles_high; ++y)
	{
		for (int x = 0; x < tiles_wide; ++x)
		{
			int tile = world[y][x];

			int xx = tile % tiles_wide;
			int yy = tile / tiles_high;

			al_draw_bitmap_region(tileset, xx * tile_size, yy * tile_size, tile_size, tile_size, offset_x + (x * tile_size), offset_y + (y * tile_size), 0);
		}
	}
	al_draw_bitmap(tileset, 0, 0, 0);
}