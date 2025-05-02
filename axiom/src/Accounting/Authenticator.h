#pragma once
#include <Axiom.h>

namespace AXIOM {

	enum AuthResult {
		LOGIN_OK = 0, USER_NOT_EXIST,
		REGISTER_OK, USER_ALREADY_EXIST, PSWD_DIFFERENT,
		LOGOUT_OK
	};

	class Authenticator
	{
	public:

		virtual AuthResult Login(std::string& name, std::string& password) = 0;
		
		virtual AuthResult Register(std::string& name, std::string& password, std::string& passwordCheck) = 0;

		virtual AuthResult Logout() = 0;
	};
}