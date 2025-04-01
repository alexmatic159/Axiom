#pragma once
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
		static std::string GetAppDataPath();
	};

}