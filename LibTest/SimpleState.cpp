#include "SimpleState.h"
#include <iostream>
#include <fstream>
#include <iomanip>

#include <allegro5\allegro_primitives.h>

float scale = 4.f;
bool redo = false;
bool undo = false;
unsigned long long dTime = 0;

SimpleState::SimpleState(axe::StateManager & states, axe::InputHandler & input, axe::EventHandler & events, axe::DrawEngine & draw)
	: AbstractState(states, input, events, draw), draw_grid(true),
	tbox(8, 316, "C:/Windows/Fonts/arial.ttf", 24, al_map_rgb(255, 100, 100)),
	current_layer(0)
{
	srand(time(NULL));

	tilemap = loadTilemap("Dungeon_Tileset.png", 16, 10, 10);
	world = createWorld("Level 3", 30, 15, 2, tilemap);

	if (!world) axe::crash("Failed to create world!");

	font = al_load_font("C:/Windows/Fonts/arial.ttf", 32, 0);
	font_small = al_load_font("C:/Windows/Fonts/arial.ttf", 18, 0);
	if (!font || !font_small) axe::crash("Failed to load arial.ttf!");

	cam.x = 0;
	cam.y = 0;
	cam.halfwidth = draw.getWindow().getWidth() / 2;
	cam.halfheight = draw.getWindow().getHeight() / 2;
}
SimpleState::~SimpleState()
{
	al_destroy_font(font);

	tilemap = destroyTilemap(tilemap);
	world = destroyWorld(world);
}

void SimpleState::pause() { }
void SimpleState::resume() { }

void SimpleState::handleEvents()
{
	if (m_events.eventIs(ALLEGRO_EVENT_DISPLAY_RESIZE))
	{
		// Need to remove dependency on Allegro for this in axeLib

		const ALLEGRO_EVENT ev = m_events.getEvent();

		cam.halfwidth = ev.display.width / 2;
		cam.halfheight = ev.display.height / 2;
	}

	char pressed = 0;
	if (pressed = m_input.getChar())
	{
		switch (pressed)
		{
		case ALLEGRO_KEY_1:
		case ALLEGRO_KEY_PAD_1:
			current_layer = 0;
			break;
		case ALLEGRO_KEY_2:
		case ALLEGRO_KEY_PAD_2:
			current_layer = 1;
			break;
		case ALLEGRO_KEY_3:
		case ALLEGRO_KEY_PAD_3:
			current_layer = 2;
			break;
		case ALLEGRO_KEY_4:
		case ALLEGRO_KEY_PAD_4:
			current_layer = 3;
			break;
		}
		
		if (current_layer >= world->num_layers) current_layer = world->num_layers - 1;
	}

	int ix = m_input.getMouseX() - cam.halfwidth + cam.x;
	if (ix > 0) ix /= (tilemap->tile_size * scale); // to correct ex: -0.5 being / to 0.

	int iy = m_input.getMouseY() - cam.halfheight + cam.y;
	if (iy > 0) iy /= (tilemap->tile_size * scale);

	if (m_input.isMouseDown(axe::MOUSE_RIGHT))
	{
		if (ix >= 0 && ix < world->width && iy >= 0 && iy < world->height)
		{
			int index = getTileIndex(world, ix, iy, current_layer);
			if (world->tiles[index] != -1)
			{
				vCommands_undo.push_back(std::unique_ptr<ClearTileCommand>(new ClearTileCommand(world, ix, iy, current_layer, -1)));

				if (!vCommands_redo.empty()) vCommands_redo.clear();
			}
		}
	}
	else if (m_input.isMouseDown(axe::MOUSE_LEFT))
	{
		int sel_x = (m_input.getMouseX() - 8) / 25;
		int sel_y = (m_input.getMouseY() - 8) / 25;

		if (sel_x >= 0 && sel_x < tilemap->tiles_wide && sel_y >= 0 && sel_y < tilemap->tiles_high)
		{
			selection = sel_y * tilemap->tiles_wide + sel_x;
			ix = -1; // set ix/iy to -1 to ignore parsing the map for the click as the tilemap was clicked.
			iy = -1;
		}

		
		if (ix >= 0 && ix < world->width && iy >= 0 && iy < world->height && selection != -1)
		{
			int index = getTileIndex(world, ix, iy, current_layer);
			if (world->tiles[index] != selection)
			{
				vCommands_undo.push_back(std::unique_ptr<SetTileCommand>(new SetTileCommand(world, ix, iy, current_layer, selection)));
				
				if (!vCommands_redo.empty()) vCommands_redo.clear();
			}
		}
	}

	if (m_input.isKeyPressed(ALLEGRO_KEY_C))
	{
		selection = -1;
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_G))
	{
		draw_grid = !draw_grid;
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_L))
	{
		if (world) world = destroyWorld(world);

		world = loadWorld("Level 3.bin", tilemap);

		if (!world) axe::crash(std::string("Failed to load map " + world->file_name).c_str());

		tbox.insertString(world->file_name + " loaded!");

		vCommands_undo.clear();
		vCommands_redo.clear();
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_S, axe::MOD_CTRL))
	{
		saveWorld(world);
		tbox.insertString(world->file_name + " saved!");
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_Z, axe::MOD_CTRL))
	{
		if (!vCommands_undo.empty() && !redo)
		{
			vCommands_undo.back()->undo();
			vCommands_redo.push_back(std::move(vCommands_undo.back()));
			vCommands_undo.pop_back();
		}

		if (!redo) undo = true;
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_Y, axe::MOD_CTRL))
	{
		if (!vCommands_redo.empty() && !undo)
		{
			vCommands_redo.back()->redo();
			vCommands_undo.push_back(std::move(vCommands_redo.back()));
			vCommands_redo.pop_back();
		}
		
		if (!undo) redo = true;
	}
	else if (m_input.isKeyReleased(ALLEGRO_KEY_Z))
	{
		undo = false;
		dTime = 0;
	}
	else if (m_input.isKeyReleased(ALLEGRO_KEY_Y))
	{
		redo = false;
		dTime = 0;
	}

	float p_x = float(cam.x) / (float((world->width * world->tilemap->tile_size) * scale));
	float p_y = float(cam.y) / (float((world->height * world->tilemap->tile_size) * scale));

	if (m_input.isMouseWheelDown())
	{
		scale -= 0.5f;
		if (scale < 1.f) scale = 1.f;

		cam.x = (world->width * world->tilemap->tile_size) * scale * p_x;
		cam.y = (world->height * world->tilemap->tile_size) * scale * p_y;
	}
	else if (m_input.isMouseWheelUp())
	{
		scale += 0.5f;
		if (scale > 10.f) scale = 10.f;

		cam.x = (world->width * world->tilemap->tile_size) * scale * p_x;
		cam.y = (world->height * world->tilemap->tile_size) * scale * p_y;
	}
}
void SimpleState::update(unsigned long long deltaTime)
{
	if (undo || redo) dTime += deltaTime;

	if (dTime >= 500)
	{
		if (undo)
		{
			if (!vCommands_undo.empty())
			{
				vCommands_undo.back()->undo();
				vCommands_redo.push_back(std::move(vCommands_undo.back()));
				vCommands_undo.pop_back();
			}
		}
		else if (redo)
		{
			if (!vCommands_redo.empty())
			{
				vCommands_redo.back()->redo();
				vCommands_undo.push_back(std::move(vCommands_redo.back()));
				vCommands_redo.pop_back();
			}
		}
		dTime = 400;
	}

	if (m_input.isKeyDown(ALLEGRO_KEY_A, axe::MOD_NONE)) cam.x -= 16;
	else if (m_input.isKeyDown(ALLEGRO_KEY_D, axe::MOD_NONE)) cam.x += 16;
	if (m_input.isKeyDown(ALLEGRO_KEY_W, axe::MOD_NONE)) cam.y -= 16;
	else if (m_input.isKeyDown(ALLEGRO_KEY_S, axe::MOD_NONE))
	{
		cam.y += 16;
	}

	if (cam.x > world->width * world->tilemap->tile_size * scale) cam.x = world->width * world->tilemap->tile_size * scale;
	else if (cam.x < 0) cam.x = 0;

	if (cam.y > world->height * world->tilemap->tile_size * scale) cam.y = world->height * world->tilemap->tile_size * scale;
	else if (cam.y < 0) cam.y = 0;

	tbox.update();
}
void SimpleState::draw()
{
	int ix = m_input.getMouseX() - cam.halfwidth + cam.x;
	if (ix > 0) ix /= (tilemap->tile_size * scale); // to correct ex: -0.5 being / to 0.

	int iy = m_input.getMouseY() - cam.halfheight + cam.y;
	if (iy > 0) iy /= (tilemap->tile_size * scale);

	int offset_x = cam.halfwidth - cam.x;
	int offset_y = cam.halfheight - cam.y;

	al_draw_filled_rectangle(offset_x, offset_y, offset_x + (world->width * world->tilemap->tile_size * scale), offset_y + (world->height * world->tilemap->tile_size * scale), al_map_rgb(10, 10, 10));

	for (int i = 0; i < world->num_layers; ++i)
	{
		for (int y = 0; y < world->height; ++y)
		{
			for (int x = 0; x < world->width; ++x)
			{
				int tile = getTileIndex(world, x, y, i);

				if (tile == -1) continue;

				tile = world->tiles[tile];

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

		x1 = (ix * world->tilemap->tile_size * scale) + cam.halfwidth - cam.x;
		x2 = x1 + world->tilemap->tile_size * scale;

		y1 = (iy * world->tilemap->tile_size * scale) + cam.halfheight - cam.y;
		y2 = y1 + world->tilemap->tile_size * scale;

		if (selection != -1)
		{
			int xx = selection % tilemap->tiles_wide;
			int yy = selection / tilemap->tiles_high;
			al_draw_tinted_scaled_rotated_bitmap_region(
				tilemap->bmp,
				xx * tilemap->tile_size,
				yy * tilemap->tile_size,
				tilemap->tile_size, tilemap->tile_size,
				al_map_rgb(255, 255, 255),
				0, 0,
				x1, y1,
				scale, scale, 0.f, 0
			);
		}

		al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(60, 60, 200), 1);

		al_draw_textf(font_small, al_map_rgb(60, 60, 200), x2 - 2, y1, ALLEGRO_ALIGN_RIGHT, "%i", current_layer+1);
	}

	al_draw_filled_rectangle(6, 6, 260, 260, al_map_rgb(70, 70, 70));
	al_draw_rectangle(6, 6, 260, 260, al_map_rgb(100, 150, 10), 1);
	al_draw_scaled_bitmap(tilemap->bmp, 0, 0, tilemap->width, tilemap->height, 8, 8, 250, 250, 0);

	if (selection != -1) al_draw_rectangle(8 + (selection % 10 * 25), 8 + (selection / 10 * 25), 8 + (selection % 10 * 25) + 25, 8 + (selection / 10 * 25) + 25, al_map_rgb(255, 0, 0), 1);
	
	//al_draw_textf(font, al_map_rgb(255, 255, 255), 8, 262, 0, "Layer: %i", current_layer);

	tbox.draw();
}