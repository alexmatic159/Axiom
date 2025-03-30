#pragma once
#include "User.h"

namespace AXIOM {

	class Authenticator
	{
	public:

		bool Login(User& u);
		User Register(std::string& name, std::string password, int accessLevel);

		bool Logout();
	};
}