#pragma once
#include "Core/Base.h"
#include "FilePath.h"

namespace AXIOM {

	class Json
	{
	public:
		static json Read(const std::filesystem::path& filePath);

		static bool Write(const std::filesystem::path& filePath, const json& content, bool forceOverwrite = false);

		static bool Append(const std::filesystem::path& filePath, const json& content);
	};
}