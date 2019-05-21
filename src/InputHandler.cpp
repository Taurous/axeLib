#include "axeLib/InputHandler.h"

#include "axeLib/util/Util.h"
#include "axeLib/util/Logger.h"

using namespace axe;

InputHandler::InputHandler() : m_mod_flags(0)
{
	allegro_init();
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

	axe::log(LOGGER_MESSAGE, "InputHandler destroyed\n");
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
		default:
			break;
		};
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
		default:
			break;
		};

		if (!m_mod_flags) m_mod_flags = MOD_NONE;
	}
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