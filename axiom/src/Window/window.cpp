#include "window.h"

#include "Logger/logger.h"

namespace AXIOM::GRAPHICS {

    bool Window::Create(const std::string& title, int width, int height) {
        //if (m_Window) return true; // Già creata

        if (!glfwInit()) {
            AX_CRITICAL("GLFW initialization failed");
        }

        AX_INFO("Creating window: {} ({}x{})", title, width, height);
        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_Window) {
            glfwTerminate();
            AX_CRITICAL("Failed to create window");
            return false;
        }

        m_Width = width;
        m_Height = height;
        glfwMakeContextCurrent(m_Window);
        return true;
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