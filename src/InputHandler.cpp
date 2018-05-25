#include "axeLib\InputHandler.h"

#include "axeLib\util\Util.h"

using namespace axe;

const unsigned char letters[] = {
	  0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
	'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
	'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	')', '!', '@', '#', '$', '%', '^', '&', '*', '(' // Must fix %
};

const m_secs BACKSPACE_WORD_DELETE_TIME(100); //Time between word deletes
const m_secs BACKSPACE_CHAR_DELETE_TIME(30); //Time between character deletes
const m_secs BACKSPACE_WAIT_TIME(400); //Time after press before continuous deleting

void deleteCharacter(std::string &str)
{
	if (!str.empty())
	{
		str.pop_back();
	}
}
void deleteWord(std::string &str)
{
	while (true)
	{
		if (str.empty()) break;

		if (str.back() != ' ' && str.back() != '\n')
		{
			str.pop_back();
		}
		else
		{
			str.pop_back();
			break;
		}
	}
}

InputHandler::InputHandler()
	: m_mod_flags(0)
	, m_backspace(false)
	, m_max_input_length(0)
	, m_backspace_wait(BACKSPACE_WAIT_TIME)
{
	axe::allegro_init();
	al_install_keyboard();
	al_install_mouse();

	al_get_mouse_state(&m_prev_mouse_state);
	m_cur_mouse_state = m_prev_mouse_state;
	al_get_keyboard_state(&m_prev_key_state);
	m_cur_key_state = m_prev_key_state;

	axe::log(LOGGER_MESSAGE, "InputHandler initialized\n");
}

InputHandler::~InputHandler()
{
	al_uninstall_keyboard();
	al_uninstall_mouse();
}

void InputHandler::stringPushBack(char c)
{
	if (m_input_string.size() < m_max_input_length) m_input_string.push_back(c);
}

void InputHandler::getInput(const ALLEGRO_EVENT &ev)
{
	m_prev_mouse_state = m_cur_mouse_state;
	al_get_mouse_state(&m_cur_mouse_state);

	m_prev_key_state = m_cur_key_state;
	al_get_keyboard_state(&m_cur_key_state);

	if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		// Get Modifiers Pressed
		switch (ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_LSHIFT:
		case ALLEGRO_KEY_RSHIFT:
			setBit(m_mod_flags, MOD_SHIFT, true);
			setBit(m_mod_flags, MOD_NONE, false);
			break;
		case ALLEGRO_KEY_ALTGR:
		case ALLEGRO_KEY_ALT:
			setBit(m_mod_flags, MOD_ALT, true);
			setBit(m_mod_flags, MOD_NONE, false);
			break;
		case ALLEGRO_KEY_LCTRL:
		case ALLEGRO_KEY_RCTRL:
			setBit(m_mod_flags, MOD_CTRL, true);
			setBit(m_mod_flags, MOD_NONE, false);
			break;
		case ALLEGRO_KEY_BACKSPACE:
			m_backspace = true;
			m_backspace_wait = m_secs(0);
			m_timer.start();
			if (m_mod_flags & MOD_CTRL) deleteWord(m_input_string);
			else deleteCharacter(m_input_string);
			break;
		default:
			break;
		};

		/*
		Handle Text Input
		Not completely done, some characters missing, and some issues still present.
		*/

		handleTextInput(ev);
	}

	// Clear Modifiers Released
	else if (ev.type == ALLEGRO_EVENT_KEY_UP)
	{
		switch (ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_LSHIFT:
		case ALLEGRO_KEY_RSHIFT:
			setBit(m_mod_flags, MOD_SHIFT, false);
			break;
		case ALLEGRO_KEY_ALTGR:
		case ALLEGRO_KEY_ALT:
			setBit(m_mod_flags, MOD_ALT, false);
			break;
		case ALLEGRO_KEY_LCTRL:
		case ALLEGRO_KEY_RCTRL:
			setBit(m_mod_flags, MOD_CTRL, false);
			break;
		case ALLEGRO_KEY_BACKSPACE:
			m_backspace = false;
			break;
		default:
			break;
		};
		if (!m_mod_flags) m_mod_flags = MOD_NONE;
	}

	if (m_backspace && m_text_input_enabled)
	{
		m_secs ms = m_timer.elapsed();

		if (m_input_string.size() > 0 && ms > BACKSPACE_WAIT_TIME && ms > m_backspace_wait)
		{
			if (m_mod_flags & MOD_CTRL) // If Control is pressed, delete characters until ' ' is reached.
			{
				m_backspace_wait = ms + BACKSPACE_WORD_DELETE_TIME; // increase wait time so word deletion isnt too fast
				while (m_input_string.size() > 0)
				{
					m_input_string.pop_back();
					if (m_input_string.empty()) return;
					else if (m_input_string.back() == ' ') return;
				}
			}
			else // Delete by character, or if deleting by words, delete last space.
			{
				m_input_string.pop_back();
				m_backspace_wait = ms + BACKSPACE_CHAR_DELETE_TIME;
			}
		}
	}
}

void InputHandler::handleTextInput(const ALLEGRO_EVENT &ev)
{
	if (m_text_input_enabled && m_input_string.size() < m_max_input_length)
	{
		bool shift = (m_mod_flags & MOD_SHIFT) && m_allow_caps; // get if shift pressed
		unsigned char c = ev.keyboard.keycode;

		if (c <= ALLEGRO_KEY_Z)
		{
			c = letters[c];

			if (shift) c = toupper(c); // Keep an eye on this, formerly (char(shift) & m_input_flags)

			stringPushBack(c);
		}
		else if (c <= ALLEGRO_KEY_PAD_9)
		{
			if (c <= ALLEGRO_KEY_9 && shift && m_allow_specials)
			{
				c += 10; // if shift, increase number row to special characters
				stringPushBack(letters[c]);
			}
			else if (c >= ALLEGRO_KEY_PAD_0 && m_allow_digits)
			{
				c -= 10; // If key pad is used, shrink to number row
				stringPushBack(letters[c]);
			}
			else if (m_allow_digits)
			{
				stringPushBack(letters[c]);
			}
		}
		else
		{
			switch (c)
			{
			case ALLEGRO_KEY_FULLSTOP:
				if (m_allow_specials) stringPushBack('.');
				break;
			case ALLEGRO_KEY_SPACE:
				if (m_allow_space) stringPushBack(' ');
				break;
			case ALLEGRO_KEY_ENTER:
				if (m_allow_newline) stringPushBack('\n');
				break;
			case ALLEGRO_KEY_COMMA:
				if (m_allow_specials) stringPushBack(',');
				break;
			default:
				break;
			}
		}
	}
}

void InputHandler::enableTextInput(unsigned max_length, bool allow_caps, bool allow_newline,
	bool allow_digits, bool allow_specials, bool allow_space)
{
	m_max_input_length = max_length;
	m_allow_caps = allow_caps;
	m_allow_newline = allow_newline;
	m_allow_digits = allow_digits;
	m_allow_specials = allow_specials;
	m_allow_space = allow_space;

	m_text_input_enabled = true;

	clearInputString();
}

void InputHandler::disableTextInput()
{
	m_text_input_enabled = false;
}

std::string InputHandler::getTextInput()
{
	return m_input_string;
}

void InputHandler::setInputString(const std::string &str)
{
	if (str.size() <= m_max_input_length)
	{
		m_input_string = str;
	}
	else
	{
		m_input_string = str.substr(0, m_max_input_length);
	}
}

void InputHandler::clearInputString(void)
{
	m_input_string = "";
}

bool InputHandler::isKeyPressed(const int key, const char &mod) const
{
	if (al_key_down(&m_cur_key_state, key) && !al_key_down(&m_prev_key_state, key))
	{
		if (mod == MOD_IGNORE || mod == m_mod_flags) return true;
	}

	return false;
}
bool InputHandler::isKeyReleased(const int key, const char &mod) const
{
	if (al_key_down(&m_prev_key_state, key) && !al_key_down(&m_cur_key_state, key))
	{
		if (mod == MOD_IGNORE || mod == m_mod_flags) return true;
	}

	return false;
}
bool InputHandler::isKeyDown(const int key, const char &mod) const
{
	if (al_key_down(&m_cur_key_state, key))
	{
		if (mod == MOD_IGNORE || mod == m_mod_flags) return true;
	}

	return false;
}

bool InputHandler::isMousePressed(const int button, const char &mod) const
{
	if (al_mouse_button_down(&m_cur_mouse_state, button) && !al_mouse_button_down(&m_prev_mouse_state, button))
	{
		if (mod == MOD_IGNORE || mod == m_mod_flags) return true;
	}

	return false;
}
bool InputHandler::isMouseReleased(const int button, const char &mod) const
{
	if (al_mouse_button_down(&m_prev_mouse_state, button) && !al_mouse_button_down(&m_cur_mouse_state, button))
	{
		if (mod == MOD_IGNORE || mod == m_mod_flags) return true;
	}

	return false;
}
bool InputHandler::isMouseDown(const int button, const char &mod) const
{
	if (al_mouse_button_down(&m_cur_mouse_state, button))
	{
		if (mod == MOD_IGNORE || mod == m_mod_flags) return true;
	}

	return false;
}

bool InputHandler::isMouseWheelDown(const char &mod) const
{
	if (m_cur_mouse_state.z < m_prev_mouse_state.z && (mod == MOD_IGNORE || mod == m_mod_flags))
	{
		return true;
	}
	else return false;
}
bool InputHandler::isMouseWheelUp(const char &mod) const
{
	if (m_cur_mouse_state.z > m_prev_mouse_state.z && (mod == MOD_IGNORE || mod == m_mod_flags))
	{
		return true;
	}
	else return false;
}

int InputHandler::getMouseX(void) const
{
	return m_cur_mouse_state.x;
}
int InputHandler::getMouseY(void) const
{
	return m_cur_mouse_state.y;
}
bool InputHandler::isMouseInWindow(void) const
{
	return (m_cur_mouse_state.display == al_get_current_display());
}