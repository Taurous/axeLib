#pragma once
#include "axeEngine\GUI\GUIObject.h"
#include "axeEngine\InputHandler.h"
#include "axeEngine\EventHandler.h"

#include <functional>

namespace axe
{
	class GUIInteractable : public GUIObject
	{
	protected:
		enum STATE
		{
			NORMAL,
			PRESSED,
			DISABLED
		};

		int state;
		bool m_hovered;

		int keybind;

	public:
		GUIInteractable(DrawEngine *draw, GUIObject *parent, int x, int y, int width, int height, int anchor = axe::ANCHOR_TOPLEFT);
		virtual ~GUIInteractable() { }

		virtual void draw() = 0;
		virtual void handleEvents(const InputHandler &input, EventHandler &events) = 0;

		bool isEnabled(void) { return state == DISABLED ? false : true; }
		virtual void setEnabled(bool enable) { if (!enable) state = DISABLED; else state = NORMAL; }

		void setKeybind(int key) { keybind = key; }
	};
}