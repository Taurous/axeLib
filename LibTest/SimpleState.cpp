#include "SimpleState.h"
#include <iostream>
#include <fstream>
#include <iomanip>

#include <allegro5\allegro_primitives.h>

float scale = 4.f;
int prev_cell = -1;

SimpleState::SimpleState(axe::StateManager & states, axe::InputHandler & input, axe::EventHandler & events, axe::DrawEngine & draw)
	: AbstractState(states, input, events, draw), draw_grid(true), tbox(8, 300, "C:/Windows/Fonts/ARBONNIE.ttf", 32, al_map_rgb(255, 100, 100)), 
	camera_x(0), camera_y(0), camera_halfwidth(draw.getWindow().getWidth() / 2), camera_halfheight(draw.getWindow().getHeight() / 2)
{
	srand(time(NULL));

	tilemap = loadTilemap("Dungeon_Tileset.png", 16, 10, 10);
	world = createWorld("Level 2", 30, 30, 2, tilemap);

	if (!world) axe::crash("Failed to create world!");

	font = al_load_font("C:/Windows/Fonts/ARBONNIE.ttf", 40, 0);
	if (!font) axe::crash("Failed to load font!");
	current_layer = 0;
}

SimpleState::~SimpleState()
{
	al_destroy_font(font);

	destroyTilemap(tilemap);
	destroyWorld(world);
}

void SimpleState::pause() { }
void SimpleState::resume() { }

void SimpleState::handleEvents()
{
	if (m_events.eventIs(ALLEGRO_EVENT_DISPLAY_RESIZE))
	{
		const ALLEGRO_EVENT ev = m_events.getEvent();

		camera_halfwidth = ev.display.width / 2;
		camera_halfheight = ev.display.height / 2;
	}

	if (m_input.isKeyPressed(ALLEGRO_KEY_L))
	{
		if (world) destroyWorld(world);

		world = loadWorld("Level 2.bin", tilemap);

		if (!world) axe::crash("Failed to load map Level 1.bin");

		tbox.insertString(world->file_name + " loaded!");
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_S))
	{
		saveWorld(world);
		tbox.insertString(world->file_name + " saved!");
	}

	char pressed = 0;
	if (pressed = m_input.getChar())
	{
		switch (pressed)
		{
		case ALLEGRO_KEY_0:
		case ALLEGRO_KEY_PAD_0:
			current_layer = 0;
			break;
		case ALLEGRO_KEY_1:
		case ALLEGRO_KEY_PAD_1:
			current_layer = 1;
			break;
		case ALLEGRO_KEY_2:
		case ALLEGRO_KEY_PAD_2:
			current_layer = 2;
			break;
		}
	}

	int ix = m_input.getMouseX() - camera_halfwidth + camera_x;
	if (ix > 0) ix /= (tilemap->tile_size * scale); // to correct ex: -0.5 being / to 0.

	int iy = m_input.getMouseY() - camera_halfheight + camera_y;
	if (iy > 0) iy /= (tilemap->tile_size * scale);

	if (m_input.isMouseDown(axe::MOUSE_RIGHT))
	{
		if (ix >= 0 && ix < world->width && iy >= 0 && iy < world->height)
		{
			int tile = getTile(world, ix, iy, current_layer);
			world->tiles[tile] = -1;
		}

		std::cout << "ix: " << ix << " iy: " << iy << std::endl;
	}
	else if (m_input.isMouseDown(axe::MOUSE_LEFT))
	{
		if (ix >= 0 && ix < world->width && iy >= 0 && iy < world->height)
		{
			int tile = getTile(world, ix, iy, current_layer);

			if (prev_cell != tile)
			{
				world->tiles[tile] = (char)selection;
			}
			prev_cell = tile;
		}

		ix = (m_input.getMouseX() - 8) / 25;
		iy = (m_input.getMouseY() - 8) / 25;

		if (ix >= 0 && ix < tilemap->tiles_wide && iy >= 0 && iy < tilemap->tiles_high)
		{
			selection = iy * tilemap->tiles_wide + ix;

			std::cout << "Selection: " << selection << std::endl;
			std::cout << "ix: " << ix << " iy: " << iy << std::endl;
		}
	}

	if (m_input.isKeyPressed(ALLEGRO_KEY_G))
	{
		draw_grid = !draw_grid;
	}
}
void SimpleState::update(unsigned long long deltaTime)
{

	if (m_input.isKeyDown(ALLEGRO_KEY_LEFT)) camera_x -= 16;
	else if (m_input.isKeyDown(ALLEGRO_KEY_RIGHT)) camera_x += 16;
	if (m_input.isKeyDown(ALLEGRO_KEY_UP)) camera_y -= 16;
	else if (m_input.isKeyDown(ALLEGRO_KEY_DOWN)) camera_y += 16;

	if (camera_x > world->width * world->tilemap->tile_size * scale) camera_x = world->width * world->tilemap->tile_size * scale;
	else if (camera_x < 0) camera_x = 0;

	if (camera_y > world->height * world->tilemap->tile_size * scale) camera_y = world->height * world->tilemap->tile_size * scale;
	else if (camera_y < 0) camera_y = 0;

	tbox.update();
}
void SimpleState::draw()
{
	int ix = m_input.getMouseX() - camera_halfwidth + camera_x;
	if (ix > 0) ix /= (tilemap->tile_size * scale); // to correct ex: -0.5 being / to 0.

	int iy = m_input.getMouseY() - camera_halfheight + camera_y;
	if (iy > 0) iy /= (tilemap->tile_size * scale);

	int offset_x = camera_halfwidth - camera_x;
	int offset_y = camera_halfheight - camera_y;

	for (int i = 0; i < world->num_layers; ++i)
	{
		for (int y = 0; y < world->height; ++y)
		{
			for (int x = 0; x < world->width; ++x)
			{
				int tile = world->tiles[getTile(world, x, y, i)];

				if (tile == -1) continue;

				int xx = tile % tilemap->tiles_wide;
				int yy = tile / tilemap->tiles_high;
				al_draw_tinted_scaled_rotated_bitmap_region(
					tilemap->bmp,
					xx * tilemap->tile_size,
					yy * tilemap->tile_size,
					tilemap->tile_size, tilemap->tile_size,
					al_map_rgb(255, 255, 255),
					0, 0,
					offset_x + (x * tilemap->tile_size * scale), offset_y + (y * tilemap->tile_size * scale),
					scale, scale, 0.f, 0
				);
			}
		}
	}

	if (draw_grid)
	{
		for (int i = offset_y; i <= world->height * tilemap->tile_size * scale + offset_y; i += tilemap->tile_size * scale)
		{
			al_draw_line(offset_x, i, offset_x + (world->width * tilemap->tile_size * scale), i, al_map_rgb(255, 0, 0), 1);
		}

		for (int i = offset_x; i <= world->width * tilemap->tile_size * scale + offset_x; i += tilemap->tile_size * scale)
		{
			al_draw_line(i, offset_y, i, offset_y + (world->height * tilemap->tile_size * scale), al_map_rgb(255, 0, 0), 1);
		}
	}

	if (ix >= 0 && ix < world->width && iy >= 0 && iy < world->height)
	{
		int x1, y1, x2, y2;

		x1 = (ix * world->tilemap->tile_size * scale) + camera_halfwidth - camera_x;
		x2 = x1 + world->tilemap->tile_size * scale;

		y1 = (iy * world->tilemap->tile_size * scale) + camera_halfheight - camera_y;
		y2 = y1 + world->tilemap->tile_size * scale;

		al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(60, 60, 200), 1);
	}

	al_draw_filled_rectangle(6, 6, 261, 300, al_map_rgb(70, 70, 70));
	al_draw_rectangle(6, 6, 261, 300, al_map_rgb(100, 150, 10), 1);
	al_draw_scaled_bitmap(tilemap->bmp, 0, 0, tilemap->width, tilemap->height, 8, 8, 250, 250, 0);

	al_draw_rectangle(8 + (selection % 10 * 25), 8 + (selection / 10 * 25), 8 + (selection % 10 * 25) + 25, 8 + (selection / 10 * 25) + 25, al_map_rgb(255, 0, 0), 1);
	
	al_draw_textf(font, al_map_rgb(255, 255, 255), 8, 256, 0, "Layer: %i", current_layer);

	tbox.draw();
}