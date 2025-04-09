#include "Authenticator.h"

Authenticator::Authenticator()
{
	std::filesystem::path dbPath = AXIOM::FilePath::GetAppDataPath() / "Axiom/data/credentials.json";
	m_UserDB = std::make_unique<UserDatabase>(dbPath);
}

AXIOM::AuthResult Authenticator::Login(std::string& name, std::string& password)
{
	// Controllo se l'utente esiste
	if (!m_UserDB->FindUser(name)) {
		return AXIOM::AuthResult::USER_NOT_EXIST;
	}
	
	// Registro il login se le password sono uguali
	User existingUser = m_UserDB->GetUser(name);

	if (password == existingUser.GetPassword()) {
		m_UserDB->RegisterAccess(name);
		return AXIOM::AuthResult::LOGIN_OK;
	}
	else {
		return AXIOM::AuthResult::PSWD_DIFFERENT;
	}
}

AXIOM::AuthResult Authenticator::Register(std::string& name, std::string& password, std::string& passwordCheck)
{
	// Controllo se l'utente esiste già
	if (m_UserDB->FindUser(name)) {
		return AXIOM::AuthResult::USER_ALREADY_EXIST;
	}

	// Controllo che le password inserite siano uguali
	if (password != passwordCheck) {
		return AXIOM::AuthResult::PSWD_DIFFERENT;
	}
	
	// Preparo il record da inserire nel db 
	UserLevel level = UserLevel::Client;
	User u(name, password, level);

	m_UserDB->AddRecord(u);

	return AXIOM::AuthResult::REGISTER_OK;
	
}

AXIOM::AuthResult Authenticator::Logout()
{
	return AXIOM::AuthResult::LOGOUT_OK;
}