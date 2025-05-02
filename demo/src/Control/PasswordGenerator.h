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

    void EnforceRules(std::string& password, bool use_lower, bool use_upper, bool use_num, bool use_sym);

	void ReplaceRandomChar(std::string& password, const std::string& char_set);
public:
    PasswordGenerator();

	std::string GeneratePassword(size_t length, bool use_lower = true, bool use_upper = true, bool use_num = true, bool use_sym = true);
};