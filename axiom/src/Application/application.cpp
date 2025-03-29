#include "application.h"
#include "Event/key_event.h"

namespace AXIOM {

    Application::Application() 
        : m_Running(true)
    {
        // Initialize Logger
        m_Logger = std::make_unique<Logger>();
        m_Logger->Init();
    }

    Application::~Application() {
        Shutdown();
    }

    bool Application::Create(const std::string& title, int width, int height)
    {
        // Initialize Window
        m_Window = std::make_unique<Window>(title, width, height);
        return m_Window->Create();

    }

    void Application::Run()
    {
        Initialize();
        SetWindowEventsCallback();

        float lastFrame = 0.0f;
        while(m_Running) 
        {
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            Update(deltaTime);

            m_Window->SwapBuffers();
            m_Window->PollEvents();
        }

    }

    void Application::Shutdown()
    {
        m_Window->Shutdown();
        m_Running = false;
    }

    void Application::SetWindowEventsCallback()
    {
        EventBus::Get()->Subscribe(EventType::KeyPressed, [this](const Event& e) {
            auto& ke = static_cast<const KeyPressEvent&>(e);
            if (ke.keyCode == GLFW_KEY_ESCAPE) {
                Shutdown();
            }
        });

        EventBus::Get()->Subscribe(EventType::WindowClose, [this](const Event& e) {
            Shutdown();
        });
    }
}