#pragma once
#include <iostream>

namespace AXIOM {

	enum UserLevel {
		Admin = 0, Client = 1
	};

	class User
	{
	public:
		User(std::string& username, std::string password, UserLevel level)
			: m_Username(username), m_Password(password), m_AccessLevel(level)
		{

		}

		// Getters
		std::string GetUsername() { return m_Username; }
		UserLevel GetUserAccessLevel() { return m_AccessLevel; }

		// Setters

	private:
		std::string m_Username;
		std::string m_Password;
		UserLevel m_AccessLevel;
	};
}