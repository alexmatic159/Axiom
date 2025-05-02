#include "Authenticator.h"

Authenticator::Authenticator()
{
	
}

AXIOM::AuthResult Authenticator::Login(std::string& name, std::string& password)
{
	// Controllo se l'utente esiste
	if (!UserDatabase::GetInstance()->FindUser(name)) {
		return AXIOM::AuthResult::USER_NOT_EXIST;
	}
	
	// Registro il login se le password sono uguali
	User existingUser = UserDatabase::GetInstance()->GetUser(name);

	if (password == existingUser.GetPassword()) {
		UserDatabase::GetInstance()->RegisterAccess(name);
		m_LoggedUser = existingUser;
		return AXIOM::AuthResult::LOGIN_OK;
	}
	else {
		return AXIOM::AuthResult::PSWD_DIFFERENT;
	}
}

AXIOM::AuthResult Authenticator::Register(std::string& name, std::string& password, std::string& passwordCheck)
{
	// Controllo se l'utente esiste già
	if (UserDatabase::GetInstance()->FindUser(name)) {
		return AXIOM::AuthResult::USER_ALREADY_EXIST;
	}

	// Controllo che le password inserite siano uguali
	if (password != passwordCheck) {
		return AXIOM::AuthResult::PSWD_DIFFERENT;
	}
	
	// Preparo il record da inserire nel db 
	UserLevel level = UserLevel::Client;
	User u(name, password, level);

	UserDatabase::GetInstance()->AddRecord(u);

	m_LoggedUser.SetUsername(name);
	return AXIOM::AuthResult::REGISTER_OK;
}

AXIOM::AuthResult Authenticator::Logout()
{
	m_LoggedUser.Clear();
	return AXIOM::AuthResult::LOGOUT_OK;
}