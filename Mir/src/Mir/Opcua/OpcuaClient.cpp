#include "opcuaClient.h"

#include <iomanip>
#include <iostream>
#include <string_view>


#include <open62541pp/client.hpp>
#include <open62541pp/node.hpp>


namespace Mir {




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
        opcua::Node node(m_client, opcua::NodeId(3, "PLC"));
        //opcua::Node rootNode(m_client, opcua::ObjectId::RootFolder);
        // Call the browseChildren method and save the result to a variable
        std::vector<opcua::Node<opcua::Client>> children = node.browseChildren();


        // const auto& nodeId = children[10].id();

        // opcua::NamespaceIndex NamespaceIndex = nodeId.namespaceIndex();
        // opcua::NodeIdType NodeId = nodeId.identifierType();
        // auto indentifier = nodeId.identifier<uint32_t>();
        // std::string nodeIdString = nodeId.toString();
        // auto datattypepe = children[10].readNodeClass();
        


        // opcua::QualifiedName teest1 = children[10].readBrowseName();

            // Store nodes in the map
        for (auto& child : children) {
            auto childid = child.id();
            opcua::NodeIdType nodeidtype = childid.identifierType();
            auto childhandle = childid.handle();
            auto DisplayName4 = child.readDisplayName().typeIndex();
            auto displayname2 = child.readDisplayName().locale();
            
            Mir::NodeId nodeid(childid);

            auto BrowseName = child.readBrowseName(); // example 4, "publis_lenght"
            auto DisplayName = child.readDisplayName();
        
            opcua::LocalizedText Description;

            opcua::DataValue Data = child.readDataValue();
            auto datatype = Data.value().isArray();
            auto datatype2 = Data.value().isEmpty();
            auto datatype3 = Data.value().isScalar();
            auto datatype4 = Data.value().data();
            auto datatype6 = Data.value().type()->memSize;
            std::string teststring;
            if (Data.value().type() == &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]) {
                UA_LocalizedText *localizedText = static_cast<UA_LocalizedText*>(datatype4);
                teststring = std::string(reinterpret_cast<const char*>(localizedText->text.data), localizedText->text.length);
            } else {
                teststring = std::string(static_cast<const char*>(datatype4), 32);
            }

            Mir::NodeValue nodevalue(child.readDataValue());


            
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
                nodevalue
            };
            nodeMap[childid.toString()] = data;
        }


        // opcua::Bitmask<opcua::AccessLevel> AccesLevel = children[6].readAccessLevel();
        // //std::vector<uint32_t> ArrayDimensions = children[4].readArrayDimensions();
        // opcua::QualifiedName BrowseName = children[6].readBrowseName();
        // opcua::NodeId DataType = children[6].readDataType();
        // //opcua::Variant DataTypeDefinition = children[6].readDataTypeDefinition();
        // opcua::DataValue DataValue = children[6].readDataValue();
        //opcua::LocalizedText Description = children[6].readDescription();

        // for (opcua::Node<opcua::Client>& child : children) {
        //     std::cout << child. << std::endl;
        // }



    }

    OpcuaClient::~OpcuaClient(){
    }

}
