#pragma once
#include "event.h"

namespace AXIOM {

	enum KeyCode {
		KEY_ESCAPE = 256 // Codice glfw per ESC

	};

	class KeyPressEvent : public Event {
	public:
		KeyCode keyCode;

		KeyPressEvent(int glfwKeyCode) : keyCode(static_cast<KeyCode>(glfwKeyCode)) {}

		EventType type() const override {
			return EventType::KeyPressed;
		}
	};
}