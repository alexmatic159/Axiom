#pragma once
#include <GLFW/glfw3.h>

#include <string>

namespace AXIOM::GRAPHICS {

	class Window
	{
	public:
		Window() = default;
		~Window();

		bool Create(const std::string& title, int width, int height);

		// Metodi base
		bool ShouldClose() const;
		void PollEvents();
		void SwapBuffers();
		void Shutdown();

		// Getters
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		GLFWwindow* GetNativeWindow() const { return m_Window; };

	private:
		GLFWwindow* m_Window;
		int m_Width;
		int m_Height;
	};
}