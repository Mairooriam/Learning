#include <string>
#include <unordered_map>
#include <iostream>
#include <variant>

enum class typTypes {
    None = 0,
    Collection,
    Struct,
    Value
};

struct CollectionInfo{
    int i;
    std::string name;
};

struct StructInfo{
    int i;
    int j;
    std::string name;
};

struct ValueInfo{
    int i;
    int j;
    int k;
    std::string name;
};

std::string typTypeToString(typTypes type) {
    switch (type) {
        case typTypes::None: return "None";
        case typTypes::Collection: return "Collection";
        case typTypes::Struct: return "Struct";
        case typTypes::Value: return "Value";
        default: return "Unknown";
    }
}

// Move the class definition outside of main
struct selectableElement {
    typTypes type = typTypes::None;
    std::variant<std::monostate, CollectionInfo, StructInfo, ValueInfo> data;
    
    selectableElement() = default;

    // Template constructor is allowed here
    template<typename T>
    selectableElement(typTypes type_, T&& data_)
        : type(type_), data(std::forward<T>(data_)) {}
    bool isType(typTypes _type) { return _type == type; }
    void toString() {
        std::cout << "Type: " << typTypeToString(type) << " - ";
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>)
                std::cout << "No data";
            else if constexpr (std::is_same_v<T, CollectionInfo>)
                std::cout << "Collection: " << arg.name << "i:" << arg.i;
            else if constexpr (std::is_same_v<T, StructInfo>)
                std::cout << "Struct: " << arg.name << ", i: " << arg.i << ", j: " << arg.j;
            else if constexpr (std::is_same_v<T, ValueInfo>)
                std::cout << "Value at " << arg.name << ", i: " << arg.i << ", j: " << arg.j << ", k: "<< arg.k;
        }, data);
        std::cout << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    size_t id1 = 123489;
    size_t id2 = 98864321;
    size_t id3 = 9832131;

    std::unordered_map<size_t, selectableElement> testmap;
    
    testmap[id1] = selectableElement(typTypes::Collection, CollectionInfo{0, "MyCollection"});
    testmap[id2] = selectableElement(typTypes::Struct, StructInfo{0,1, "MyStruct"});
    testmap[id3] = selectableElement(typTypes::Value, ValueInfo{0,1,5,"MyValue"});
    
    for (auto &&value : testmap)
    {
        std::cout << "ID:" << value.first << "\n";
        value.second.toString();
        if (value.second.isType(typTypes::Struct))
        {
            StructInfo info = std::get<StructInfo>(value.second.data);
            std::cout << "printing from if i: " << info.i << "j:" << info.j << "\n";
        }
        
        
    }
    
    std::string input;
    std::cout << "Press Enter to continue...";
    std::getline(std::cin, input); 

    return 0;
}