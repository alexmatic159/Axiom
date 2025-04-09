#pragma once
#include "Axiom.h"
#include <vector>
#include <random>
#include <algorithm>
#include <climits>

class PasswordGenerator
{
private:
	std::string lowercase;
	std::string uppercase;
	std::string numbers;
	std::string symbols;
	std::random_device rd;
	std::mt19937 rng{ rd() };

    void EnforceRules(std::string& password, bool use_lower, bool use_upper,
        bool use_num, bool use_sym) {
        // Controlla e aggiunge caratteri mancanti se necessario
        if (use_lower && password.find_first_of(lowercase) == std::string::npos) {
            ReplaceRandomChar(password, lowercase);
        }
        if (use_upper && password.find_first_of(uppercase) == std::string::npos) {
            ReplaceRandomChar(password, uppercase);
        }
        if (use_num && password.find_first_of(numbers) == std::string::npos) {
            ReplaceRandomChar(password, numbers);
        }
        if (use_sym && password.find_first_of(symbols) == std::string::npos) {
            ReplaceRandomChar(password, symbols);
        }
    }

    void ReplaceRandomChar(std::string& password, const std::string& char_set) {
        std::uniform_int_distribution<size_t> dist_pass(0, password.size() - 1);
        std::uniform_int_distribution<size_t> dist_char(0, char_set.size() - 1);

        password[dist_pass(rng)] = char_set[dist_char(rng)];
    }
public:
    PasswordGenerator();

	std::string GeneratePassword(size_t length, bool use_lower = true, bool use_upper = true, bool use_num = true, bool use_sym = true);
};