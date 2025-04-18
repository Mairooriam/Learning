#pragma once
#include "Mirpch.h"
#include <nlohmann/json.hpp>
#include "MirTypes.h"
using json = nlohmann::json;
namespace Mir
{

    
    class Mapper
    {
    private:
        json m_config;

        bool isValidConfig(const json& config);
        std::unordered_map<std::string, std::string> processCard(const std::string& card);
        std::string processTemplate(       
            const std::string& templateStr,          // for example                         {location}_{card}_randomtext
            const std::vector<std::string>& headers, // csv headers "available templates"   { "location", "card"        etc. }
            const std::vector<std::string>& row   // the actual row to process.             { "CC1"     , "Analoginput" etc. }
        );
    public:
        Mapper(const json& config);
        ~Mapper();
        std::string process(CSV::Data data);

         
    };
} // namespace Mir
