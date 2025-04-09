#include "UserDatabase.h"

json UserDatabase::Get()
{
    if (!AXIOM::FilePath::FileExist(m_DBpath)) {
        return { {"users", json::object()} };
    }

    std::ifstream file(m_DBpath);
    if (!file.is_open()) {
        AX_INFO("Failed to open user database");
    }

    return json::parse(file);
}

bool UserDatabase::Save(const json& db)
{
    std::ofstream file(m_DBpath);
    if (!file.is_open()) {
        return false;
    }
    file << db.dump(4);
    return true;
}

bool UserDatabase::AddRecord(const User& record)
{
    json db = Get();

    // Hash della password
    //std::string hashedPassword = BCrypt::generateHash(plainPassword);

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

/*
bool Database::Authenticate(const std::string& username, const std::string& plainPassword)
{
    json db = Load();

    // Verifica se l'utente esiste
    if (!db["users"].contains(username)) {
        return false;
    }

    // Recupera la password hashed
    std::string hashedPassword = db["users"][username]["password"];

    // Verifica la password
    bool passwordMatch = true; //BCrypt::validatePassword(plainPassword, hashedPassword);

    // Aggiorna last_login se l'autenticazione ha successo
    if (passwordMatch) {
        db["users"][username]["last_login"] = currentTimestamp();
        Save(db);
    }

    return passwordMatch;
}

int Database::GetUserLevel(const std::string& username)
{
    json db = Load();
    if (!db["users"].contains(username)) {
        return -1;  // Codice per utente non trovato
    }
    return db["users"][username]["level"];
}

bool Database::UpdatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword)
{
    if (!Authenticate(username, oldPassword)) {
        return false;
    }

    json db = Load();
    db["users"][username]["password"] = newPassword;
    return Save(db);
}
*/