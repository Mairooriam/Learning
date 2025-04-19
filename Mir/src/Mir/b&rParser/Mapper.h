#pragma once
#include "Mirpch.h"
#include <nlohmann/json.hpp>
#include "Types/MirTypes.h"
using json = nlohmann::json;
namespace Mir
{

    
    class Mapper
    {
    private:
        json m_config;
        Types::CSV::Data m_data;
        bool isValidConfig(const json& config);
        std::unordered_map<std::string, std::string> processCard(const std::string& card);
        std::string processTemplate(       
            const std::string& templateStr,          // for example                         {location}_{card}_randomtext
            const std::vector<std::string>& headers, // csv headers "available templates"   { "location", "card"        etc. }
            const std::vector<std::string>& row   // the actual row to process.             { "CC1"     , "Analoginput" etc. }
        );
        public:
        Mapper(const json& config, const Types::CSV::Data& data);
        ~Mapper();
        Types::StructDefinition process();
        
        
    };


    
} // namespace Mir
