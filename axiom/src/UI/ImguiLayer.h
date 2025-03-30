#pragma once
#include "ImguiContext.h"

namespace AXIOM {

	class ImGuiLayer {
    public:
        void OnAttach(GLFWwindow* window) {
            ImGuiContext::Init(window);
        }

        void OnDetach() {
            ImGuiContext::Shutdown();
        }

        void Begin() {
            ImGuiContext::BeginFrame();
        }

        void End() {
            ImGuiContext::EndFrame();
        }

        // Esempio di UI
        void DemoWindow() {
            ImGui::ShowDemoWindow();
        }

        void StyleWindow() {
            ImGui::ShowStyleEditor();
        }
    };

}