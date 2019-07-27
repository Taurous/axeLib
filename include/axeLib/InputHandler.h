#pragma once

#include <string>
#include <allegro5/allegro.h>

namespace axe
{
	enum
	{
		MOUSE_LEFT = 1,
		MOUSE_RIGHT = 2,
		MOUSE_MIDDLE = 3
	};

	const char MOD_IGNORE = 0b0000; // Ignores modifiers that are pressed (Allowing any modifier to be pressed)
	const char MOD_NONE = 0b0001; // Requires that no modifiers be pressed
	const char MOD_SHIFT = 0b0010;
	const char MOD_CTRL = 0b0100;
	const char MOD_ALT = 0b1000;

	class InputHandler
	{
	public:
		InputHandler();

		InputHandler(const InputHandler &other) = delete;
		InputHandler &operator=(const InputHandler &other) = delete;

		InputHandler(InputHandler &&other) = delete;
		InputHandler &operator=(InputHandler &&other) = delete;

		~InputHandler();

		void getInput(const ALLEGRO_EVENT &ev);

		char getChar() const;

		bool isKeyPressed(const int key, const char &mod = MOD_IGNORE) const;
		bool isKeyReleased(const int key, const char &mod = MOD_IGNORE) const;
		bool isKeyDown(const int key, const char &mod = MOD_IGNORE) const;

		bool isMousePressed(const int button, const char &mod = MOD_IGNORE) const;
		bool isMouseReleased(const int button, const char &mod = MOD_IGNORE) const;
		bool isMouseDown(const int button, const char &mod = MOD_IGNORE) const;
		bool isMouseWheelDown(const char &mod = MOD_IGNORE) const;
		bool isMouseWheelUp(const char &mod = MOD_IGNORE) const;

		int getMouseX() const;
		int getMouseY() const;
		bool isMouseInWindow() const;

	private:
		char last_pressed;
		char m_mod_flags;

		ALLEGRO_KEYBOARD_STATE m_prev_key_state;
		ALLEGRO_KEYBOARD_STATE m_cur_key_state;

		ALLEGRO_MOUSE_STATE m_prev_mouse_state;
		ALLEGRO_MOUSE_STATE m_cur_mouse_state;
	};

};