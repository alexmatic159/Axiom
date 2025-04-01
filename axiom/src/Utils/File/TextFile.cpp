#include "TextFile.h"

namespace AXIOM {
	std::string TextFile::Read(const std::string& filePath)
	{
        std::ifstream file(filePath);
        if (!file.is_open()) {
            AX_ERROR("Failed to open file: {}", filePath);
        }
        return std::string((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
	}
    
    void TextFile::Write(const std::string& filePath, const std::string& content)
    {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            AX_ERROR("Failed to create file: {}", filePath);
        }
        file << content;
    }

    void TextFile::Append(const std::string& filePath, const std::string& content)
    {
        std::ofstream file(filePath, std::ios::app);
        if (!file.is_open()) {
            AX_ERROR("Failed to open file: {}", filePath);
        }
        file << content;
    }
}