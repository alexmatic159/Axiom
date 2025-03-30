#pragma once
#include "Window/Window.h"
#include "Logger/Logger.h"
#include "Event/Eventbus.h"
#include "UI/ImguiLayer.h"

#include <imgui.h>

namespace AXIOM {

	class Application {
	public:
        Application();
        virtual ~Application();

        // Gestione finestra
        bool Create(const std::string& title, int width, int height);
        void ResizeWindow(int width, int height);
        ImVec2 GetWindowSize() { return ImVec2(m_Window->GetWidth(), m_Window->GetHeight()); }

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
        ImGuiLayer m_DemoLayer;
	};
}