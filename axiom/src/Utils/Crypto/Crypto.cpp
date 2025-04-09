#include "Crypto.h"
#include "Logger/Logger.h"
#include "../File/FilePath.h"

namespace AXIOM {

	Crypto::Crypto(const std::filesystem::path& path, const std::string& masterPassword)
		: m_FilePath(path)
	{
        
        // Controlla se il file esiste
        bool isNewFile = !std::filesystem::exists(path);
        std::vector<uint8_t> salt(16);

        if (isNewFile) {
            // Genera un nuovo sale e scrivilo nel file
            if (RAND_bytes(salt.data(), salt.size()) != 1) {
                throw std::runtime_error("Impossibile generare il sale");
            }
            std::ofstream file(path, std::ios::binary);
            if (!file) {
                throw std::runtime_error("Impossibile creare il file");
            }
            file.write(reinterpret_cast<const char*>(salt.data()), salt.size());
            file.close();
        }
        else {
            // Leggi il sale esistente
            std::ifstream file(path, std::ios::binary);
            if (!file) {
                throw std::runtime_error("Impossibile aprire il file");
            }
            file.read(reinterpret_cast<char*>(salt.data()), salt.size());
            if (file.gcount() != salt.size()) {
                throw std::runtime_error("Impossibile leggere il sale");
            }
            file.close();
        }

        // Deriva la chiave dalla password e dal sale
        DeriveKey(masterPassword, salt);
	}

	void Crypto::CreateRecord(const std::string& url, const std::string& user, const std::string& pwd)
	{
        std::string data = url + "|" + user + "|" + pwd;
        auto encrypted = Encrypt(data);

        DisplayEncryptedData(encrypted);
        AX_INFO("{}", Decrypt(encrypted));

        // Scrivi i dati cifrati nel file
        std::ofstream file(m_FilePath, std::ios::binary | std::ios::app);
        if (!file) {
            throw std::runtime_error("Impossibile aprire il file per la scrittura");
        }
        file.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
        file.close();
	}

	std::vector<unsigned char> Crypto::Encrypt(const std::string& text)
	{
        // Genera un IV casuale
        std::vector<uint8_t> iv(16);
        if (RAND_bytes(iv.data(), iv.size()) != 1) {
            throw std::runtime_error("Generazione IV fallita");
        }

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Impossibile allocare EVP_CIPHER_CTX");
        }

        // Inizializza la cifratura
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, m_EncryptionKey.data(), iv.data()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Inizializzazione cifratura fallita");
        }

        // Buffer per il testo cifrato
        int cipher_len, len;
        std::vector<uint8_t> ciphertext(text.length() + 16);

        // Aggiorna il contesto con i dati
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &cipher_len, reinterpret_cast<const uint8_t*>(text.data()), text.length()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Aggiornamento cifratura fallito");
        }
        len = cipher_len;

        // Finalizza la cifratura
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &cipher_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Finalizzazione cifratura fallita");
        }
        len += cipher_len;
        EVP_CIPHER_CTX_free(ctx);

        ciphertext.resize(len);

        // Combina IV e testo cifrato
        std::vector<uint8_t> encrypted_data(iv.size() + ciphertext.size());
        std::copy(iv.begin(), iv.end(), encrypted_data.begin());
        std::copy(ciphertext.begin(), ciphertext.end(), encrypted_data.begin() + iv.size());

        return encrypted_data;
	}

	std::string Crypto::Decrypt(const std::vector<unsigned char>& cipherText)
	{
        if (cipherText.size() < 16) {
            throw std::runtime_error("Testo cifrato troppo corto");
        }

        // Estrai IV e testo cifrato
        std::vector<uint8_t> iv(cipherText.begin(), cipherText.begin() + 16);
        std::vector<uint8_t> ciphertext(cipherText.begin() + 16, cipherText.end());

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Impossibile allocare EVP_CIPHER_CTX");
        }

        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, m_EncryptionKey.data(), iv.data()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Inizializzazione decifrazione fallita");
        }

        int plain_len, len;
        std::vector<uint8_t> plaintext(ciphertext.size() + 16);

        // Aggiorna il contesto con i dati
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &plain_len, ciphertext.data(), ciphertext.size()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Aggiornamento decifrazione fallito");
        }
        len = plain_len;

        // Finalizza la decifrazione
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &plain_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Finalizzazione decifrazione fallita");
        }
        len += plain_len;
        EVP_CIPHER_CTX_free(ctx);

        plaintext.resize(len);
        return std::string(reinterpret_cast<const char*>(plaintext.data()), plaintext.size());
	}

    void Crypto::DeriveKey(const std::string& password, const std::vector<uint8_t>& salt)
    {
        m_EncryptionKey.resize(32); // AES-256 richiede 32 byte
        if (PKCS5_PBKDF2_HMAC(
            password.c_str(), password.length(),
            salt.data(), salt.size(),
            100000,
            EVP_sha256(),
            m_EncryptionKey.size(),
            m_EncryptionKey.data()) != 1) {
            throw std::runtime_error("Derivazione chiave fallita");
        }
    }

	void Crypto::DisplayEncryptedData(const std::vector<uint8_t>& encrypted_data)
	{
        for (auto byte : encrypted_data) {
            printf("%02x", byte);
        }
        printf("\n");
	}

}


