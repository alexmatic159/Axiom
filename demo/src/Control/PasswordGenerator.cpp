#include "PasswordGenerator.h"

PasswordGenerator::PasswordGenerator()
{
    // Inizializza i caratteri disponibili
    lowercase = "abcdefghijklmnopqrstuvwxyz";
    uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    numbers = "0123456789";
    symbols = "!@#$%^&*()-_=+[]{}|;:,.<>?";   
}

std::string PasswordGenerator::GeneratePassword(size_t length, bool use_lower, bool use_upper, bool use_num, bool use_sym)
{
    std::string charPool;

    // Costruisci il pool di caratteri in base alle opzioni
    if (use_lower) charPool += lowercase;
    if (use_upper) charPool += uppercase;
    if (use_num)  charPool += numbers;
    if (use_sym)  charPool += symbols;

    // Verifica che ci siano caratteri disponibili
    if (charPool.empty()) {
        throw std::runtime_error("Nessun tipo di carattere selezionato");
    }

    // Mescola il pool di caratteri
    std::shuffle(charPool.begin(), charPool.end(), rng);

    std::string password;
    std::uniform_int_distribution<size_t> dist(0, charPool.size() - 1);

    // Genera la password
    for (size_t i = 0; i < length; ++i) {
        password += charPool[dist(rng)];
    }

    // Garantisce che la password contenga almeno un carattere di ogni tipo richiesto
    EnforceRules(password, use_lower, use_upper, use_num, use_sym);

    return password;
}
