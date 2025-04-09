#pragma once
#include "Logger/Logger.h"

#include <filesystem>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <libloaderapi.h> 
#include <ShlObj.h>

namespace AXIOM {

	class FilePath
	{
	public:
		static std::filesystem::path GetAppPath();
		
		static std::filesystem::path GetAppDataPath();
		
		static bool FileExist(std::filesystem::path file);

	};

}