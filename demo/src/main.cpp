#include "Axiom.h"

#include <iostream>
#include <imgui.h>
#include <imgui_internal.h>

class DemoApp : public AXIOM::Application 
{
private:
	unsigned int clicked = 0;
    ImGuiWindowClass m_WindowClass;
public:
	void Initialize() override {
		AX_INFO("Initializing demo");

		if (!Create("Axiom Demo", 1600, 900))
			AX_CRITICAL("Application creation failed");

        m_WindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	}

	void Update(float deltaTime) override {
        
        MenuBar();

        ImGui::SetNextWindowClass(&m_WindowClass);
        ImGui::Begin("Root", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        {
            SideArea(); 
            ImGui::SameLine();
            CenterArea();
        }
        ImGui::End();
	}

	void MenuBar() 
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Nuovo login"))
                    AX_TRACE("Nuovo Login");
                if (ImGui::MenuItem("Importa File"))
                    AX_TRACE("Importa file");
                if (ImGui::MenuItem("Esporta File"))
                    AX_TRACE("Esporta file");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Account"))
            {
                if (ImGui::MenuItem("Cambia root password"))
                    AX_TRACE("Cambia root password");
                if (ImGui::MenuItem("2 FA"))
                    AX_TRACE("2FA");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Aiuto"))
            {
        
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
	}
    
    void SideArea() 
    {
        ImVec2 window = GetWindowSize();
        ImGui::BeginChild("Sidebar", ImVec2(window.x * 0.3, ImGui::GetContentRegionAvail().y), true);
        {
            if (ImGui::CollapsingHeader("Filtri", ImGuiTreeNodeFlags_None))
            {
                ImGui::Text("Vault");
            }
            if (ImGui::CollapsingHeader("Generatore", ImGuiTreeNodeFlags_None))
            {
                ImGui::Text("Generatore");
            }
        }
        ImGui::EndChild();
    }
    
    void CenterArea()
    {
        ImGui::BeginChild("MainArea", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
        {
            ImGui::Text("Login");
            ImGui::Separator();

            ImGui::Text("Username");
            ImGui::SameLine();
            static char user[128] = "";
            ImGui::InputText("##1", user, IM_ARRAYSIZE(user));

            ImGui::Text("Password");
            ImGui::SameLine();
            static char pass[128] = "";
            ImGui::InputText("##2", pass, IM_ARRAYSIZE(pass));

            if (ImGui::Button("Bottom Button")) {}
        }
        ImGui::EndChild();
        
    }
};

int main()
{
	DemoApp app;
	app.Run();
}

