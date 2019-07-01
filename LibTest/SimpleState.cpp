#include "SimpleState.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include <allegro5\allegro_primitives.h>

float scale = 2.f;
int menu_width = 256;

float tilemap_scale = 1.f;
int tilemap_offset_x = 6;
int tilemap_offset_y = 6;

bool redo = false;
bool undo = false;
unsigned long long dTime = 0;

SimpleState::SimpleState(axe::StateManager & states, axe::InputHandler & input, axe::EventHandler & events, axe::DrawEngine & draw)
	: AbstractState(states, input, events, draw), draw_grid(false),
	tbox(8, 264, "C:/Windows/Fonts/arial.ttf", 24, al_map_rgb(255, 100, 100)),
	current_layer(0), selection(-1)
{
	srand(time(NULL));

	tilemap = loadTilemap("Dungeon_Tileset.png", 8, 20, 20);
	Level = createLevel("test", 10, 10, 1, tilemap);

	if (!Level) axe::crash("Failed to create Level!");

	font = al_load_font("C:/Windows/Fonts/arial.ttf", 32, 0);
	font_small = al_load_font("C:/Windows/Fonts/arial.ttf", 18, 0);
	if (!font || !font_small) axe::crash("Failed to load arial.ttf!");

	cam.x = Level->width * tilemap->tile_size * scale / 2.f;
	cam.y = Level->height * tilemap->tile_size * scale / 2.f;
	cam.halfwidth = (draw.getWindow().getWidth() - menu_width) / 2;
	cam.halfheight = draw.getWindow().getHeight() / 2;

	tilemap_scale = float(menu_width - (tilemap_offset_x * 2)) / float(tilemap->width);
}
SimpleState::~SimpleState()
{
	al_destroy_font(font);

	tilemap = destroyTilemap(tilemap);
	Level = destroyLevel(Level);
}

void SimpleState::pause() { }
void SimpleState::resume() { }

void SimpleState::handleEvents()
{
	cam.halfwidth = (m_draw.getWindow().getWidth() - menu_width) / 2;
	cam.halfheight = m_draw.getWindow().getHeight() / 2;

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
		
		if (current_layer >= Level->num_layers) current_layer = Level->num_layers - 1;
	}

	int ix = m_input.getMouseX() - cam.halfwidth + cam.x - menu_width;
	if (ix > 0) ix /= (tilemap->tile_size * scale); // to correct ex: -0.5 being / to 0.

	if (m_input.getMouseX() <= menu_width) ix = -1;

	int iy = m_input.getMouseY() - cam.halfheight + cam.y;
	if (iy > 0) iy /= (tilemap->tile_size * scale);

	if (m_input.isMouseDown(axe::MOUSE_RIGHT))
	{
		if (ix >= 0 && ix < Level->width && iy >= 0 && iy < Level->height)
		{
			int index = getTileIndex(Level, ix, iy, current_layer);
			if (Level->tiles[index] != -1)
			{
				vCommands_undo.push_back(std::unique_ptr<ClearTileCommand>(new ClearTileCommand(Level, ix, iy, current_layer, -1)));

				if (!vCommands_redo.empty()) vCommands_redo.clear();
			}
		}
	}
	else if (m_input.isMouseDown(axe::MOUSE_LEFT))
	{
		if (m_input.getMouseX() <= menu_width)
		{
			ix = (m_input.getMouseX() - tilemap_offset_x);
			ix /= (float(tilemap_scale) * float(tilemap->tile_size));
			
			iy = m_input.getMouseY() - tilemap_offset_y;
			iy /= (float(tilemap_scale) * float(tilemap->tile_size));
			
			if (ix >= 0 && ix < tilemap->tiles_wide && iy >= 0 && iy < tilemap->tiles_high)
			{
				selection = iy * tilemap->tiles_wide + ix;
			}
		}
		else
		{
			if (ix >= 0 && ix < Level->width && iy >= 0 && iy < Level->height && selection != -1)
			{
				int index = getTileIndex(Level, ix, iy, current_layer);
				if (Level->tiles[index] != selection)
				{

					std::cout << "Selection: " << selection << std::endl;
					vCommands_undo.push_back(std::unique_ptr<SetTileCommand>(new SetTileCommand(Level, ix, iy, current_layer, selection)));

					if (!vCommands_redo.empty()) vCommands_redo.clear();
				}
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
		if (Level) Level = destroyLevel(Level);

		Level = loadLevel("Level 3.bin", tilemap);

		if (!Level) axe::crash(std::string("Failed to load map " + Level->file_name).c_str());

		tbox.insertString(Level->file_name + " loaded!");

		vCommands_undo.clear();
		vCommands_redo.clear();
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_S, axe::MOD_CTRL))
	{
		saveLevel(Level);
		tbox.insertString(Level->file_name + " saved!");
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_Z, axe::MOD_CTRL))
	{
		if (!vCommands_undo.empty() && !redo)
		{
			vCommands_undo.back()->undo();
			vCommands_redo.push_back(std::move(vCommands_undo.back()));
			vCommands_undo.pop_back();

			tbox.insertString("Undo");
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

			tbox.insertString("Redo");
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

	float p_x = float(cam.x) / (float((Level->width * Level->tilemap->tile_size) * scale)); // Need to adjust
	float p_y = float(cam.y) / (float((Level->height * Level->tilemap->tile_size) * scale));

	if (m_input.isMouseWheelDown())
	{
		scale -= 0.5f;
		if (scale < 1.f) scale = 1.f;

		cam.x = (Level->width * Level->tilemap->tile_size) * scale * p_x;
		cam.y = (Level->height * Level->tilemap->tile_size) * scale * p_y;
	}
	else if (m_input.isMouseWheelUp())
	{
		scale += 0.5f;
		if (scale > 10.f) scale = 10.f;

		cam.x = (Level->width * Level->tilemap->tile_size) * scale * p_x;
		cam.y = (Level->height * Level->tilemap->tile_size) * scale * p_y;
	}
	
	if (m_input.isMousePressed(axe::MOUSE_MIDDLE))
	{
		int tile = getTileIndex(Level, ix, iy, current_layer);
		selection = Level->tiles[tile];
	}
}
void SimpleState::update(unsigned long long deltaTime)
{
	float cam_speed = (deltaTime / 1000.f * 400.f) * (0.5f * scale);

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

				tbox.insertString("Undo");
			}
		}
		else if (redo)
		{
			if (!vCommands_redo.empty())
			{
				vCommands_redo.back()->redo();
				vCommands_undo.push_back(std::move(vCommands_redo.back()));
				vCommands_redo.pop_back();

				tbox.insertString("Redo");
			}
		}
		dTime = 400;
	}

	if (m_input.isKeyDown(ALLEGRO_KEY_A, axe::MOD_NONE)) cam.x -= cam_speed;
	else if (m_input.isKeyDown(ALLEGRO_KEY_D, axe::MOD_NONE)) cam.x += cam_speed;
	if (m_input.isKeyDown(ALLEGRO_KEY_W, axe::MOD_NONE)) cam.y -= cam_speed;
	else if (m_input.isKeyDown(ALLEGRO_KEY_S, axe::MOD_NONE)) cam.y += cam_speed;

	if (cam.x > Level->width * Level->tilemap->tile_size * scale) cam.x = Level->width * Level->tilemap->tile_size * scale;
	else if (cam.x < 0) cam.x = 0;

	if (cam.y > Level->height * Level->tilemap->tile_size * scale) cam.y = Level->height * Level->tilemap->tile_size * scale;
	else if (cam.y < 0) cam.y = 0;

	tbox.update();
}
void SimpleState::draw()
{
	// Calculate tile coords under mouse
	int ix = m_input.getMouseX() - cam.halfwidth + cam.x - menu_width;
	if (ix > 0) ix /= (tilemap->tile_size * scale); // to correct ex: -0.5 being / to 0.

	if (m_input.getMouseX() <= menu_width) ix = -1;

	int iy = m_input.getMouseY() - cam.halfheight + cam.y;
	if (iy > 0) iy /= (tilemap->tile_size * scale);

	// Calulate Level offset
	int offset_x = cam.halfwidth - cam.x + menu_width;
	int offset_y = cam.halfheight - cam.y;

	int vis_tiles_x = (cam.halfwidth * 2) / float(tilemap->tile_size * scale);
	int vis_tiles_y = (cam.halfheight * 2) / float(tilemap->tile_size * scale);

	int start_x, start_y, end_x, end_y;

	start_x = std::max(int((cam.x - cam.halfwidth) / (tilemap->tile_size * scale)), 0);
	start_y = std::max(int((cam.y - cam.halfheight) / (tilemap->tile_size * scale)), 0);

	end_x = std::min(start_x + vis_tiles_x + 2, Level->width);
	end_y = std::min(start_y + vis_tiles_y + 2, Level->height);

	al_draw_filled_rectangle(
		std::max(offset_x, menu_width),
		std::max(offset_y, 0),
		std::min(int(offset_x + (tilemap->tile_size * scale * Level->width)), m_draw.getWindow().getWidth()),
		std::min(int(offset_y + (tilemap->tile_size * scale * Level->height)), m_draw.getWindow().getHeight()),
		al_map_rgb(37, 19, 26)
	);

	al_set_clipping_rectangle(menu_width, 0, m_draw.getWindow().getWidth() - menu_width, m_draw.getWindow().getHeight());

	// Draw tiles
	al_hold_bitmap_drawing(true);
	for (int i = 0; i <= Level->num_layers; ++i)
	{
		if (i == Level->num_layers && !draw_grid) break;
		else
		{
			//clip edges?
			al_hold_bitmap_drawing(false);
		}
		for (int y = start_y; y < end_y; ++y)
		{
			for (int x = start_x; x < end_x; ++x)
			{
				if (i == Level->num_layers && draw_grid)
				{
					int xx, yy;

					xx = offset_x + (x * tilemap->tile_size * scale);
					yy = offset_y + (y * tilemap->tile_size * scale);

					al_draw_line(xx, yy - 4, xx, yy + 3, al_map_rgb(255, 0, 255), 1);
					al_draw_line(xx - 4, yy, xx + 3, yy, al_map_rgb(255, 0, 255), 1);

					if (x + 1 == end_x)
					{
						al_draw_line(xx + tilemap->tile_size * scale, yy - 4, xx + tilemap->tile_size * scale, yy + 3, al_map_rgb(255, 0, 255), 1);
						al_draw_line(xx + tilemap->tile_size * scale - 4, yy, xx + tilemap->tile_size * scale + 3, yy, al_map_rgb(255, 0, 255), 1);
					}
					if (y + 1 == end_y)
					{
						al_draw_line(xx, yy + tilemap->tile_size * scale - 4, xx, yy + tilemap->tile_size * scale + 3, al_map_rgb(255, 0, 255), 1);
						al_draw_line(xx - 4, yy + tilemap->tile_size * scale, xx + 3, yy + tilemap->tile_size * scale, al_map_rgb(255, 0, 255), 1);
					}

					if (y + 1 == end_y && x + 1 == end_x)
					{
						al_draw_line(xx + tilemap->tile_size * scale, yy + tilemap->tile_size * scale - 4, xx + tilemap->tile_size * scale, yy + tilemap->tile_size * scale + 3, al_map_rgb(255, 0, 255), 1);
						al_draw_line(xx + tilemap->tile_size * scale - 4, yy + tilemap->tile_size * scale, xx + tilemap->tile_size * scale + 3, yy + tilemap->tile_size * scale, al_map_rgb(255, 0, 255), 1);
					}
					
					continue;
				}

				int tile = getTileIndex(Level, x, y, i);
				tile = Level->tiles[tile];

				if (tile == -1 && i == 0)
				{
					/*std::cout << "Tile is -1" << std::endl;
					int x1, y1, x2, y2;

					x1 = offset_x + (x * tilemap->tile_size * scale);
					y1 = offset_y + (y * tilemap->tile_size * scale);

					x2 = x1 + tilemap->tile_size * scale;
					y2 = y1 + tilemap->tile_size * scale;

					al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(10, 10, 10));*/
				}
				else
				{
					int xx = tile % tilemap->tiles_wide * tilemap->tile_size;
					int yy = tile / tilemap->tiles_wide * tilemap->tile_size;
					al_draw_tinted_scaled_rotated_bitmap_region(
						tilemap->bmp, xx, yy,
						tilemap->tile_size, tilemap->tile_size,
						al_map_rgb(255, 255, 255),
						0, 0,
						offset_x + (x * tilemap->tile_size * scale), offset_y + (y * tilemap->tile_size * scale),
						scale, scale, 0.f, 0
					);
				}
			}
		}
	}

	// Draw Selected tile at mouse position and layer number
	if (ix >= 0 && ix < Level->width && iy >= 0 && iy < Level->height && selection != -1)
	{
		int x1, y1, x2, y2;

		x1 = (ix * Level->tilemap->tile_size * scale) + cam.halfwidth - cam.x + menu_width;
		x2 = x1 + Level->tilemap->tile_size * scale;

		y1 = (iy * Level->tilemap->tile_size * scale) + cam.halfheight - cam.y;
		y2 = y1 + Level->tilemap->tile_size * scale;

		if (selection != -1)
		{
			int xx = selection % tilemap->tiles_wide;
			int yy = selection / tilemap->tiles_wide;
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

		al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(255, 255, 255), 1);

		al_draw_textf(font_small, al_map_rgb(255, 255, 255), x2 - 2, y1, ALLEGRO_ALIGN_RIGHT, "%i", current_layer+1);
	}

	al_reset_clipping_rectangle();
	al_draw_line(menu_width, 0, menu_width, m_draw.getWindow().getHeight(), al_map_rgb(90, 10, 0), 2);

	// Draw Tilemap selection box
	al_draw_scaled_bitmap(tilemap->bmp,
		0, 0, tilemap->width, tilemap->height,
		tilemap_offset_x, tilemap_offset_y, float(tilemap->width) * tilemap_scale, float(tilemap->height) * tilemap_scale, 0);

	// Draw selected tile on tilemap
	if (selection != -1) al_draw_rectangle(
		tilemap_offset_x + ((selection % tilemap->tiles_wide) * tilemap->tile_size * tilemap_scale),
		tilemap_offset_y + ((selection / tilemap->tiles_wide) * tilemap->tile_size * tilemap_scale),
		tilemap_offset_x + ((selection % tilemap->tiles_wide) * tilemap->tile_size * tilemap_scale) + (tilemap->tile_size * tilemap_scale),
		tilemap_offset_y + ((selection / tilemap->tiles_wide) * tilemap->tile_size * tilemap_scale) + (tilemap->tile_size * tilemap_scale),
		al_map_rgb(255, 0, 0), 1);
	
	// Draw Textbox, unnecessary?
	tbox.draw();
}