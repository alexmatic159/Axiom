#pragma once
#include <iostream>

namespace AXIOM {

	enum UserLevel {
		Admin = 0, Client = 1
	};

	class User
	{
	public:
		// Getters
		std::string GetUsername() { return m_Username; }
		UserLevel GetUserAccessLevel() { return m_AccessLevel; }

		// Setters

	private:
		std::string m_Username;
		std::string password;
		UserLevel m_AccessLevel;
	};
}