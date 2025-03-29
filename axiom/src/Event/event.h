#pragma once
#include <functional>

namespace AXIOM { 

	enum class EventType {
		WindowClose, WindowResize,
		KeyPressed, KeyReleased,
		MouseMoved, MouseClicked
	};

	class Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType type() const = 0;
		bool handled = false;
	};

}