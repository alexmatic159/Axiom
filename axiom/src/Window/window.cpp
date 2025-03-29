#include "window.h"

#include "Logger/logger.h"
#include "Event/window_event.h"
#include "Event/key_event.h"

namespace AXIOM::GRAPHICS {

    Window::Window(const std::string& title, int width, int height)
        : m_Window(nullptr), m_Title(title), m_Width(width), m_Height(height), m_callback([](EVENT::Event&) {})
    {

    }

    Window::~Window() {
        Shutdown();
    }

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
        
        glfwSetWindowUserPointer(m_Window, this);
        glfwSetWindowCloseCallback(m_Window, glfw_window_close_callback);
        glfwSetKeyCallback(m_Window, glfw_key_callback);
        return true;
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

    void Window::set_event_callback(const EVENT::EventCallback& callback)
    {
        m_callback = callback;
    }

    void Window::glfw_window_close_callback(GLFWwindow* window)
    {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        EVENT::WindowCloseEvent event;
        win->m_callback(event);
    }

    void Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS) {
            EVENT::KeyPressEvent e(static_cast<EVENT::KeyCode>(key));
            win->m_callback(e);
        }
    }

}