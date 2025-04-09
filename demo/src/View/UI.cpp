#include "UI.h"

UserInterface::UserInterface()
{
    m_WindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    m_Auth = std::make_unique<Authenticator>();
}

void UserInterface::View()
{
    MenuBarView();

    ImGui::SetNextWindowClass(&m_WindowClass);
    ImGui::Begin("Root", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    {
        // Check credentials
        if (m_RegisterCheck) {
            RegisterFormView();
        }
        else if (m_LoginCheck) {
            LoggedView();
        }
        else {
            LoginFormView();
        }
    }
    ImGui::End();
}

void UserInterface::LoggedView()
{
    SideView();
    ImGui::SameLine();
    CenterView();
}

void UserInterface::MenuBarView()
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
            if (m_LoginCheck) {
                if (ImGui::MenuItem("Logout")) {
                    AXIOM::AuthResult logoutResult = m_Auth->Logout();

                    switch (logoutResult)
                    {
                        case AXIOM::LOGOUT_OK :
                            m_RegisterCheck = false;
                            m_LoginCheck = false;
                    }
                }
            }
            ImGui::EndMenu();     
        }
        if (ImGui::BeginMenu("Aiuto"))
        {

            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();
}

void UserInterface::SideView()
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window = io.DisplaySize;
    ImGui::BeginChild("Sidebar", ImVec2(window.x * 0.3, ImGui::GetContentRegionAvail().y), true);
    {
        if (ImGui::CollapsingHeader("Filtri", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("Vault");
        }
        if (ImGui::CollapsingHeader("Generatore", ImGuiTreeNodeFlags_None))
        {
            if (ImGui::Button("Generatore##1"))
            {
                m_GeneratorVisibility = true;
            }
        }
    }
    ImGui::EndChild();
}

void UserInterface::CenterView()
{
    ImGui::BeginChild("MainArea", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
    {
        if (m_GeneratorVisibility) {
            static std::string passwordGenerata;
            static int number_char;
            static bool use_lower = true;
            static bool use_upper = true;
            static bool use_number = true;
            static bool use_symbol = true;
            static ImVec4 bg_color = ImVec4(0.15f, 0.15f, 0.15f, 1.00f); // Colore di sfondo persona

            ImGui::Text("Generatore di Password");
            ImGui::Spacing();

            ImGui::Text("Password generata:");
            ImGui::PushStyleColor(ImGuiCol_FrameBg, bg_color);
            ImGui::InputText("##PasswordField", &passwordGenerata, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::PopStyleColor();

            ImGui::InputInt("Numero di caratteri", &number_char);
            ImGui::Checkbox("Caratteri minuscoli", &use_lower);
            ImGui::Checkbox("Caratteri maiuscoli", &use_upper);
            ImGui::Checkbox("Numeri", &use_number);
            ImGui::Checkbox("Simboli", &use_symbol);
            // settings
            if (ImGui::Button("Genera")) {
                PasswordGenerator pg;
                passwordGenerata = pg.GeneratePassword(number_char, use_lower, use_upper, use_number, use_symbol);
            }

        }
    }
    ImGui::EndChild();

}

void UserInterface::LoginFormView()
{
    // Ottieni la dimensione della finestra principale
    ImGuiIO& io = ImGui::GetIO();
    // Dimensioni della finestra di login
    ImVec2 window_size(io.DisplaySize.x * 0.6f, io.DisplaySize.y * 0.6f);

    // Calcola la posizione per centrarla
    ImVec2 center_pos = ImVec2((io.DisplaySize.x - window_size.x) * 0.5f,
        (io.DisplaySize.y - window_size.y) * 0.5f);

    static std::string username;
    static std::string password;

    ImGui::SetNextWindowClass(&m_WindowClass);
    ImGui::SetWindowPos(center_pos);
    ImGui::BeginChild("LoginForm", window_size, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    {
        ImGui::SeparatorText("Benvenuto!");

        ImGui::BeginGroup();
        {
            ImGui::Text("Username:");
            ImGui::PushItemWidth(window_size.x * 0.9f);
            ImGui::InputText("##username", &username);
            ImGui::Spacing();
            ImGui::Text("Password:");
            ImGui::InputText("##password", &password, ImGuiInputTextFlags_Password);
            ImGui::PopItemWidth();

            ImVec2 button_size = ImVec2(window_size.x * 0.9f, window_size.y * 0.2f);
            if (ImGui::Button("Login", button_size))
            {
                if ((username.length() != 0) && (password.length() != 0)) {
                    AXIOM::AuthResult loginResult = m_Auth->Login(username, password);

                    switch (loginResult)
                    {
                        case AXIOM::AuthResult::LOGIN_OK :
                        {
                            AX_INFO("Benvenuto!, {}", username);
                            m_RegisterCheck = false;
                            m_LoginCheck = true;
                            break;
                        }
                        case AXIOM::AuthResult::USER_NOT_EXIST :
                        {
                            AX_ERROR("L'utente non esiste");
                            break;
                        }
                        case AXIOM::AuthResult::PSWD_DIFFERENT:
                        {
                            AX_ERROR("Password sbagliata");
                            break;
                        }
                    }

                }
                else {
                    AX_INFO("Inserire tutti i campi");
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

void UserInterface::RegisterFormView()
{
    // Ottieni la dimensione della finestra principale
    ImGuiIO& io = ImGui::GetIO();
    // Dimensioni della finestra di login
    ImVec2 window_size(io.DisplaySize.x * 0.6f, io.DisplaySize.y * 0.6f);

    // Calcola la posizione per centrarla
    ImVec2 center_pos = ImVec2((io.DisplaySize.x - window_size.x) * 0.5f,
        (io.DisplaySize.y - window_size.y) * 0.5f);

    static std::string username;
    static std::string password;
    static std::string passwordCheck;

    ImGui::SetNextWindowClass(&m_WindowClass);
    ImGui::SetWindowPos(center_pos);
    ImGui::BeginChild("RegisterForm", window_size, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    {
        ImGui::SeparatorText("Registrati ora!");

        ImGui::BeginGroup();
        {
            ImGui::PushItemWidth(window_size.x * 0.9f);

            ImGui::Text("Username:");
            ImGui::InputText("##username", &username);

            ImGui::Spacing();
            ImGui::Text("Password:");
            ImGui::InputText("##password", &password, ImGuiInputTextFlags_Password);

            ImGui::Text("Conferma Password:");
            ImGui::InputText("##passwordcheck", &passwordCheck, ImGuiInputTextFlags_Password);

            ImGui::PopItemWidth();

            ImVec2 button_size = ImVec2(window_size.x * 0.9f, window_size.y * 0.2f);
            if (ImGui::Button("Registrati", button_size))
            {
                if ((username.length() != 0) && (password.length() != 0) && (passwordCheck.length() != 0)) {
                    AXIOM::AuthResult registerResult = m_Auth->Register(username, password, passwordCheck);

                    switch (registerResult)
                    {
                        case AXIOM::AuthResult::REGISTER_OK : 
                        {
                            AX_INFO("Utente {} registrato!", username);
                            m_RegisterCheck = false;
                            m_LoginCheck = true;
                            break;
                        }   
                        case AXIOM::AuthResult::USER_ALREADY_EXIST : 
                        {
                            AX_ERROR("L'utente esiste gia'");
                            break;
                        }   
                        case AXIOM::AuthResult::PSWD_DIFFERENT : 
                        {
                            AX_ERROR("Le password inserite non coincidono");
                            break;
                        }  
                    }

                }
                else {
                    AX_INFO("Inserire tutti i campi");
                }
            }
            if (ImGui::Button("Indietro")) {
                m_RegisterCheck = false;
            }
        }
        ImGui::EndGroup();

    }
    ImGui::EndChild();
}