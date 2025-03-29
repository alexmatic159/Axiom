#pragma once
#include "event.h"

namespace AXIOM::EVENT {

	class WindowCloseEvent : public Event 
	{
	public:
		EventType type() const override {
			return EventType::WindowClose;
		}
	};
}
