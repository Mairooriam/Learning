#include "opcuaClient.h"

#include <iomanip>
#include <iostream>
#include <string_view>


#include <open62541pp/client.hpp>
#include <open62541pp/node.hpp>


namespace Mir {

    enum UaDataTypes1 {
        Boolean = 1,           // A two-state logical value (true or false).
        SByte,                 // An integer value between −128 and 127 inclusive.
        Byte,                  // An integer value between 0 and 255 inclusive.
        Int16,                 // An integer value between −32 768 and 32 767 inclusive.
        UInt16,                // An integer value between 0 and 65 535 inclusive.
        Int32,                 // An integer value between −2 147 483 648 and 2 147 483 647 inclusive.
        UInt32,                // An integer value between 0 and 4 294 967 295 inclusive.
        Int64,                 // An integer value between −9 223 372 036 854 775 808 and 9 223 372 036 854 775 807 inclusive.
        UInt64,                // An integer value between 0 and 18 446 744 073 709 551 615 inclusive.
        Float,                 // An IEEE single precision (32 bit) floating point value.
        Double,                // An IEEE double precision (64 bit) floating point value.
        String,                // A sequence of Unicode characters.
        DateTime,              // An instance in time.
        Guid,                  // A 16-byte value that can be used as a globally unique identifier.
        ByteString,            // A sequence of octets.
        XmlElement,            // An XML element.
        NodeId2,               // An identifier for a node in the address space of an OPC UA Server.
        ExpandedNodeId,        // A NodeId that allows the namespace URI to be specified instead of an index.
        StatusCode,            // A numeric identifier for an error or condition that is associated with a value or an operation.
        QualifiedName,         // A name qualified by a namespace.
        LocalizedText,         // Human readable text with an optional locale identifier.
        ExtensionObject,       // A structure that contains an application specific data type that may not be recognized by the receiver.
        DataValue,             // A data value with an associated status code and timestamps.
        Variant,               // A union of all of the types specified above.
        DiagnosticInfo         // A structure that contains detailed error and diagnostic information associated with a StatusCode.
    };

    std::string toString(UaDataTypes1 type) {
        switch (type) {
            case Boolean: return "Boolean";
            case SByte: return "SByte";
            case Byte: return "Byte";
            case Int16: return "Int16";
            case UInt16: return "UInt16";
            case Int32: return "Int32";
            case UInt32: return "UInt32";
            case Int64: return "Int64";
            case UInt64: return "UInt64";
            case Float: return "Float";
            case Double: return "Double";
            case String: return "String";
            case DateTime: return "DateTime";
            case Guid: return "Guid";
            case ByteString: return "ByteString";
            case XmlElement: return "XmlElement";
            case NodeId2: return "NodeId2";
            case ExpandedNodeId: return "ExpandedNodeId";
            case StatusCode: return "StatusCode";
            case QualifiedName: return "QualifiedName";
            case LocalizedText: return "LocalizedText";
            case ExtensionObject: return "ExtensionObject";
            case DataValue: return "DataValue";
            case Variant: return "Variant";
            case DiagnosticInfo: return "DiagnosticInfo";
            default: return "Unknown";
        }
    }

    // Define the function
    std::variant<uint32_t, opcua::String, opcua::Guid, opcua::ByteString> getIdentifierMir(const opcua::NodeIdType& type, const opcua::NodeId& datatype) {
        switch (type) {
            case opcua::NodeIdType::Numeric:
                return datatype.identifier<uint32_t>();
            case opcua::NodeIdType::String:
                return datatype.identifier<opcua::String>();
            case opcua::NodeIdType::Guid:
                return datatype.identifier<opcua::Guid>();
            default:
                throw std::invalid_argument("Unknown identifier type");
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


            // NODE VALUUE
            Mir::NodeValue nodevalue(child.readDataValue());
            Mir::NodeDataType nodeDataType(child.readDataType());

            auto datatype = child.readDataType();
            auto dasda42 = child.readDataValue().typeIndex();
           
            auto test222 = datatype.namespaceIndex();
            auto test333 = datatype.identifierType();
            
            std::string_view test5455 = toString(datatype.identifierType());
            std::variant<uint32_t, opcua::String, opcua::Guid, opcua::ByteString> identifier;

            try {
                identifier = getIdentifierMir(test333, datatype);
                //auto tset11 = toString(UaDataTypes1(identifier));
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << '\n';
            }

            // switch (test333) {
            //     case opcua::NodeIdType::Numeric:
            //         identifier = datatype.identifier<uint32_t>();
            //         break;
            //     case opcua::NodeIdType::String:
            //         identifier = datatype.identifier<opcua::String>();
            //         break;
            //     case opcua::NodeIdType::Guid:
            //         identifier = datatype.identifier<opcua::Guid>();
            //         break;
            //     default:
            //         std::cout << "Unknown identifier type\n";
            //         break;
            // }
            


           
            
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
                nodevalue,
                nodeDataType
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
