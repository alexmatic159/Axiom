#include "Json.h"

namespace AXIOM {

	json Json::Read(const std::string& filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open()) {
			AX_ERROR("Failed to open file: {}", filePath);
		}
		json data = json::parse(file);

		return data;
	}

	void Json::Write(const std::string& filePath, const std::string& content)
	{
		std::ofstream file(filePath);
		if (!file.is_open()) {
			AX_ERROR("Failed to create file: {}", filePath);
		}
		file << content;
	}

	void Json::Append(const std::string& filePath, const std::string& content)
	{

	}
}