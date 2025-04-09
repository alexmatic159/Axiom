#include "Json.h"

namespace AXIOM {

	json Json::Read(const std::filesystem::path& filePath)
	{
		if (!std::filesystem::exists(filePath)) {
			AX_ERROR("File does not exist: {}", filePath.string());
			return json();
		}

		std::ifstream file(filePath);
		if (!file.is_open()) {
			AX_ERROR("Failed to open file: {}", filePath.string());
			return json();
		}

		try {
			return json::parse(file);
		}
		catch (const std::exception& e) {
			AX_ERROR("JSON parse error in {}: {}", filePath.string(), e.what());
			return json();
		}
	}

	bool Json::Write(const std::filesystem::path& filePath, const json& content, bool forceOverwrite)
	{
		// Se non forziamo overwrite e il file esiste, facciamo append
		if (!forceOverwrite && std::filesystem::exists(filePath)) {
			return Append(filePath, content);
		}

		// Altrimenti crea/sovrascrive il file
		std::ofstream file(filePath);
		if (!file.is_open()) {
			AX_ERROR("Failed to create file: {}", filePath.string());
			return false;
		}

		file << content.dump(4); // Formattazione consistente
		return true;
	}

	bool Json::Append(const std::filesystem::path& filePath, const json& content)
	{
		if (!std::filesystem::exists(filePath)) {
			AX_INFO("File not found, creating new one");
			return Write(filePath, content, true);
		}

		json existingData = Read(filePath);
		if (existingData.is_null()) {
			AX_ERROR("Failed to read existing file for append");
			return false;
		}

		// Logica di merge
		if (existingData.is_array() && content.is_array()) {
			existingData.insert(existingData.end(), content.begin(), content.end());
		}
		else if (existingData.is_object() && content.is_object()) {
			existingData.update(content);
		}
		else if (existingData.is_null()) {
			existingData = content;
		}
		else {
			existingData = json::array({ existingData, content });
		}

		AX_INFO("Merged data: {}", existingData.dump());

		// Sovrascrive con i dati combinati
		return Write(filePath, existingData, true);
	}

}