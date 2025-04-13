#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
namespace Mir {
namespace Utils{
    namespace File
    {
        std::ifstream openFile(const std::filesystem::path &filepath);

        
        std::vector<std::string> readLines(const std::filesystem::path& filepath);
        std::string readFile(const std::filesystem::path& filepath);
        std::vector<std::string> readLines(std::istream& stream);
        std::string readFile(std::istream& stream);
        
        std::string getFileExtension(const std::filesystem::path& filepath);
        bool hasExtension(const std::filesystem::path& filepath, const std::string& extension);
    } // namespace File
    namespace Text
    {
        std::vector<std::string> splitLine(const std::string& line, char delimeter = ' ');
    } // namespace String
    
} // namespace Utils
} // Namepsace Mir