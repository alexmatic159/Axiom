#include "window.h"

#include "Logger/logger.h"

namespace AXIOM::GRAPHICS {

    bool Window::Create() {
        if (m_Window) return true; // Già creata

        if (!glfwInit()) {
            AX_CRITICAL("GLFW initialization failed");
            return false;
        }

        AX_INFO("Creating window: {} ({}x{})", m_Title, m_Width, m_Height);
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        if (!m_Window) {
            glfwTerminate();
            AX_CRITICAL("Failed to create window");
            return false;
        }

        glfwMakeContextCurrent(m_Window);
        return true;
    }

    Window::Window(const std::string& title, int width, int height)
        : m_Window(nullptr), m_Title(title), m_Width(width), m_Height(height)
    {
        
    }

    Window::~Window() {
        Shutdown();
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(m_Window);
    }

    void Window::Shutdown()
    {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
            glfwTerminate();
        }
    }

}