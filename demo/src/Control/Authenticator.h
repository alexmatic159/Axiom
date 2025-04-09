#pragma once
#include "Axiom.h"

#include "Model/UserDatabase.h"

class Authenticator : AXIOM::Authenticator
{
private:
	std::unique_ptr<UserDatabase> m_UserDB;
public:
	Authenticator();

	AXIOM::AuthResult Login(std::string& name, std::string& password) override;
	
	AXIOM::AuthResult Register(std::string& name, std::string& password, std::string& passwordCheck) override;
	
	AXIOM::AuthResult Logout() override;

};