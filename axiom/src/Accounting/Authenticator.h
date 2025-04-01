#pragma once
#include "User.h"

namespace AXIOM {

	enum AuthResult {
		LOGIN_OK = 0, USER_NOT_EXIST, 
		REGISTER_OK, USER_ALREADY_EXIST,
		LOGOUT_OK
	};

	class Authenticator
	{
	public:

		static AuthResult Login(std::string& name, std::string& password);
		
		static AuthResult Register(std::string& name, std::string& password, UserLevel& accessLevel);

		static AuthResult Logout();
	};
}