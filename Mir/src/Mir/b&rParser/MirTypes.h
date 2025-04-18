#pragma once
#include "Mirpch.h"
#include <filesystem>
namespace Mir
{
    namespace CSV
    {
        struct Settings {
            std::filesystem::path targetFile;
            std::string header;
            char delimeter;
        };
        struct Data {
            std::vector<std::string> header;
            std::vector<std::vector<std::string>> content;
        };

        
    } // namespace CSV
} // namespace Mir

