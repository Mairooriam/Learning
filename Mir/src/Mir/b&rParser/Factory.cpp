#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <sstream>
namespace Mir {


}

int main(){
    


    struct brStructData {
        std::string name;
        std::string type;
        std::string value;
        std::string comment;

        std::string toString() const {
            std::stringstream ss;
            ss << name;
            if (!type.empty()) ss << " : " << type;
            if (!value.empty()) ss << " := " << value;
            if (!comment.empty()) ss << "; (*" << comment << "*)";
            return ss.str();
        }
    };

    struct brStructNode {
        size_t id;
        std::string name;
        std::vector<brStructData> data;

        std::string toString() const {
            std::stringstream ss;
            ss << name << " :\t" << "STRUCT";
            for (const auto& d : data) {
                ss << "\n\t\t " << d.toString();
            }
            return ss.str();
        }
    };

    struct brStructCollection {
        std::string comment;
        std::vector<brStructNode> nodes;
    
        std::string toString() const {
            std::stringstream ss;
            if (!comment.empty()) {
                ss << "(*" << comment << "*)";
            }
    
            ss << "\nTYPE";
            for (const auto& node : nodes) {
                ss << "\n" << node.toString();
            }
            ss << "\nEND_TYPE\n";
            
            return ss.str();
        }
    };

    struct brTyp {
        std::vector<brStructCollection> typ;
        
        void push_back(const brStructCollection& collection) {
            typ.push_back(collection);
        }

        std::string toString() const {
            std::stringstream ss;
            for (const auto& collection : typ) {
                ss << collection.toString();
            }
            return ss.str();
        }
    };

    // Create multiple nodes
    std::vector<brStructNode> nodes;
    for (size_t i = 0; i < 3; i++) {
        brStructNode node;
        node.name = "Node1" + std::to_string(i);
        node.id = i;
        
        // Create multiple data entries for each node
        for (size_t j = 0; j < 2; j++) {
            brStructData data;
            data.comment = "datakommentti" + std::to_string(j);
            data.name = "DataName" + std::to_string(j);
            data.type = "Bool" + std::to_string(j);
            data.value = "5" + std::to_string(j);
            node.data.push_back(data);
        }
        
        nodes.push_back(node);
        // Print each node
    }
    brStructCollection brSC;
    brSC.comment = "Kommentti";
    brSC.nodes.push_back(nodes[0]);
    brSC.nodes.push_back(nodes[1]);


    brTyp brTyp;
    brTyp.push_back(brSC);
    brTyp.typ.push_back(brSC);


    std::cout << brTyp.toString() << "\n";

    
}