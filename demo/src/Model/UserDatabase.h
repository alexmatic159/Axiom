#pragma once
#include "Axiom.h"

#include "Model/User.h"

class UserDatabase : public AXIOM::Database<json, User>
{
private:
    std::filesystem::path m_DBpath;

protected:
    json Get() override;
    bool Save(const json& db) override;

public:
    UserDatabase(const std::filesystem::path& path) : m_DBpath(path) {}

    bool AddRecord(const User& record) override;

    // Client defined methods
    //bool EditRecord(const User& record);
    void RegisterAccess(const std::string& name);
    bool FindUser(const std::string& name);
    User GetUser(const std::string& name);


    //m_db = std::make_unique<AXIOM::Database>(GetPath());
    /*
        // Registra un nuovo utente
        bool RegisterUser(const std::string& username, const std::string& plainPassword, int userLevel);

        // Verifica le credenziali
        bool Authenticate(const std::string& username, const std::string& plainPassword);

        // Ottieni il livello utente
        int GetUserLevel(const std::string& username);

        // Aggiorna la password
        bool UpdatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword);
        */

};