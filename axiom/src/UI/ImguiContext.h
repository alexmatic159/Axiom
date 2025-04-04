#pragma once
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace AXIOM {

    class ImGuiContext {
    public:
        static void Init(GLFWwindow* window);

        static void Shutdown();

        static void BeginFrame();

        static void EndFrame();
    };
}