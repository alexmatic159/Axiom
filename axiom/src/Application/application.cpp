#include "Application.h"
#include "Event/KeyEvent.h"
#include "Event/WindowEvent.h"

#include "Utils/Crypto/Crypto.h"

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

    void Application::ResizeWindow(int width, int height)
    {
        m_Window->SetWidth(width);
        m_Window->SetHeight(height);
        AX_WARN("New Window Size:{}x{}", width, height);

        // Aggiorna il viewport OpenGL
        glViewport(0, 0, width, height);

        // Aggiorna le dimensioni in ImGui
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)width, (float)height);
    }

    void Application::Run()
    {
        Initialize();
        SetWindowEventsCallback();

        m_DemoLayer.OnAttach(m_Window->GetNativeWindow());

        float lastFrame = 0.0f;
        while(m_Running) 
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_DemoLayer.Begin();
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            
            //m_DemoLayer.StyleWindow();
            //m_DemoLayer.DemoWindow();
            Update(deltaTime);

            m_DemoLayer.End();

            m_Window->SwapBuffers();
            m_Window->PollEvents();
        }

        m_DemoLayer.OnDetach();
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

        EventBus::Get()->Subscribe(EventType::WindowResize, [this](const Event& e) {
            auto& we = static_cast<const WindowResizeEvent&>(e);
            ResizeWindow(we.width, we.height);
        });
    }
}