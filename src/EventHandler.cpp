#include "axeLib\Eventhandler.h"
#include "axeLib\util\Util.h"

using namespace axe;

EventHandler::EventHandler(int engine_speed) : focus(true), event_queue(nullptr), timer(nullptr)
{
	axe::allegro_init();

	timer = al_create_timer(1.0f / float(engine_speed));

	event_queue = al_create_event_queue();

	if (al_is_keyboard_installed()) al_register_event_source(event_queue, al_get_keyboard_event_source());
	if (al_is_mouse_installed()) al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_init_user_event_source(&user_event_source);
	al_register_event_source(event_queue, &user_event_source);

	axe::log(LOGGER_MESSAGE, "EventHandler Initialized\n");
}

EventHandler::~EventHandler(void)
{
	al_unregister_event_source(event_queue, &user_event_source);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
}

bool EventHandler::handleEvents()
{
	ALLEGRO_EVENT ev;

	if (!al_get_next_event(event_queue, &ev)) return false;

	this->ev = ev;

	return true;
}

const ALLEGRO_EVENT &EventHandler::getEvent(void) const
{
	return ev;
}

bool EventHandler::getFocus(void) const
{
	return focus;
}

void EventHandler::startTimer()
{
	al_start_timer(timer);
}

bool EventHandler::eventIs(int type) const
{
	if (ev.type == type)
		return true;
	else return false;
}

bool EventHandler::eventQueueEmpty() const
{
	return al_is_event_queue_empty(event_queue);
}

void EventHandler::emitEvent(int type, int field1, int field2, int field3, int field4)
{
	ALLEGRO_EVENT user_event;
	user_event.user.type = type;
	user_event.user.data1 = field1;
	user_event.user.data2 = field2;
	user_event.user.data3 = field3;
	user_event.user.data4 = field4;

	al_emit_user_event(&user_event_source, &user_event, NULL);
}