#pragma once
#include "Util\Logger.h"

#include <allegro5/allegro.h>
#include <stdio.h>

namespace axe
{
	enum
	{
		GUI_EVENT_BUTTON_PRESSED = 1025,
		GUI_EVENT_BUTTON_RELEASED = 1026
	};

	class EventHandler
	{
	public:
		EventHandler(int engine_speed);

		EventHandler(const EventHandler &other) = delete;
		EventHandler &operator=(const EventHandler &other) = delete;

		EventHandler(EventHandler &&other) = delete;
		EventHandler &operator=(EventHandler && other) = delete;
		
		~EventHandler(void);

		bool handleEvents(void);

		bool eventIs(int type) const;
		bool eventQueueEmpty(void) const;
		void emitEvent(int type, int field1 = 0, int field2 = 0, int field3 = 0, int field4 = 0);
		const ALLEGRO_EVENT &getEvent(void) const;
		ALLEGRO_EVENT_QUEUE *getEventQueue(void) { return event_queue; }
		void startTimer(void);

		bool getFocus(void) const;

	private:
		bool focus;

		ALLEGRO_EVENT ev;
		ALLEGRO_EVENT_SOURCE user_event_source;
		ALLEGRO_EVENT_QUEUE *event_queue;
		ALLEGRO_TIMER *timer;
	};
};