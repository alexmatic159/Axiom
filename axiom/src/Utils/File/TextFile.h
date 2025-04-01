#pragma once
#include "Logger/Logger.h"

#include <string>
#include <fstream>
#include <vector>

namespace AXIOM {

    class TextFile 
    {
    public:
        // Legge l'intero file
        static std::string Read(const std::string& filePath);

        // Scrive testo nel file (sovrascrive)
        static void Write(const std::string& filePath, const std::string& content);

        // Aggiunge testo al file
        static void Append(const std::string& filePath, const std::string& content);
    };
}

