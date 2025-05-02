#include "Crypt.h"

namespace AXIOM {

    constexpr size_t Crypt::KEY_SIZE;
    constexpr size_t Crypt::SALT_SIZE;
    constexpr size_t Crypt::IV_SIZE;

    std::filesystem::path Crypt::GetMetaPath(const std::filesystem::path& inputPath)
    {
        return inputPath.string() + ".meta";
    }

    void Crypt::DeriveKeyFromPassword(const std::string& password, const byte* salt, CryptoPP::SecByteBlock& key)
    {
        CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf2;
        pbkdf2.DeriveKey(key, key.size(), 0,
            reinterpret_cast<const byte*>(password.data()), password.size(),
            salt, SALT_SIZE,
            10000); // Iterazioni
    }

    void Crypt::Encrypt(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath, const std::string& password)
    {
        CryptoPP::AutoSeededRandomPool rng;

        // Genera salt e iv
        byte salt[SALT_SIZE];
        rng.GenerateBlock(salt, sizeof(salt));

        byte iv[IV_SIZE];
        rng.GenerateBlock(iv, sizeof(iv));

        // Deriva chiave
        CryptoPP::SecByteBlock key(KEY_SIZE);
        DeriveKeyFromPassword(password, salt, key);

        // Leggi dati originali
        std::string plainData;
        CryptoPP::FileSource(inputPath.c_str(), true,
            new CryptoPP::StringSink(plainData)
        );

        // Calcola HMAC
        byte hmacDigest[CryptoPP::SHA256::DIGESTSIZE];
        CryptoPP::HMAC<CryptoPP::SHA256> hmac(key, key.size());
        hmac.Update(reinterpret_cast<const byte*>(plainData.data()), plainData.size());
        hmac.Final(hmacDigest);

        // Cifra e salva su disco
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption;
        encryption.SetKeyWithIV(key, key.size(), iv);

        CryptoPP::StringSource(plainData, true,
            new CryptoPP::StreamTransformationFilter(encryption,
                new CryptoPP::FileSink(outputPath.c_str())
            )
        );

        // Salva metadati: magic header + salt + iv + hmac
        std::ofstream meta(GetMetaPath(outputPath), std::ios::binary);
        meta.write("AXM1", 4); // Magic header
        meta.write(reinterpret_cast<const char*>(salt), SALT_SIZE);
        meta.write(reinterpret_cast<const char*>(iv), IV_SIZE);
        meta.write(reinterpret_cast<const char*>(hmacDigest), sizeof(hmacDigest));
        meta.close();
    }

    void Crypt::Decrypt(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath, const std::string& password)
    {
        std::filesystem::path metaPath = GetMetaPath(inputPath);
        std::ifstream meta(metaPath, std::ios::binary);
        if (!meta.is_open()) {
            std::cerr << "Metafile non trovato.\n";
            return;
        }

        // Magic header
        char magic[4];
        meta.read(magic, 4);
        if (std::string(magic, 4) != "AXM1") {
            std::cerr << "Il file non è stato cifrato con questo sistema.\n";
            return;
        }

        // Leggi salt, iv, hmac
        byte salt[SALT_SIZE];
        byte iv[IV_SIZE];
        byte expectedHmac[CryptoPP::SHA256::DIGESTSIZE];

        meta.read(reinterpret_cast<char*>(salt), SALT_SIZE);
        meta.read(reinterpret_cast<char*>(iv), IV_SIZE);
        meta.read(reinterpret_cast<char*>(expectedHmac), sizeof(expectedHmac));
        meta.close();

        // Deriva chiave
        CryptoPP::SecByteBlock key(KEY_SIZE);
        DeriveKeyFromPassword(password, salt, key);

        // Leggi dati cifrati
        std::string encryptedData;
        CryptoPP::FileSource(inputPath.c_str(), true,
            new CryptoPP::StringSink(encryptedData)
        );

        // Verifica HMAC
        byte actualHmac[CryptoPP::SHA256::DIGESTSIZE];
        CryptoPP::HMAC<CryptoPP::SHA256> hmac(key, key.size());
        hmac.Update(reinterpret_cast<const byte*>(encryptedData.data()), encryptedData.size());
        hmac.Final(actualHmac);

        if (0 != std::memcmp(expectedHmac, actualHmac, sizeof(expectedHmac))) {
            std::cerr << "Password errata o dati alterati. Decryption bloccata.\n";
            return;
        }

        // Decifra
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption;
        decryption.SetKeyWithIV(key, key.size(), iv);

        CryptoPP::StringSource(encryptedData, true,
            new CryptoPP::StreamTransformationFilter(decryption,
                new CryptoPP::FileSink(outputPath.c_str())
            )
        );
    }
}
