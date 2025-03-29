#pragma once
#include "event.h"

namespace AXIOM::EVENT {

	enum KeyCode {
		KEY_ESCAPE = 256 // Codice glfw per ESC

	};

	class KeyEvent : public Event {
	public: 
		KeyCode key;

		KeyEvent(KeyCode k) : key(k) {}
	};

	class KeyPressEvent : public KeyEvent {
	public:
		KeyPressEvent(KeyCode k) : KeyEvent(k) {}

		EventType type() const override {
			return EventType::KeyPressed;
		}
	};
}