#include "SimpleState.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include <allegro5\allegro_primitives.h>

float scale = 2.f;
int menu_width = 180;

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
	level = std::make_shared<Level>("test", 10, 10, 2);
	if (!level->loadTilemap("Dungeon_Tileset.png", 16))
	{
		std::cout << "Failed to load tilemap!" << std::endl;
	}

	font = al_load_font("C:/Windows/Fonts/arial.ttf", 32, 0);
	font_small = al_load_font("C:/Windows/Fonts/arial.ttf", 18, 0);
	if (!font || !font_small) axe::crash("Failed to load arial.ttf!");

	cam.x = level->getWidth() * level->getTileSize() * scale / 2.f;
	cam.y = level->getHeight() * level->getTileSize() * scale / 2.f;
	cam.halfwidth = (draw.getWindow().getWidth() - menu_width) / 2;
	cam.halfheight = draw.getWindow().getHeight() / 2;

	tilemap_scale = float(menu_width - (tilemap_offset_x * 2)) / level->getTilemapWidth();
}
SimpleState::~SimpleState()
{
	al_destroy_font(font);
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
		
		if (current_layer >= level->getNumLayers()) current_layer = level->getNumLayers() - 1;
	}

	int ix = m_input.getMouseX() - cam.halfwidth + cam.x - menu_width;
	if (ix > 0) ix /= (level->getTileSize() * scale); // don't bother dividing if its negative as a negtive x value is not over the level

	if (m_input.getMouseX() <= menu_width) ix = -1;

	int iy = m_input.getMouseY() - cam.halfheight + cam.y;
	if (iy > 0) iy /= (level->getTileSize() * scale);

	bool mouse_in_level;
	mouse_in_level = ix >= 0 && ix < level->getWidth() && iy >= 0 && iy < level->getHeight();

	if (m_input.isMousePressed(axe::MOUSE_LEFT) && !m_input.isMouseDown(axe::MOUSE_RIGHT))
	{
		Command::incrementCommandCounter();
	}
	else if (m_input.isMousePressed(axe::MOUSE_RIGHT) && !m_input.isMouseDown(axe::MOUSE_LEFT))
	{
		Command::incrementCommandCounter();
	}

	if (m_input.isMouseDown(axe::MOUSE_RIGHT) && mouse_in_level) // pressing mouse right while holding mouse left cause
	{
		if (level->getTile(ix, iy, current_layer) != INVALID_TILE)
		{
			vCommands_undo.push_back(std::make_unique<ClearTileCommand>(level, ix, iy, current_layer));
			if (!vCommands_redo.empty()) vCommands_redo.clear();
		}
	}
	else if (m_input.isMouseDown(axe::MOUSE_LEFT))
	{
		if (m_input.getMouseX() <= menu_width)
		{
			ix = (m_input.getMouseX() - tilemap_offset_x);
			ix /= (float(tilemap_scale) * float(level->getTileSize()));
			
			iy = m_input.getMouseY() - tilemap_offset_y;
			iy /= (float(tilemap_scale) * float(level->getTileSize()));
			
			if (ix >= 0 && ix < level->getTilesWide() && iy >= 0 && iy < level->getTilesHigh())
			{
				selection = iy * level->getTilesWide() + ix;
			}
		}
		else if (mouse_in_level)
		{
			if (level->getTile(ix, iy, current_layer) != selection && selection != INVALID_TILE)
			{
				vCommands_undo.push_back(std::make_unique<SetTileCommand>(level, ix, iy, current_layer, selection));

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
		if (!level->load(level->getFileName()))				/// If this fails the previously loaded world will could have corrupt data !!! ///
		{
			axe::crash(std::string("Failed to load map " + level->getFileName()).c_str());
		}
		else
		{
			tbox.insertString(level->getFileName() + " loaded!");

			vCommands_undo.clear();
			vCommands_redo.clear();
		}
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_S, axe::MOD_CTRL))
	{
		level->save();

		tbox.insertString(level->getFileName() + " saved!");
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_Z, axe::MOD_CTRL))
	{
		if (!vCommands_undo.empty() && !redo)
		{
			int id = vCommands_undo.back()->getID();

			while (vCommands_undo.back()->getID() == id)
			{
				vCommands_undo.back()->undo();
				vCommands_redo.push_back(std::move(vCommands_undo.back()));
				vCommands_undo.pop_back();

				if (vCommands_undo.empty()) break;
			}

			tbox.insertString("Undo");
		}

		if (!redo) undo = true;
	}
	else if (m_input.isKeyPressed(ALLEGRO_KEY_Y, axe::MOD_CTRL))
	{
		if (!vCommands_redo.empty() && !undo)
		{
			int id = vCommands_redo.back()->getID();

			while (vCommands_redo.back()->getID() == id)
			{
				vCommands_redo.back()->redo();
				vCommands_undo.push_back(std::move(vCommands_redo.back()));
				vCommands_redo.pop_back();

				if (vCommands_redo.empty()) break;
			}

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

	float p_x = float(cam.x) / (float((level->getWidth() * level->getTileSize()) * scale));
	float p_y = float(cam.y) / (float((level->getHeight()* level->getTileSize()) * scale));

	if (m_input.isMouseWheelDown())
	{
		scale -= 0.5f;
		if (scale < 1.f) scale = 1.f;

		cam.x = (level->getWidth() * level->getTileSize()) * scale * p_x;
		cam.y = (level->getHeight()* level->getTileSize()) * scale * p_y;
	}
	else if (m_input.isMouseWheelUp())
	{
		scale += 0.5f;
		if (scale > 10.f) scale = 10.f;

		cam.x = (level->getWidth() * level->getTileSize()) * scale * p_x;
		cam.y = (level->getHeight()* level->getTileSize()) * scale * p_y;
	}
	
	if (m_input.isMousePressed(axe::MOUSE_MIDDLE) && mouse_in_level)
	{
		int tile = level->getTile(ix, iy, current_layer);
		if (tile != INVALID_TILE) selection = tile;
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
				int id = vCommands_undo.back()->getID();

				while (vCommands_undo.back()->getID() == id)
				{
					vCommands_undo.back()->undo();
					vCommands_redo.push_back(std::move(vCommands_undo.back()));
					vCommands_undo.pop_back();

					if (vCommands_undo.empty()) break;
				}

				tbox.insertString("Undo");
			}
		}
		else if (redo)
		{
			if (!vCommands_redo.empty())
			{
				int id = vCommands_redo.back()->getID();

				while (vCommands_redo.back()->getID() == id)
				{
					vCommands_redo.back()->redo();
					vCommands_undo.push_back(std::move(vCommands_redo.back()));
					vCommands_redo.pop_back();

					if (vCommands_redo.empty()) break;
				}

				tbox.insertString("Redo");
			}
		}
		dTime = 400;
	}

	if (m_input.isKeyDown(ALLEGRO_KEY_A, axe::MOD_NONE)) cam.x -= cam_speed;
	else if (m_input.isKeyDown(ALLEGRO_KEY_D, axe::MOD_NONE)) cam.x += cam_speed;
	if (m_input.isKeyDown(ALLEGRO_KEY_W, axe::MOD_NONE)) cam.y -= cam_speed;
	else if (m_input.isKeyDown(ALLEGRO_KEY_S, axe::MOD_NONE)) cam.y += cam_speed;

	if (cam.x > level->getWidth() * level->getTileSize() * scale) cam.x = level->getWidth() * level->getTileSize() * scale;
	else if (cam.x < 0) cam.x = 0;

	if (cam.y > level->getHeight() * level->getTileSize() * scale) cam.y = level->getHeight() * level->getTileSize() * scale;
	else if (cam.y < 0) cam.y = 0;

	tbox.update();
}
void SimpleState::draw()
{
	float tileSz = level->getTileSize();

	// Calculate tile coords under mouse
	int ix = m_input.getMouseX() - cam.halfwidth + cam.x - menu_width;
	if (ix > 0) ix /= (tileSz * scale); // to correct ex: -0.5 being / to 0.

	if (m_input.getMouseX() <= menu_width) ix = -1;

	int iy = m_input.getMouseY() - cam.halfheight + cam.y;
	if (iy > 0) iy /= (tileSz * scale);

	// Calulate Level offset
	int offset_x = cam.halfwidth - cam.x + menu_width;
	int offset_y = cam.halfheight - cam.y;

	int vis_tiles_x = (cam.halfwidth * 2) / (tileSz * scale);
	int vis_tiles_y = (cam.halfheight * 2) / (tileSz * scale);

	int start_x, start_y, end_x, end_y;

	start_x = std::max(int((cam.x - cam.halfwidth) / (tileSz * scale)), 0);
	start_y = std::max(int((cam.y - cam.halfheight) / (tileSz * scale)), 0);

	end_x = std::min(start_x + vis_tiles_x + 2, level->getWidth());
	end_y = std::min(start_y + vis_tiles_y + 2, level->getHeight());

	al_draw_filled_rectangle(
		std::max(offset_x, menu_width),
		std::max(offset_y, 0),
		std::min(int(offset_x + (tileSz * scale * level->getWidth())), m_draw.getWindow().getWidth()),
		std::min(int(offset_y + (tileSz * scale * level->getHeight())), m_draw.getWindow().getHeight()),
		al_map_rgb(37, 19, 26)
	);

	al_set_clipping_rectangle(menu_width, 0, m_draw.getWindow().getWidth() - menu_width, m_draw.getWindow().getHeight());

	// Draw tiles
	al_hold_bitmap_drawing(true);
	for (int i = 0; i <= level->getNumLayers(); ++i)
	{
		if (i == level->getNumLayers() && !draw_grid) break;
		else
		{
			//clip edges?
			al_hold_bitmap_drawing(false);
		}
		for (int y = start_y; y < end_y; ++y)
		{
			for (int x = start_x; x < end_x; ++x)
			{
				if (i == level->getNumLayers() && draw_grid)
				{
					int xx, yy;

					xx = offset_x + (x * tileSz * scale);
					yy = offset_y + (y * tileSz * scale);

					al_draw_line(xx, yy - 4, xx, yy + 3, al_map_rgb(255, 0, 255), 1);
					al_draw_line(xx - 4, yy, xx + 3, yy, al_map_rgb(255, 0, 255), 1);

					if (x + 1 == end_x)
					{
						al_draw_line(xx + tileSz * scale, yy - 4, xx + tileSz * scale, yy + 3, al_map_rgb(255, 0, 255), 1);
						al_draw_line(xx + tileSz * scale - 4, yy, xx + tileSz * scale + 3, yy, al_map_rgb(255, 0, 255), 1);
					}
					if (y + 1 == end_y)
					{
						al_draw_line(xx, yy + tileSz * scale - 4, xx, yy + tileSz * scale + 3, al_map_rgb(255, 0, 255), 1);
						al_draw_line(xx - 4, yy + tileSz * scale, xx + 3, yy + tileSz * scale, al_map_rgb(255, 0, 255), 1);
					}

					if (y + 1 == end_y && x + 1 == end_x)
					{
						al_draw_line(xx + tileSz * scale, yy + tileSz * scale - 4, xx + tileSz * scale, yy + tileSz * scale + 3, al_map_rgb(255, 0, 255), 1);
						al_draw_line(xx + tileSz * scale - 4, yy + tileSz * scale, xx + tileSz * scale + 3, yy + tileSz * scale, al_map_rgb(255, 0, 255), 1);
					}
					
					continue;
				}

				int tile = level->getTile(x, y, i);

				if (!(tile == INVALID_TILE && i == 0)) level->drawTile(offset_x + (x * tileSz * scale), offset_y + (y * tileSz * scale), tile, scale);
			}
		}
	}

	// Draw Selected tile at mouse position and layer number
	if (ix >= 0 && ix < level->getWidth() && iy >= 0 && iy < level->getHeight() && selection != INVALID_TILE)
	{
		int x1, y1, x2, y2;

		x1 = (ix * tileSz * scale) + cam.halfwidth - cam.x + menu_width;
		x2 = x1 + tileSz * scale;

		y1 = (iy * tileSz * scale) + cam.halfheight - cam.y;
		y2 = y1 + tileSz * scale;

		if (selection != INVALID_TILE)
		{
			int xx = selection % level->getTilesWide();
			int yy = selection / level->getTilesWide();
			al_draw_tinted_scaled_rotated_bitmap_region(
				level->getAllegroBitmap(),
				xx * tileSz,
				yy * tileSz,
				tileSz, tileSz,
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
	level->drawTilemap(tilemap_offset_x, tilemap_offset_y, tilemap_scale);

	// Draw selected tile on tilemap
	int t_wide = level->getTilesWide();

	if (selection != -1) al_draw_rectangle(
		tilemap_offset_x + ((selection % t_wide) * tileSz * tilemap_scale),
		tilemap_offset_y + ((selection / t_wide) * tileSz* tilemap_scale),
		tilemap_offset_x + ((selection % t_wide) * tileSz* tilemap_scale) + (tileSz* tilemap_scale),
		tilemap_offset_y + ((selection / t_wide) * tileSz* tilemap_scale) + (tileSz* tilemap_scale),
		al_map_rgb(255, 0, 0), 1);
	
	// Draw Textbox, unnecessary?
	tbox.draw();
}