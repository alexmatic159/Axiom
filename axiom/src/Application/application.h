#pragma once
#include "Window/window.h"
#include "Logger/logger.h"
#include "Event/eventbus.h"

namespace AXIOM {

	class Application {
	public:
        Application();
        virtual ~Application();

        // Gestione finestra
        bool Create(const std::string& title, int width, int height);

        // Metodi virtuali utente
        virtual void Initialize() {}
        virtual void Update(float deltaTime) {}
        
        // Loop principale
        void Run();

        void Shutdown();
        void SetWindowEventsCallback();
	private:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Logger> m_Logger;
        bool m_Running;
	};
}