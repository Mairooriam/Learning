#pragma once
#include "Mirpch.h"
#include <filesystem>
namespace Mir
{
    namespace Types{
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
    struct MemberDefinition {
        std::string name;
        std::string type;
        std::string value;
        std::string comment;
    };

    struct StructDefinition {
        std::string name;
        std::string comment;
        std::vector<MemberDefinition> members;
    };

    struct TypeDefinition {
        std::string comment;
        std::vector<StructDefinition> structs;
    };

    struct VarConfigMemberDefinition {
        std::string processVar;  // The process variable components
        std::string ioAdress;
        std::string hardwareType;  // Data type (e.g., "%IW")
        std::string comment;       // Optional comment (e.g., "Generated with Mir")
    };

    struct VarConfigDefinition {
        std::string comment;
        std::vector<VarConfigMemberDefinition> members;
    };
}
} // namespace Mir

