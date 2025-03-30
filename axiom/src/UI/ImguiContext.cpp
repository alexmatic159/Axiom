#include "ImguiContext.h"
#include "GLFW/glfw3.h"

namespace AXIOM {

    void ImGuiContext::Init(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        
        // Stile dark di default
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // Sfondo
        colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // #1E1E2E

        // Widget
        colors[ImGuiCol_FrameBg] = ImVec4(0.23f, 0.23f, 0.30f, 1.00f);  // #3B3B4D
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.36f, 0.36f, 0.48f, 1.00f);

        // Testo
        colors[ImGuiCol_Text] = ImVec4(0.75f, 0.79f, 0.96f, 1.00f);     // #C0CAF5

        // Bottoni
        colors[ImGuiCol_Button] = ImVec4(0.48f, 0.63f, 0.97f, 1.00f);   // #7AA2F7
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.56f, 0.71f, 1.00f, 1.00f);

        // Header (per menu)
        colors[ImGuiCol_Header] = ImVec4(0.34f, 0.34f, 0.45f, 1.00f);

        ImFont* font = io.Fonts->AddFontFromFileTTF(
            "../include/fonts/Roboto.ttf",  // Percorso del file
            25.0f,                     // Dimensione in pixel
            nullptr,                   // Configurazione font opzionale
            io.Fonts->GetGlyphRangesDefault() // Range di caratteri
        );

        io.FontDefault = font;

        // Configura backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        ImGui_ImplOpenGL3_DestroyDeviceObjects(); // Sostituisci XXXX con il tuo backend
        ImGui_ImplOpenGL3_CreateDeviceObjects();
    }

    void ImGuiContext::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiContext::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport();
    }

    void ImGuiContext::EndFrame()
    {
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_context);
        }
    }

}