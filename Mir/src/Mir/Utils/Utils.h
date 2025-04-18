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
        std::string findNumbers(const std::string& str);
        void toLowerCase(std::string& str);
        std::string toLowerCase(const std::string& str);
        // Modifies the original & returns numbers as string.
        std::string findNumbersAConsume(std::string& str);
        std::pair<std::string, std::string> splitCharsFromNums(const std::string& str);
        std::string filterNumbers(const std::string& str);
        void replacePlaceholderIf(std::string& str, const std::string& placeholder, const std::string& replaceStr, const char oChar = '{', const char cChar = '}' );
    } // namespace String
    

    template<typename MapType, typename KeyType>
    bool mapContains(const MapType& map, const KeyType& key)
    {
        return map.find(key) != map.end();
    }





} // namespace Utils
} // Namepsace Mir