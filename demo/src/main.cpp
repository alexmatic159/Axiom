#include "Axiom.h"

#include <iostream>
#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>

class DemoApp : public AXIOM::Application 
{
private:
	unsigned int clicked = 0;
    bool m_LoginCheck = false;
    bool m_RegisterCheck = false;
    ImGuiWindowClass m_WindowClass;
public:
	void Initialize() override {
		AX_INFO("Initializing demo");

		if (!Create("Axiom Demo", 900, 650))
			AX_CRITICAL("Application creation failed");

        m_WindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
         
        AXIOM::Crypto crypto("/Axiom/data/credential.json", "password");
        crypto.CreateRecord("www.google.com", "alexmatic", "milan123");

        /*
        std::string path = AXIOM::FilePath::GetAppDataPath() + "/Axiom/data/credential.json";
        AXIOM::json contenuto = AXIOM::Json::Read(path);
        AX_TRACE("Content of file credential.json : {} al percorso {}", (contenuto["username"]).dump(), path);
        */

	}

	void Update(float deltaTime) override {
       
        MenuBar();

        ImGui::SetNextWindowClass(&m_WindowClass);
        ImGui::Begin("Root", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        {
            // Check credentials
            if (m_RegisterCheck) {
                RegisterForm();
            }
            else if (m_LoginCheck) {
                SideArea();
                ImGui::SameLine();
                CenterArea();
            }
            else { 
                LoginForm(); 
            }
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
            
        }
        ImGui::EndChild();
        
    }

    void LoginForm()
    {
        // Ottieni la dimensione della finestra principale
        ImGuiIO& io = ImGui::GetIO();
        // Dimensioni della finestra di login
        ImVec2 window_size(io.DisplaySize.x * 0.6f, io.DisplaySize.y * 0.6f);

        // Calcola la posizione per centrarla
        ImVec2 center_pos = ImVec2((io.DisplaySize.x - window_size.x) * 0.5f,
            (io.DisplaySize.y - window_size.y) * 0.5f);

        static char username[64] = "";
        static char password[64] = "";

        ImGui::SetNextWindowClass(&m_WindowClass);
        ImGui::SetWindowPos(center_pos);
        ImGui::BeginChild("LoginForm", window_size, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        {
            ImGui::SeparatorText("Benvenuto!");

            ImGui::BeginGroup();
            {
                ImGui::Text("Username:");
                ImGui::PushItemWidth(window_size.x * 0.9f);
                ImGui::InputText("##username", username, IM_ARRAYSIZE(username));
                ImGui::Spacing();
                ImGui::Text("Password:");
                ImGui::InputText("##password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
                ImGui::PopItemWidth();

                ImVec2 button_size = ImVec2(window_size.x * 0.9f, window_size.y * 0.2f);
                if (ImGui::Button("Login", button_size))
                {
                    AXIOM::AuthResult result = AXIOM::Authenticator::Login((std::string)username, (std::string)password);

                    switch (result)
                    {
                    case AXIOM::LOGIN_OK: m_LoginCheck = true;
                        break;
                    case AXIOM::USER_NOT_EXIST: AX_ERROR("User not exist");
                        break;
                    }
                }

                ImGui::Text("Non hai un account?");
                ImGui::SameLine();
                if (ImGui::SmallButton("Registrati ora"))
                {
                    m_RegisterCheck = true;
                }
            }
            ImGui::EndGroup();

        }
        ImGui::EndChild();

    }

    void RegisterForm()
    {
        // Ottieni la dimensione della finestra principale
        ImGuiIO& io = ImGui::GetIO();
        // Dimensioni della finestra di login
        ImVec2 window_size(io.DisplaySize.x * 0.6f, io.DisplaySize.y * 0.6f);

        // Calcola la posizione per centrarla
        ImVec2 center_pos = ImVec2((io.DisplaySize.x - window_size.x) * 0.5f,
            (io.DisplaySize.y - window_size.y) * 0.5f);

        static char username[64] = "";
        static char password[64] = "";
        static char passwordcheck[64] = "";

        ImGui::SetNextWindowClass(&m_WindowClass);
        ImGui::SetWindowPos(center_pos);
        ImGui::BeginChild("RegisterForm", window_size, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        {
            ImGui::SeparatorText("Registrati ora!");

            ImGui::BeginGroup();
            {
                ImGui::Text("Username:");
                ImGui::PushItemWidth(window_size.x * 0.9f);
                ImGui::InputText("##username", username, IM_ARRAYSIZE(username));
                ImGui::Spacing();
                ImGui::Text("Password:");
                ImGui::InputText("##password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
                ImGui::Text("Conferma Password:");
                ImGui::InputText("##passwordcheck", passwordcheck, IM_ARRAYSIZE(passwordcheck), ImGuiInputTextFlags_Password);
                ImGui::PopItemWidth();

                ImVec2 button_size = ImVec2(window_size.x * 0.9f, window_size.y * 0.2f);
                if (ImGui::Button("Registrati", button_size))
                {
                    if ((std::string(password).length() != 0) && (std::string(passwordcheck).length() != 0)) {
                        AX_INFO("PUOI REGISTRARTI!");

                        AXIOM::UserLevel level = AXIOM::UserLevel::Admin;
                        AXIOM::AuthResult result = AXIOM::Authenticator::Register(
                            (std::string)username, 
                            (std::string)password, 
                            level
                        );

                        switch (result)
                        {
                        case AXIOM::REGISTER_OK: m_LoginCheck = true;
                            break;
                        case AXIOM::USER_ALREADY_EXIST: AX_ERROR("User already exist");
                            break;
                        }
                    }
                    else {
                        AX_INFO("NON PUOI REGISTRARTI!");
                    }
                }
            }
            ImGui::EndGroup();

        }
        ImGui::EndChild();
    }
};

int main()
{
	DemoApp app;
	app.Run();
}

