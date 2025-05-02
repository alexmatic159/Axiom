#pragma once
#include "Axiom.h"

#include "Model/User.h"

class UserDatabase : public AXIOM::Database<json, User>
{
protected:
    UserDatabase(const std::filesystem::path& path) : m_DBpath(path) {}
    
    static UserDatabase* s_DB;

    json Get() override;
    bool Save(const json& db) override;

public:
    static UserDatabase* Create(const std::filesystem::path& path);
    static UserDatabase* GetInstance();

    bool AddRecord(const User& record) override;

    // Client defined methods
    void RegisterAccess(const std::string& name);
    bool RegisterCredential(const std::string& name, const Credential& credenziali);
    bool DeleteCredential(const std::string& name, const Credential& credenziali);
    std::vector<Credential> GetCredential(const std::string& name);
    bool FindUser(const std::string& name);
    User GetUser(const std::string& name);

    void EncryptDB();
    void DecryptDB();

private:
    std::filesystem::path m_DBpath;

};