#include "ImguiContext.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"

#include "Utils/File/FileManager.h"

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

        std::string filePath = (AXIOM::FileManager::GetAppDataPath() / "Axiom/data/fonts/Roboto.ttf").string();

        ImFont* font = io.Fonts->AddFontFromFileTTF(
            filePath.c_str(),  // Percorso del file
            25.0f,                     // Dimensione in pixel
            nullptr,                   // Configurazione font opzionale
            io.Fonts->GetGlyphRangesDefault() // Range di caratteri
        );

        io.FontDefault = font;

        // Configura backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        ImGui_ImplOpenGL3_DestroyDeviceObjects(); 
        ImGui_ImplOpenGL3_CreateDeviceObjects();
        AX_INFO("Inizializzato ImGui");
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