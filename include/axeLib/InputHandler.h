#pragma once

#include <string>
#include <allegro5/allegro.h>

#include "axeLib/util/Timer.h" //For timing character deletion
#include "axeLib/util/Logger.h"

namespace axe
{
	// Mouse key must be bit field, Allegro limitation
	const char MOUSE_LEFT	= 0b0001;
	const char MOUST_RIGHT	= 0b0010;
	const char MOUSE_MIDDLE	= 0b0100;

	const char MOD_IGNORE	= 0b0000; // Ignores modifiers that are pressed (Allowing any modifier to be pressed)
	const char MOD_NONE		= 0b0001; // Requires that no modifiers be pressed
	const char MOD_SHIFT	= 0b0010;
	const char MOD_CTRL		= 0b0100;
	const char MOD_ALT		= 0b1000;

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

		void enableTextInput(unsigned max_length, bool allow_caps, bool allow_newline,
			bool allow_digits, bool allow_specials, bool allow_space);
		void disableTextInput();
		std::string getTextInput();

		void setInputString(const std::string &str);
		void clearInputString();

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
		void handleTextInput(const ALLEGRO_EVENT &ev);
		void stringPushBack(char c);

		bool m_backspace;

		bool m_text_input_enabled;
		bool m_allow_caps;
		bool m_allow_newline;
		bool m_allow_digits;
		bool m_allow_specials;
		bool m_allow_space;

		char m_mod_flags;

		unsigned m_max_input_length;
		
		std::string m_input_string;
	
		m_secs m_backspace_wait;
		Timer m_timer;

		ALLEGRO_KEYBOARD_STATE m_prev_key_state;
		ALLEGRO_KEYBOARD_STATE m_cur_key_state;

		ALLEGRO_MOUSE_STATE m_prev_mouse_state;
		ALLEGRO_MOUSE_STATE m_cur_mouse_state;

	};

};