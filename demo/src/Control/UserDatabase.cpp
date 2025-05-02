#include "UserDatabase.h"

UserDatabase* UserDatabase::s_DB = nullptr;

json UserDatabase::Get()
{
    if (!std::filesystem::exists(m_DBpath)) {
        return { {"users", json::object()} };
    }

    return AXIOM::FileManager::ReadJSON(m_DBpath);
}

bool UserDatabase::Save(const json& db)
{
    return AXIOM::FileManager::WriteJSON(m_DBpath, db);
}

UserDatabase* UserDatabase::Create(const std::filesystem::path& path)
{
    if (s_DB == nullptr)
        s_DB = new UserDatabase(path);
    return s_DB;
}

UserDatabase* UserDatabase::GetInstance()
{
    return s_DB;
}

bool UserDatabase::AddRecord(const User& record)
{
    json db = Get();

    db["users"][record.GetUsername()] = {
        {"password", record.GetPassword()},
        {"level", record.GetUserAccessLevel()},
        {"created_at", AXIOM::Time::GetTimestamp()},
        {"last_login", AXIOM::Time::GetTimestamp()}
    };

    return Save(db);
}

void UserDatabase::RegisterAccess(const std::string& name)
{
    json db = Get();

    db["users"][name]["last_login"] = AXIOM::Time::GetTimestamp();

    Save(db);
}

bool UserDatabase::RegisterCredential(const std::string& name, const Credential& credenziali)
{
    json db = Get();

    for (auto& cred : db["users"][name]["credentials"]) {
        if (cred["id"] == credenziali.id) {
            cred["service"] = credenziali.service;
            cred["url"] = credenziali.url;
            cred["email"] = credenziali.email;
            cred["password"] = credenziali.password;
            cred["notes"] = credenziali.notes;
            cred["last_updated"] = AXIOM::Time::GetDate();
            return Save(db);
        }
    }

    db["users"][name]["credentials"].push_back({
       {"id", credenziali.id},
       {"service", credenziali.service},
       {"url", credenziali.url},
       {"email", credenziali.email},
       {"password", credenziali.password},
       {"notes", credenziali.notes},
       {"created_at", credenziali.created_at}, // Formato "YYYY-MM-DD"
       { "last_updated", credenziali.created_at } // Formato "YYYY-MM-DD"
        });

    return Save(db);
}

bool UserDatabase::DeleteCredential(const std::string& name, const Credential& credenziali)
{
    json db = Get();

    auto& credentials = db["users"][name]["credentials"];
    for (auto it = credentials.begin(); it != credentials.end(); it++ ) {
        if ((*it)["id"] == credenziali.id) {
            credentials.erase(it);
            return Save(db);
        }
    }

    return false;
}

std::vector<Credential> UserDatabase::GetCredential(const std::string& name)
{
    
    json db = Get();

    if (db["users"][name]["credentials"].empty()) {
        return {};
    }
    
    return db["users"][name]["credentials"].get<std::vector<Credential>>();
}

bool UserDatabase::FindUser(const std::string& name)
{
    json db = Get();

    if (db["users"].contains(name)) {
        return true;
    }

    return false;
}

User UserDatabase::GetUser(const std::string& name)
{
    json db = Get();

    User u(
        name, 
        db["users"][name]["password"], 
        (UserLevel)db["users"][name]["level"]
    );

    return u;    
}

void UserDatabase::EncryptDB()
{
    //AXIOM::Crypt::EncryptFileWithSalt(m_DBpath);
}

void UserDatabase::DecryptDB()
{
    //AXIOM::Crypt::DecryptFileWithSalt(m_DBpath);
}
