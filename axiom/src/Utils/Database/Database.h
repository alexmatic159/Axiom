#pragma once
#include "Core/Base.h"
#include <chrono>
#include <iomanip>
#include "Utils/File/FilePath.h"

namespace AXIOM {

    template <class T, typename U>
    class Database {
    protected:
        virtual T Get() = 0;
        virtual bool Save(const T& db) = 0;

    public:
        virtual bool AddRecord(const U& record) = 0;
        
        // virtual bool EditRecord();

    };
}