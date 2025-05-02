#pragma once
#include "Core/Base.h"
#include "Utils/File/FileManager.h"
#include <chrono>
#include <iomanip>

namespace AXIOM {

    template <class T, typename U>
    class Database {
    protected:
        virtual T Get() = 0;
        virtual bool Save(const T& db) = 0;

    public:
        virtual bool AddRecord(const U& record) = 0;
    };
}