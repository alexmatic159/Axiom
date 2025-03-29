#pragma once
#include "Window/window.h"
#include "Logger/logger.h"

namespace AXIOM {

	class Application {
	public:
        Application() = default;
        virtual ~Application();

        // Gestione finestra
        bool Create(const std::string& title, int width, int height);

        // Metodi virtuali utente
        virtual void Initialize() {}
        virtual void Update(float deltaTime) {}
        virtual void Render() {}
        virtual void Shutdown() {}

        // Loop principale
        void Run();

        // Accessori
        void CloseWindow();
	protected:
		std::unique_ptr<GRAPHICS::Window> m_Window;
		std::unique_ptr<CORE::Logger> m_Logger;
	};
}