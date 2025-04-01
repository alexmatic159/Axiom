#include "Authenticator.h"


namespace AXIOM {

	AuthResult Authenticator::Login(std::string& name, std::string& password)
	{
		std::string username = "pippo";
		std::string pwd = "franco";

		if ((name == username) && (pwd == password)) 
			return LOGIN_OK;
		return USER_NOT_EXIST;
	}

	AuthResult Authenticator::Register(std::string& name, std::string& password, UserLevel& accessLevel)
	{
		User u(name, password, accessLevel);

		// TODO: add to file

		return REGISTER_OK;
	}

	AuthResult Authenticator::Logout()
	{
		return LOGOUT_OK;
	}
}