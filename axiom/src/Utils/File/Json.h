#pragma once
#include "Logger/Logger.h"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

namespace AXIOM {

	using json = nlohmann::json;

	class Json
	{
	public:
		static json Read(const std::string& filePath);

		static void Write(const std::string& filePath, const std::string& content);

		static void Append(const std::string& filePath, const std::string& content);
	};
}