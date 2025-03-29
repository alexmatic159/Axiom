#include "application.h"

namespace AXIOM {
    Application::Application()
    {
        
    }

    Application::~Application() {
        Shutdown();
        CloseWindow();
    }

    bool Application::Create(const std::string& title, int width, int height)
    {
        // Initialize Logger
        m_Logger = std::make_unique<CORE::Logger>();
        m_Logger->Init();

        // Initialize Window
        m_Window = std::make_unique<GRAPHICS::Window>();
        return m_Window->Create(title, width, height);
    }

    void Application::Run()
    {
        float lastFrame = 0.0f;
        while(!m_Window->ShouldClose()) 
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            Update(deltaTime);

            m_Window->SwapBuffers();
            m_Window->PollEvents();
        }
    }

    void Application::CloseWindow()
    {
        m_Window->Shutdown();
    }
}