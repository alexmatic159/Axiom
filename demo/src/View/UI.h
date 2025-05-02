#pragma once
#include "Control/Authenticator.h"
#include "Control/PasswordGenerator.h"

#include "Axiom.h"

class UserInterface 
{
private:
    ImGuiWindowClass m_WindowClass;
    bool m_LoginCheck = false;
    bool m_RegisterCheck = false;
    bool m_GeneratorVisibility = false;
    bool m_VaultVisibility = false;
    bool m_ImportVisibility = false;

    std::filesystem::path m_ImportedFilePath;
    std::unique_ptr<Authenticator> m_Auth;
public:
    UserInterface();
    ~UserInterface();

    void MainView();

    void LoggedView();

    void MenuBarView();

    void SideView();

    void CenterView();

    void LoginFormView();

    void RegisterFormView();

    void PasswordGeneratorView();

    void VaultView();

    void CredentialTable(bool& showEditPopup, bool& showNewPopup, bool& deleteRowSelected, int& idSelected);

    void ImportedFilesTable(const json& importedFileData);

    void DynamicImportedFilesTable(const json& importedFileData);

    bool CredentialPopup(const char* title, bool& open, Credential& credenziali);

    bool DeleteCredentialPopup(bool& open, Credential& credenziali);
};

