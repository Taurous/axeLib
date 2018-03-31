#pragma once

#include "GUI/GUIObject.h"
#include "EventHandler.h"

namespace axe
{
	enum
	{
		GUI_BUTTON_PRESSED = 1025,
		GUI_BUTTON_RELEASED = 1026
	};

	class GUIManager
	{
	public:
		GUIManager(axe::EventHandler *handler);
		~GUIManager();

		void registerGUIObject(GUIObject *obj);
		void unregisterGUIObject(GUIObject *obj);

		GUIObject *getUIParent(void) { return UIParent; }

		void hideUI(void);
		void showUI(void);

		void emitEvent(int type, int id);

		int getNextID(void) { return ++next_id; }

	private:
		GUIObject *UIParent;
		std::vector<GUIObject*> objects;

		ALLEGRO_EVENT_SOURCE gui_event_source;
		ALLEGRO_EVENT guiEvent;

		axe::EventHandler *event_handler;

		int next_id;
	};
}