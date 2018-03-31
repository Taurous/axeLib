#include "GUIManager.h"

namespace axe
{
	GUIManager::GUIManager(axe::EventHandler *handler) : event_handler(handler), next_id(0)
	{
		UIParent = new GUIObject(nullptr, 0, 0, Engine::getInstance().draw.getScreenWidth(), Engine::getInstance().draw.getScreenHeight());

		al_init_user_event_source(&gui_event_source);
		al_register_event_source(event_handler->getEventQueue(), &gui_event_source);
	}

	GUIManager::~GUIManager()
	{
		if (UIParent) delete UIParent;
	}

	void GUIManager::registerGUIObject(GUIObject * obj)
	{

	}
	void GUIManager::unregisterGUIObject(GUIObject *obj)
	{

	}

	void GUIManager::hideUI(void)
	{
		UIParent->setVisible(false);
	}
	void GUIManager::showUI(void)
	{
		UIParent->setVisible(true);
	}
	void GUIManager::emitEvent(int type, int id)
	{
		guiEvent.user.type = type;
		guiEvent.user.data1 = id;

		al_emit_user_event(&gui_event_source, &guiEvent, NULL);
	}
}