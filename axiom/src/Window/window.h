#pragma once
#include "Event/event.h"

#include <GLFW/glfw3.h>
#include <string>

namespace AXIOM::GRAPHICS {

	class Window
	{
	public:
		Window(const std::string& title, int width, int height);
		~Window();

		bool Create();

		// Metodi base
		bool ShouldClose() const;
		void PollEvents();
		void SwapBuffers();
		void Shutdown();

		// Getters
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		GLFWwindow* GetNativeWindow() const { return m_Window; }

		// Event
		void set_event_callback(const EVENT::EventCallback& callback);
	private:
		static void glfw_window_close_callback(GLFWwindow* window);
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	private:
		GLFWwindow* m_Window;
		std::string m_Title;
		int m_Width;
		int m_Height;
		
		EVENT::EventCallback m_callback;
	};
}