#pragma once
#include "Core/Base.h"
#include "Utils/File/FileManager.h"

#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>

namespace AXIOM {
    class Crypt
    {
    private:
        static constexpr size_t KEY_SIZE = CryptoPP::AES::DEFAULT_KEYLENGTH;
        static constexpr size_t SALT_SIZE = 16;
        static constexpr size_t IV_SIZE = CryptoPP::AES::BLOCKSIZE;

        std::filesystem::path GetMetaPath(const std::filesystem::path& inputPath);
        
        void DeriveKeyFromPassword(const std::string& password, const byte* salt, CryptoPP::SecByteBlock& key);

    public:

        void Encrypt(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath, const std::string& password);

        void Decrypt(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath, const std::string& password);
    };
}