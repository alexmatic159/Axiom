#include "UI.h"

UserInterface::UserInterface()
{
    m_WindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    m_Auth = std::make_unique<Authenticator>();
}

UserInterface::~UserInterface()
{
}

void UserInterface::MainView()
{
    ImGui::SetNextWindowClass(&m_WindowClass);
    ImGui::Begin("Root", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    {
        if (m_LoginCheck) 
        {
            LoggedView();
        }
        else if (m_RegisterCheck) {
            RegisterFormView();
        } else {
            LoginFormView();
        }

    }
    ImGui::End();
}

void UserInterface::LoggedView()
{
    MenuBarView();
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
            
            if (ImGui::MenuItem("Importa File")) 
            {
                m_ImportedFilePath = std::filesystem::path(AXIOM::FileManager::OpenFileExplorer());
                if (!m_ImportedFilePath.empty()) {
                    m_ImportVisibility = true;
                }
                    
            }

            if (ImGui::MenuItem("Esporta File")) 
            {
                AX_TRACE("Esporta file");
            }
            ImGui::EndMenu();
        }

        if (m_LoginCheck) 
        {
            if (ImGui::BeginMenu("Account"))
            {
                if (ImGui::MenuItem("Logout")) 
                {
                    AXIOM::AuthResult logoutResult = m_Auth->Logout();

                    switch (logoutResult)
                    {
                        case AXIOM::LOGOUT_OK :
                            m_RegisterCheck = false;
                            m_LoginCheck = false;
                    }
                }
                ImGui::EndMenu();     
            }
        }
        
        if (ImGui::BeginMenu("Aiuto"))
        {

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void UserInterface::SideView()
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window = io.DisplaySize;
    ImGui::BeginChild("Sidebar", ImVec2(window.x * 0.2, ImGui::GetContentRegionAvail().y), true);
    {
        if (ImGui::CollapsingHeader("Filtri", ImGuiTreeNodeFlags_None))
        {
            if (ImGui::Button("Vault"))
            {
                m_VaultVisibility = true;
                m_GeneratorVisibility = false;
                m_ImportVisibility = false;
            }

            if (m_ImportVisibility) 
            {
                static std::string buttonLabel = m_ImportedFilePath.filename().string();
                if (ImGui::Button(buttonLabel.c_str())) {
                    m_ImportVisibility = true;
                    m_VaultVisibility = false;
                    m_GeneratorVisibility = false;
                }
            }
            
        }

        if (ImGui::CollapsingHeader("Generatore", ImGuiTreeNodeFlags_None))
        {
            if (ImGui::Button("Generatore##1"))
            {
                m_GeneratorVisibility = true;
                m_VaultVisibility = false;
                m_ImportVisibility = false;
            }
        }
        
    }
    ImGui::EndChild();
}

void UserInterface::CenterView()
{
    ImGui::BeginChild("MainArea", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
    {
        if (m_GeneratorVisibility)
            PasswordGeneratorView();
        else if (m_VaultVisibility)
            VaultView();
        else if (m_ImportVisibility)
            ImportedFilesTable(AXIOM::FileManager::CSVtoJSON(m_ImportedFilePath));
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

void UserInterface::PasswordGeneratorView()
{
    static std::string passwordGenerata;
    static int number_char = 12;
    static bool use_lower = true;
    static bool use_upper = true;
    static bool use_number = true;
    static bool use_symbol = true;
    static ImVec4 bg_color = ImVec4(0.15f, 0.15f, 0.15f, 1.00f); // Colore di sfondo persona

    ImGui::Text("Generatore di Password");
    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, bg_color);
    ImGui::InputText("##PasswordField", &passwordGenerata, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
    ImGui::PopStyleColor();
    ImGui::SameLine();
    if (ImGui::Button("Copia"))
        ImGui::SetClipboardText(passwordGenerata.c_str());

    ImGui::InputInt("Numero di caratteri", &number_char);
    number_char = std::clamp(number_char, 12, 32);
    ImGui::Checkbox("Caratteri minuscoli", &use_lower);
    ImGui::Checkbox("Caratteri maiuscoli", &use_upper);
    ImGui::Checkbox("Numeri", &use_number);
    ImGui::Checkbox("Simboli", &use_symbol);

    if (ImGui::Button("Genera")) {
        PasswordGenerator pg;
        passwordGenerata = pg.GeneratePassword(number_char, use_lower, use_upper, use_number, use_symbol);
    }
}

void UserInterface::VaultView() 
{
    static bool showEditPopup = false;
    static bool showNewPopup = false;
    static bool deleteCredential = false;
    static int idSelected = -1;
    static Credential tempCredential;
    
    // Lista credenziali in una tabella
    CredentialTable(showEditPopup, showNewPopup, deleteCredential, idSelected);

    // Gestione popup modifica
    if (showEditPopup) {
        static bool editInitialized = false;

        if (!editInitialized) {
            User u = m_Auth->GetLoggedUser();
            auto credenziali = UserDatabase::GetInstance()->GetCredential(u.GetUsername());
            tempCredential = credenziali[idSelected];
            editInitialized = true;
        }

        if (CredentialPopup("Modifica Credenziale", showEditPopup, tempCredential)) {
            AX_INFO("Credenziale modificata con successo!");
            editInitialized = false; // Reset per la prossima apertura
        }

        if (!showEditPopup) {
            editInitialized = false; // Reset anche in caso di annullamento
        }
    }

    // Gestione popup nuova credenziale
    if (showNewPopup) {
        static bool newInitialized = false;

        if (!newInitialized) {
            tempCredential = Credential();
            newInitialized = true;
        }

        if (CredentialPopup("Nuova Credenziale", showNewPopup, tempCredential)) {
            AX_INFO("Credenziale aggiunta con successo!");
        }

        if (!showNewPopup) {
            newInitialized = false; // Reset anche in caso di annullamento
        }
    }

    // Gestione eliminazione
    if (deleteCredential) {
    static bool deleteInitialized = false;

        if (!deleteInitialized) {
            User u = m_Auth->GetLoggedUser();
            auto credentials = UserDatabase::GetInstance()->GetCredential(u.GetUsername());
            tempCredential = credentials[idSelected];
            deleteInitialized = true;
        }

        if (DeleteCredentialPopup(deleteCredential, tempCredential)) {
            AX_INFO("Credenziale eliminata con successo!");
            deleteInitialized = false; // Reset per la prossima apertura
        }

        if (!deleteCredential) {
            deleteInitialized = false; // Reset anche in caso di annullamento
        }
    }
}

void UserInterface::CredentialTable(bool& showEditPopup, bool& showNewPopup, bool& deleteRowSelected, int& idSelected)
{
    static std::string searchText;
    float panelWidth = ImGui::GetContentRegionAvail().x;

    // Sezione ricerca e pulsante aggiunta
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Cerca: ");
    ImGui::SameLine();
    ImGui::InputText("##Search", &searchText);

    ImGui::SameLine();
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 50.0f);
    if (ImGui::Button("+", ImVec2(panelWidth * 0.15f, 0.0f))) {
        showNewPopup = true;
        idSelected = -1;
    }
    ImGui::PopStyleVar();
    ImGui::Separator();

    if (ImGui::BeginTable("CredentialsTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
    {

        ImGui::TableSetupColumn("Service", ImGuiTableColumnFlags_WidthFixed, panelWidth * 0.17f);
        ImGui::TableSetupColumn("Email/Username", ImGuiTableColumnFlags_WidthFixed, panelWidth * 0.30f);
        ImGui::TableSetupColumn("Last Update", ImGuiTableColumnFlags_WidthFixed, panelWidth * 0.2f);
        ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, panelWidth * 0.33f);
        ImGui::TableHeadersRow();

        User u = m_Auth->GetLoggedUser();
        auto credenziali = UserDatabase::GetInstance()->GetCredential(u.GetUsername());

        if (!credenziali.empty())
        {
            for (int i = 0; i < credenziali.size(); i++)
            {

                ImGui::TableNextRow();

                // Colonna Servizio
                ImGui::TableSetColumnIndex(0);
                ImGui::TextLinkOpenURL(credenziali[i].service.c_str(), credenziali[i].url.c_str());

                // Colonna Email/Username
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", credenziali[i].email.c_str());

                // Colonna Data
                ImGui::TableSetColumnIndex(2);
                std::string date = AXIOM::Time::GetDate();
                ImGui::Text("%s", credenziali[i].last_updated.c_str());

                // Colonna Azioni
                ImGui::TableSetColumnIndex(3);
                float columnWidth = ImGui::GetContentRegionAvail().x;
                ImGui::PushID(credenziali[i].id.c_str());
                if (ImGui::Button("Edit", ImVec2(columnWidth * 0.5f, 0.0f))) {
                    showEditPopup = true;
                    idSelected = i;
                }
                ImGui::SameLine();

                if (ImGui::Button("Delete", ImVec2(columnWidth * 0.5f, 0.0f))) {
                    deleteRowSelected = true;
                    idSelected = i;
                }
                ImGui::PopID();
            }
        }

        ImGui::EndTable();
    }
}

void UserInterface::ImportedFilesTable(const json& importedFileData)
{
    static std::string searchText;
    float panelWidth = ImGui::GetContentRegionAvail().x;

    // Sezione ricerca 
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Cerca: ");
    ImGui::SameLine();
    ImGui::InputText("##Search", &searchText);

    ImGui::Separator();

    if (ImGui::BeginTable("CredentialsTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("Service", ImGuiTableColumnFlags_WidthFixed, panelWidth * 0.4f);
        ImGui::TableSetupColumn("Username", ImGuiTableColumnFlags_WidthFixed, panelWidth * 0.6f);
        ImGui::TableHeadersRow();

        
        for (int i = 0; i < importedFileData.size(); i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", importedFileData[i]["name"].dump().c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", importedFileData[i]["username"].dump().c_str());
        }

        ImGui::EndTable();
    }
}

void UserInterface::DynamicImportedFilesTable(const json& importedFileData)
{
    static std::string searchText;
    float panelWidht = ImGui::GetContentRegionAvail().x;

    // Sezione ricerca 
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Cerca: ");
    ImGui::SameLine();
    ImGui::InputText("##Search", &searchText);

    ImGui::Separator();

    std::vector<std::string> columns;
    for (auto& [key, value] : importedFileData[0].items()) {
        columns.push_back(key);
    }

    if (ImGui::BeginTable("CredentialsTable", columns.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
    {
        for (int i = 0; i < columns.size(); i++)
        {
            ImGui::TableSetupColumn(columns[i].c_str());
        }
        ImGui::TableHeadersRow();
        
        
        for (int i = 0; i < importedFileData.size(); i++) 
        {
            ImGui::TableNextRow();
            for (int j = 0; j < columns.size(); j++)
            {
                ImGui::TableSetColumnIndex(j);
                ImGui::Text("%s", importedFileData[i][columns[j]].dump().c_str());
            }
        }
        
        ImGui::EndTable();
    }
}

bool UserInterface::CredentialPopup(const char* title, bool& open, Credential& credenziali)
{
    bool result = false;

    ImGui::OpenPopup(title);
    if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Servizio", &credenziali.service);
        ImGui::InputText("URL", &credenziali.url);

        ImGui::Separator();
        ImGui::Text("Credenziali di accesso:");
        ImGui::InputText("Email", &credenziali.email);
        ImGui::InputText("Password", &credenziali.password, ImGuiInputTextFlags_Password);

        ImGui::Separator();
        ImGui::InputTextMultiline("Note (Opzionale)", &credenziali.notes);

        ImGui::Separator();
        if (ImGui::Button("Salva")) {
            if (!credenziali.service.empty() && !credenziali.email.empty()
                && !credenziali.password.empty() && !credenziali.url.empty())
            {
                if (credenziali.notes.empty()) {
                    credenziali.notes = "null";
                }
                if (credenziali.id.empty()) {
                    UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
                    credenziali.id = uuidGenerator.getUUID().str();
                    credenziali.created_at = AXIOM::Time::GetDate();
                    credenziali.last_updated = AXIOM::Time::GetDate();
                    User u = m_Auth->GetLoggedUser();
                    result = UserDatabase::GetInstance()->RegisterCredential(u.GetUsername(), credenziali);
                    open = false;
                    ImGui::CloseCurrentPopup();
                }
                else {
                    User u = m_Auth->GetLoggedUser();
                    result = UserDatabase::GetInstance()->RegisterCredential(u.GetUsername(), credenziali);
                    open = false;
                    ImGui::CloseCurrentPopup();
                }
            }
            else {
                AX_ERROR("Compila tutti i campi obbligatori");
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Annulla")) {
            open = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    return result;
}

bool UserInterface::DeleteCredentialPopup(bool& open, Credential& credenziali)
{
    bool result = false;

    ImGui::OpenPopup("Eliminazione Credenziali");
    if (ImGui::BeginPopupModal("Eliminazione Credenziali", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

        if (ImGui::Button("Elimina")) {
            User u = m_Auth->GetLoggedUser();
            result = UserDatabase::GetInstance()->DeleteCredential(u.GetUsername(), credenziali);
            open = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancella")) {
            open = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    return result;
}
