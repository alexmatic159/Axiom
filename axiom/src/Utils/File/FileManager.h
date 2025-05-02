#pragma once
#include "Core/Base.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <libloaderapi.h> 
#include <ShlObj.h>
#include <commdlg.h>

namespace AXIOM {

	class FileManager
	{
	public:
		// Files Related
		static std::string ReadTXT(const std::filesystem::path& filePath);
		static json ReadJSON(const std::filesystem::path& filePath);
		static csv ReadCSV(const std::filesystem::path& filePath);
		static std::vector<unsigned char> ReadBIN(const std::filesystem::path& filePath);

		static bool WriteTXT(const std::filesystem::path& filePath, const std::string& content);
		static bool WriteJSON(const std::filesystem::path& filePath, const json& content);
		static bool WriteCSV(const std::filesystem::path& filePath, const std::string& content);
		static bool WriteBIN(const std::filesystem::path& filePath, const std::vector<std::vector<unsigned char>>& dataBlocks);

		static json CSVtoJSON(const std::filesystem::path& filePath);

		// Path Related
		static std::filesystem::path GetAppDataPath();

		// File Explorer Related
		static std::string OpenFileExplorer();
		
	};
}