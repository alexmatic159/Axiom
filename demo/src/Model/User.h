#pragma once
#include <string>

enum UserLevel {
	Admin = 0, Client = 1
};

class User
{
public:
	User(const std::string& username, const std::string& password, const UserLevel& level)
		: m_Username(username), m_Password(password), m_AccessLevel(level)
	{

	}

	// Getters
	std::string GetUsername() const { return m_Username; }
	std::string GetPassword() const { return m_Password; }
	UserLevel GetUserAccessLevel() const { return m_AccessLevel; }

	// Setters

private:
	std::string m_Username;
	std::string m_Password;
	UserLevel m_AccessLevel;
};
