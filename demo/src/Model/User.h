#pragma once
#include <string>

struct Credential
{
	std::string id;
	std::string service;
	std::string url;
	std::string username;
	std::string email;
	std::string password;
	std::string notes;
	std::string created_at;
	std::string last_updated;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Credential,
		id, service, url, email, password, notes, created_at, last_updated)
};

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

	User() {}

	// Getters
	std::string GetUsername() const { return m_Username; }
	std::string GetPassword() const { return m_Password; }
	UserLevel GetUserAccessLevel() const { return m_AccessLevel; }

	// Setters
	void SetUsername(const std::string& name) { m_Username = name; }

	void Clear() { m_Username.clear(); m_Password.clear(); }

private:
	std::string m_Username;
	std::string m_Password;
	UserLevel m_AccessLevel;
};
