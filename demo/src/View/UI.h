#pragma once
#include "Axiom.h"

#include "Control/Authenticator.h"
#include "Control/PasswordGenerator.h"

class UserInterface 
{
private:
    ImGuiWindowClass m_WindowClass;
    bool m_LoginCheck = false;
    bool m_RegisterCheck = false;
    bool m_GeneratorVisibility = false;

    std::unique_ptr<Authenticator> m_Auth;
public:
    UserInterface();

    void View();

    void LoggedView();

    void MenuBarView();

    void SideView();

    void CenterView();

    void LoginFormView();

    void RegisterFormView();
};

