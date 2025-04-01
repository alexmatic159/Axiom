#include "Crypto.h"
#include "Logger/Logger.h"

#include <iostream>

namespace AXIOM {

	Crypto::Crypto(const std::filesystem::path& path, const std::string& masterPassword)
		: m_FilePath(path)
	{
		m_EncryptionKey.resize(32); // 32 byte = 256 bit
		const unsigned char* salt = (const unsigned char*)"salt"; // TODO: salt generato random

		PKCS5_PBKDF2_HMAC(
			masterPassword.c_str(), masterPassword.length(),
			salt, 5,
			100000,
			EVP_sha256(),
			m_EncryptionKey.size(), m_EncryptionKey.data()
		);
	}

	void Crypto::CreateRecord(const std::string& url, const std::string& user, const std::string& pwd)
	{
		std::string plainText = url + user + pwd;
		AX_TRACE("{}", plainText);

		std::vector<unsigned char> encrypted = Encrypt(plainText);
		
		AX_TRACE("{}", reinterpret_cast<const char*>(encrypted.data()));
		// Salva su file
		/*
		std::ofstream file(filePath, std::ios::binary);
		file.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
		*/
	}

	std::vector<unsigned char> Crypto::Encrypt(const std::string& text)
	{
		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		std::vector<unsigned char> iv(16); // Vettore di inizializzazione
		RAND_bytes(iv.data(), iv.size());  // IV random per ogni cifratura

		std::vector<unsigned char> ciphertext(text.size() + EVP_MAX_BLOCK_LENGTH);
		int len;
		int ciphertext_len;

		EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
			m_EncryptionKey.data(), iv.data());

		// Aggiungi l'IV all'inizio del ciphertext
		ciphertext.insert(ciphertext.begin(), iv.begin(), iv.end());

		EVP_EncryptUpdate(ctx, ciphertext.data() + iv.size(), &len,
			(const unsigned char*)text.c_str(), text.length());
		ciphertext_len = len;

		EVP_EncryptFinal_ex(ctx, ciphertext.data() + iv.size() + len, &len);
		ciphertext_len += len;

		EVP_CIPHER_CTX_free(ctx);

		ciphertext.resize(iv.size() + ciphertext_len);
		return ciphertext;
	}

	std::string Crypto::Decrypt(const std::vector<unsigned char>& cipherText)
	{
		if (cipherText.size() <= 16) {
			AX_ERROR("Dati cifrati non validi");
		}

		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		std::vector<unsigned char> iv(cipherText.begin(), cipherText.begin() + 16);
		std::vector<unsigned char> plaintext(cipherText.size() - 16);

		int len;
		int plaintext_len;

		EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL,
			m_EncryptionKey.data(), iv.data());

		EVP_DecryptUpdate(ctx, plaintext.data(), &len,
			cipherText.data() + 16, cipherText.size() - 16);
		plaintext_len = len;

		EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
		plaintext_len += len;

		EVP_CIPHER_CTX_free(ctx);

		plaintext.resize(plaintext_len);
		return std::string(plaintext.begin(), plaintext.end());
	}
}


