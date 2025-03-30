#pragma once
#include "Event.h"

namespace AXIOM {

	class WindowCloseEvent : public Event 
	{
	public:
		EventType type() const override {
			return EventType::WindowClose;
		}
	};

	class WindowResizeEvent : public Event {
	public:
		int width, height;

		WindowResizeEvent(int w, int h) : width(w), height(h) {}

		EventType type() const override {
			return EventType::WindowResize;
		}
	};
}
