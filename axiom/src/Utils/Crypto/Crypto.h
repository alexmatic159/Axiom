#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

namespace AXIOM {

	class Crypto
	{
	private:
		std::filesystem::path m_FilePath;
		std::vector<unsigned char> m_EncryptionKey;

	public:
		Crypto(const std::filesystem::path& path, const std::string& masterPassword);

		void CreateRecord(const std::string& url, const std::string& user, const std::string& pwd);

		void DisplayEncryptedData(const std::vector<uint8_t>& encrypted_data);
	private:
		std::vector<unsigned char> Encrypt(const std::string& text);

		std::string Decrypt(const std::vector<unsigned char>& cipherText);

		void DeriveKey(const std::string& password, const std::vector<uint8_t>& salt);
	};
}