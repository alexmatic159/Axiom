#pragma once
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <filesystem>
#include <vector>
#include <string>

namespace AXIOM {

	class Crypto
	{
	private:
		std::filesystem::path m_FilePath;
		std::vector<unsigned char> m_EncryptionKey;

	public:
		Crypto(const std::filesystem::path& path, const std::string& masterPassword);
		void CreateRecord(const std::string& url, const std::string& user, const std::string& pwd);
	private:
		std::vector<unsigned char> Encrypt(const std::string& text);
		std::string Decrypt(const std::vector<unsigned char>& cipherText);

	};
}