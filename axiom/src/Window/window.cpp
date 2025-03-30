#include "window.h"

#include "Logger/Logger.h"
#include "Event/Eventbus.h"
#include "Event/WindowEvent.h"
#include "Event/KeyEvent.h"

namespace AXIOM {

    Window::Window(const std::string& title, int width, int height)
        : m_Window(nullptr), m_Title(title), m_Width(width), m_Height(height)
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
        glfwSetWindowSizeCallback(m_Window, glfw_window_size_callback);
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

    void Window::SetWidth(int width)
    {
        m_Width = width;
    }

    void Window::SetHeight(int height)
    {
        m_Height = height;
    }

    void Window::glfw_window_close_callback(GLFWwindow* window)
    {
        WindowCloseEvent event;
        EventBus::Get()->Publish(event);
    }

    void Window::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS) {
            KeyPressEvent event(key);
            EventBus::Get()->Publish(event);
        }
    }

    void Window::glfw_window_size_callback(GLFWwindow* window, int width, int height)
    {
        WindowResizeEvent event(width, height);
        EventBus::Get()->Publish(event);
    }

}