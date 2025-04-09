#include "FilePath.h"

namespace AXIOM {

	std::filesystem::path FilePath::GetAppPath()
	{
#ifdef _WIN32
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
#else
        char path[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
#endif
        return std::filesystem::path(path).parent_path();
	}

    std::filesystem::path FilePath::GetAppDataPath()
	{
        char appDataPath[MAX_PATH];
        if (SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath) == S_OK) {
            return std::filesystem::path(appDataPath);
        }
        return "";
	}

    bool FilePath::FileExist(std::filesystem::path file)
    {
        if (!std::filesystem::exists(file)) {
            AX_INFO("Il file: {} non esiste", file.string());
            return false;
        }
        return true;
    }

}