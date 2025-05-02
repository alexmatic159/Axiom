#pragma once
#include "Axiom.h"

#include "Control/UserDatabase.h"
#include "Model/User.h";

class Authenticator : AXIOM::Authenticator
{
private:
	User m_LoggedUser;
public:
	Authenticator();

	AXIOM::AuthResult Login(std::string& name, std::string& password) override;
	
	AXIOM::AuthResult Register(std::string& name, std::string& password, std::string& passwordCheck) override;
	
	AXIOM::AuthResult Logout() override;

	// Client Defined methods
	User GetLoggedUser() const { return m_LoggedUser; }

};