#include "Sorting.h"
#include <unordered_map>
#include "Utils/Utils.h"
namespace Mir {
    using namespace Types;
    std::vector<StructDefinition> groupByField(const StructDefinition& _sDef, std::function<std::string(const MemberDefinition&)> getField,
                            std::function<std::string(const std::string&)> strProcessingFunc) {
        std::vector<StructDefinition> result;
        
        std::unordered_map<std::string, std::vector<MemberDefinition>> groupedMembers;
        
        for (const auto &member : _sDef.members) {
            std::string temp = getField(member);
            std::string fieldValue = strProcessingFunc(temp);
            groupedMembers[fieldValue].push_back(member);
        }
        
        for (const auto &[fieldValue, members] : groupedMembers) {
            StructDefinition newStruct;
            newStruct.name = _sDef.name;  
            newStruct.comment = fieldValue;
            newStruct.members = members;
        
            result.push_back(newStruct);
        }
        
        return result;
    }

}
