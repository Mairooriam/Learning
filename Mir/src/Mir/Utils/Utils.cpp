#include "Utils.h"

#include "Log.h"

namespace Mir {
    namespace Utils {
        namespace File {
            std::string getFileExtension(const std::filesystem::path& filepath) {
                return filepath.extension().string();
            }

            bool hasExtension(const std::filesystem::path& filepath, const std::string& extension) {
                std::string fileExt = getFileExtension(filepath);

                // Convert both to lowercase for case-insensitive comparison
                std::transform(fileExt.begin(), fileExt.end(), fileExt.begin(),
                               [](unsigned char c) { return std::tolower(c); });

                std::string extensionLower = extension;
                std::transform(extensionLower.begin(), extensionLower.end(), extensionLower.begin(),
                               [](unsigned char c) { return std::tolower(c); });

                // Add the dot if missing in the comparison extension
                if (!extensionLower.empty() && extensionLower[0] != '.') {
                    extensionLower = "." + extensionLower;
                }

                return fileExt == extensionLower;
            }

            std::ifstream openFile(const std::filesystem::path& filepath) {
                std::ifstream file(filepath);

                if (!file.is_open()) {
                    std::string errorMsg = "Failed to open file: " + filepath.string();
                    if (std::error_code ec; !std::filesystem::exists(filepath, ec)) {
                        errorMsg += " [file does not exist]";
                    } else if (!std::filesystem::is_regular_file(filepath, ec)) {
                        errorMsg += " [not a regular file]";
                    } else {
                        errorMsg += " [possible permission issue]";
                    }
                    MIR_CORE_ERROR("Error opening file: {0}", errorMsg);
                }

                return file;
            }

            std::vector<std::string> readLines(std::istream& stream) {
                std::vector<std::string> lines;
                std::string line;

                while (std::getline(stream, line)) {
                    if (!line.empty() && line.back() == '\r') {
                        line.pop_back();
                    }
                    lines.push_back(line);
                }

                return lines;
            }

            std::vector<std::string> readLines(const std::filesystem::path& filepath) {
                std::ifstream file = openFile(filepath);

                if (!file.is_open()) {
                    return {};
                }

                return readLines(file);
            }

            std::string readFile(std::istream& stream) {
                if (!stream.good()) {
                    return "";
                }

                return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
            }

            std::string readFile(const std::filesystem::path& filepath) {
                std::ifstream file = openFile(filepath);

                if (!file.is_open()) {
                    return "";
                }

                return readFile(file);
            }

        }  // namespace File
        namespace Text {
            std::vector<std::string> splitLine(const std::string& line, char delimiter) {
                std::vector<std::string> tokens;
                std::string token;
                bool inQuotes = false;

                for (char c : line) {
                    if (c == '"') {
                        inQuotes = !inQuotes;
                    } else if (c == delimiter && !inQuotes) {
                        tokens.push_back(token);
                        token.clear();
                    } else {
                        token += c;
                    }
                }

                tokens.push_back(token);

                return tokens;
            }
        }  // namespace Text

    }  // namespace Utils
}  // Namespace Mir