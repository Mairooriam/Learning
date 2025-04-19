#include "Types/MirTypes.h"

namespace Mir {
    std::vector<Types::StructDefinition> groupByField(const Types::StructDefinition &_sDef,
        std::function<std::string(const Types::MemberDefinition &)> getField,
        std::function<std::string(const std::string& )>);
}




