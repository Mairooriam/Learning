#include "Serializers.h"
namespace Mir
{
    
    std::string toString(Types::MemberDefinition mDef) {
        std::stringstream ss;
    
        ss << mDef.name << " : ";
        if (!mDef.value.empty()) {
            ss << mDef.type << " := ";
            ss << mDef.value << "; ";
        } else {
            ss << mDef.type << "; ";
        }
    
        if (!mDef.comment.empty()) {
            ss << "(*" << mDef.comment << "*)\n";
        } else {
            ss << "\n";
        }
    
        return ss.str();
    }
    
    std::string toString(Types::StructDefinition sDef) {
        std::stringstream ss;
    
        ss << "\t" << sDef.name << ":\t STRUCT  ";
        if (!sDef.comment.empty()) {
            ss << "(*" << sDef.comment << "*)" << "\n";
        } else {
            ss << "\n";
        }
    
        for (const auto& member : sDef.members) {
            ss << "\t\t" << toString(member);
        }
        ss << "END_STRUCT\n";
    
        return ss.str();
    }
    
    std::string toString(Types::TypeDefinition tDef) {
        std::stringstream ss;
        if (!tDef.comment.empty()) {
            ss << "(*" << tDef.comment << "*)\n";
        }
    
        ss << "TYPE";
        for (const auto& structDef : tDef.structs) {
            ss << "\n" << toString(structDef);
        }
        ss << "END_TYPE";
    
        return ss.str();
    }
    
} // namespace Mir

