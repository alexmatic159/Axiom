#include "FileManager.h"

namespace AXIOM {

	std::string FileManager::ReadTXT(const std::filesystem::path& filePath)
	{
		std::ifstream file(filePath);

		return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	}

	json FileManager::ReadJSON(const std::filesystem::path& filePath)
	{
		if (!std::filesystem::exists(filePath)) {
			AX_ERROR("File does not exist: {}", filePath.string());
			return json();
		}

		std::ifstream file(filePath);
		if (!file.is_open()) {
			AX_ERROR("Failed to open file: {}", filePath.string());
		}

		return json::parse(file);
	}

	csv FileManager::ReadCSV(const std::filesystem::path& filePath)
	{
		csv data;
		std::ifstream file(filePath);

		if (!file.is_open())
		{
			AX_ERROR("Errore apertura file CSV: {}", filePath.string());
			return data;
		}

		std::string line;
		while (std::getline(file, line))
		{
			std::vector<std::string> row;
			std::stringstream ss(line);
			std::string cell;

			while (std::getline(ss, cell, ','))
			{
				row.push_back(cell);
			}

			data.push_back(row);
		}

		return data;
	}

	std::vector<unsigned char> FileManager::ReadBIN(const std::filesystem::path& filePath)
	{
		// 1. Apertura file
		std::ifstream file(filePath, std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			throw std::filesystem::filesystem_error(
				"Failed to open file", filePath,
				std::make_error_code(std::errc::no_such_file_or_directory));
		}

		// 2. Determina dimensione
		auto fileSize = file.tellg();
		if (fileSize == -1) {
			throw std::runtime_error("Failed to determine file size");
		}
		file.seekg(0);

		// 3. Lettura contenuto
		std::vector<unsigned char> buffer(fileSize);
		if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
			throw std::runtime_error("Failed to read file contents");
		}

		return buffer;
	}

	bool FileManager::WriteTXT(const std::filesystem::path& filePath, const std::string& content)
	{
		std::ofstream file(filePath);
		if (!file.is_open()) {
			AX_ERROR("Failed to create file: {}", filePath.string());
			return false;
		}
		file << content;

		return true;
	}

	bool FileManager::WriteJSON(const std::filesystem::path& filePath, const json& content)
	{
		std::ofstream file(filePath);
		if (!file.is_open()) {
			return false;
		}
		file << content.dump(4);

		return true;
	}

	bool FileManager::WriteCSV(const std::filesystem::path& filePath, const std::string& content)
	{
		return false;
	}

	bool FileManager::WriteBIN(const std::filesystem::path& filePath, const std::vector<std::vector<unsigned char>>& dataBlocks)
	{
		// 1. Apertura file
		std::ofstream file(filePath, std::ios::binary);
		if (!file.is_open()) {
			AX_ERROR("Failed to open file for writing: {}", filePath.string());
			return false;
		}

		// 2. Scrittura blocchi
		try {
			for (const auto& block : dataBlocks) {
				if (!block.empty()) {
					file.write(reinterpret_cast<const char*>(block.data()), block.size());
					if (!file) {
						throw std::runtime_error("Write operation failed");
					}
				}
			}
			return true;
		}
		catch (const std::exception& e) {
			AX_ERROR("File write error: {} - {}", filePath.string(), e.what());
			file.close();
			std::filesystem::remove(filePath);
			return false;
		}
	}

	json FileManager::CSVtoJSON(const std::filesystem::path& filePath)
	{
		std::ifstream file(filePath);
		json result;

		if (!file.is_open())
		{
			AX_ERROR("Errore apertura file CSV: {}", filePath.string());
			return result;
		}

		std::string line;
		std::vector<std::string> headers;

		// Legge la prima riga per gli header
		if (std::getline(file, line))
		{
			std::stringstream ss(line);
			std::string cell;
			while (std::getline(ss, cell, ','))
			{
				headers.push_back(cell);
			}
		}

		// Legge le righe successive
		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			std::string cell;
			json rowJson;
			size_t i = 0;

			while (std::getline(ss, cell, ','))
			{
				if (i < headers.size())
				{
					rowJson[headers[i]] = cell;
				}
				++i;
			}

			result.push_back(rowJson);
		}

		return result;
	}

	std::filesystem::path FileManager::GetAppDataPath()
	{
		char appDataPath[MAX_PATH];
		if (SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath) == S_OK) {
			return std::filesystem::path(appDataPath);
		}
		return "";
	}

	std::string FileManager::OpenFileExplorer()
	{
		wchar_t filePath[MAX_PATH] = L"";

		OPENFILENAMEW ofn;
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFilter = L"Tutti i file\0*.*\0JSON files\0*.json\0CSV files\0*.csv\0";
		ofn.lpstrFile = filePath;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		ofn.lpstrTitle = L"Seleziona un file da importare";

		if (GetOpenFileNameW(&ofn))
		{
			// Converti da wstring a string (UTF-8 o ANSI a seconda delle necessità)
			char pathStr[MAX_PATH];
			wcstombs(pathStr, filePath, MAX_PATH);
			return std::string(pathStr);
		}

		return "";
	}

}