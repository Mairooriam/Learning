#include "opcuaClient.h"

#include <iomanip>
#include <iostream>
#include <string_view>


#include <open62541pp/client.hpp>
#include <open62541pp/node.hpp>



namespace Mir {

    /// Get name of NodeIdType.
    constexpr std::string_view toString(opcua::NodeIdType nodeIdType) {
        switch (nodeIdType) {
        case opcua::NodeIdType::Numeric:
            return "Numeric";
        case opcua::NodeIdType::String:
            return "String";
        case opcua::NodeIdType::Guid:
            return "Guid";
        case opcua::NodeIdType::ByteString:
            return "ByteString";
        default:
            return "Unknown";
        }
    }
    /// Get name of VariableType.
    constexpr std::string_view toString(opcua::NodeClass variableType) {
        switch (variableType) {
        case opcua::NodeClass::Unspecified:
            return "Unspecified";
        case opcua::NodeClass::Object:
            return "Object";
        case opcua::NodeClass::Variable:
            return "Variable";
        case opcua::NodeClass::Method:
            return "Method";
        case opcua::NodeClass::ObjectType:
            return "ObjectType";
        case opcua::NodeClass::VariableType:
            return "VariableType";
        case opcua::NodeClass::ReferenceType:
            return "ReferenceType";
        case opcua::NodeClass::DataType:
            return "DataType";
        case opcua::NodeClass::View:
            return "View";
        default:
            return "Unknown";
        }
    }

    // Separate definition for recursion
    void printNodeTree(opcua::Node<opcua::Client>& node, int indent);

    // Browse and print the server's node tree recursively:
    // - Objects (Object)
    //   - Server (Object)
    //     - Auditing (Variable)
    //     - ServiceLevel (Variable)
    //     - NamespaceArray (Variable)
    //     - ServerArray (Variable)
    //     - ServerRedundancy (Object)
    //       - RedundancySupport (Variable)
    //     - VendorServerInfo (Object)
    // ...

    void printNodeTree(opcua::Node<opcua::Client>& node, int indent) {  
        auto children = node.browseChildren();
        std::cout << children[0].readBrowseName().name() << "\n";
        std::cout << children[1].readBrowseName().name() << "\n";
        std::cout << children[2].readBrowseName().name() << "\n";
        for (auto&& child : node.browseChildren()) {
            std::cout << std::setw(indent) << "- " << child.readBrowseName().name() << " ("
                    << toString(child.readNodeClass()) << ")\n";
            printNodeTree(child, indent + 2);
        }
    }

    void OpcuaClient::browseNode(opcua::Node<opcua::Client>& node, size_t depth){
        

        auto children = node.browseChildren();

        for (size_t i = 0; i < children.size(); ++i){
            std::cout  << children[i].readBrowseName().name() << " ("
                    << toString(children[i].readNodeClass()) << ")\n";
            // Recursively browse the children nodes with decreased depth
            browseNode(children[i], depth - 1);
        }

        
    }



    

    OpcuaClient::OpcuaClient(std::string_view serverAdress){
        
        
        m_client.connect(serverAdress);

        //opcua::Node nodeRoot(m_client, opcua::ObjectId::RootFolder);
        //browseNode(nodeRoot, 5);

        std::vector<std::string> test = m_client.namespaceArray();
        // Create a node with a specific NodeId instead of the RootFolder
        opcua::Node node(m_client, opcua::NodeId(4, 2));
        opcua::Node rootNode(m_client, opcua::ObjectId::RootFolder);
        // Call the browseChildren method and save the result to a variable
        std::vector<opcua::Node<opcua::Client>> children = rootNode.browseChildren();


        const auto& nodeId = children[10].id();

        opcua::NamespaceIndex NamespaceIndex = nodeId.namespaceIndex();
        opcua::NodeIdType NodeId = nodeId.identifierType();
        auto indentifier = nodeId.identifier<uint32_t>();
        std::string nodeIdString = nodeId.toString();
        auto datattypepe = children[10].readNodeClass();
        


        opcua::QualifiedName teest1 = children[10].readBrowseName();

            // Store nodes in the map
        for (auto& child : children) {
            auto childid = child.id();
            opcua::NodeIdType nodeidtype = childid.identifierType();
            auto childhandle = childid.handle();
            auto DisplayName4 = child.readDisplayName().typeIndex();
            auto displayname2 = child.readDisplayName().locale();

            Mir::NodeId nodeid{
                childid,
                static_cast<uint16_t>(childid.namespaceIndex()),
                std::string(toString(nodeidtype)),
                childid.identifier<uint32_t>(), // will cause bug at somepoinmt not considering other types

            };

            auto BrowseName = child.readBrowseName(); // example 4, "publis_lenght"
            auto DisplayName = child.readDisplayName();
            opcua::LocalizedText Description;

            opcua::Variant DataValue;

            try { 
                DataValue = child.readValue();
                
                // // Check the data type and cast accordingly
                // if (DataValue.type() == opcua::VariantType::Int32) {
                //     int32_t value = *static_cast<int32_t*>(DataValue.data());
                //     // Use the value as needed
                // } else if (DataValue.type() == opcua::VariantType::Double) {
                //     double value = *static_cast<double*>(DataValue.data());
                //     // Use the value as needed
                // } else if (DataValue.type() == opcua::VariantType::String) {
                //     std::string value = *static_cast<std::string*>(DataValue.data());
                //     // Use the value as needed
                // }
                // Add more type checks and casts as needed
            } catch (const std::exception& e) {
                DataValue = opcua::LocalizedText("", e.what());
            }

            
            try {
                Description = child.readDescription();
            } catch (const std::exception& e) {
                Description = opcua::LocalizedText("", e.what());
            }
            

            NodeData data{
                nodeid,
                std::string(toString(child.readNodeClass())),
                std::to_string(BrowseName.namespaceIndex()) + " ," + std::string(BrowseName.name()),
                std::string(DisplayName.locale()) + std::string(DisplayName.text().data(), DisplayName.text().length()), // will cuase problems locale not showintg correctly
                std::string(Description.text().data(), Description.text().length()),
                DataValue
            };
            nodeMap[childid.toString()] = data;
        }


        opcua::Bitmask<opcua::AccessLevel> AccesLevel = children[6].readAccessLevel();
        //std::vector<uint32_t> ArrayDimensions = children[4].readArrayDimensions();
        opcua::QualifiedName BrowseName = children[6].readBrowseName();
        opcua::NodeId DataType = children[6].readDataType();
        //opcua::Variant DataTypeDefinition = children[6].readDataTypeDefinition();
        opcua::DataValue DataValue = children[6].readDataValue();
        //opcua::LocalizedText Description = children[6].readDescription();

        // for (opcua::Node<opcua::Client>& child : children) {
        //     std::cout << child. << std::endl;
        // }



    }

    OpcuaClient::~OpcuaClient(){
    }

}
