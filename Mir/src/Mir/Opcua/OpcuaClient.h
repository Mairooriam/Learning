#pragma once

#include "Mirpch.h"

#include <open62541pp/open62541pp.hpp>
#include <map>
#include <open62541pp/client.hpp>
#include <open62541pp/node.hpp>
namespace Mir {

    

    // Function to get the identifier based on NodeIdType
    std::variant<uint32_t, opcua::String, opcua::Guid, opcua::ByteString> getIdentifierMir(const opcua::NodeIdType& type, const opcua::NodeId& datatype);


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

    //------------------------ NODE NodeId ------------------------//
    struct NodeId {
        opcua::NodeId nodeIdHandle;
        uint16_t NamespaceIndex;
        std::string identifierType;
        std::variant<uint32_t, std::string, opcua::Guid, opcua::ByteString> identifier;
    
        // Default constructor
        NodeId()
            : nodeIdHandle(), NamespaceIndex(0), identifierType(""), identifier(uint32_t(0)) {}
    
        NodeId(const opcua::NodeId& nodeId)
            : nodeIdHandle(nodeId),
                NamespaceIndex(static_cast<uint16_t>(nodeId.namespaceIndex())),
                identifierType(std::string(toString(nodeId.identifierType()))),
                identifier(extractIdentifier(nodeId)) {}
    
    private:
        static std::variant<uint32_t, std::string, opcua::Guid, opcua::ByteString> extractIdentifier(const opcua::NodeId& nodeId) {
            switch (nodeId.identifierType()) {
                case opcua::NodeIdType::Numeric:
                    return nodeId.identifier<uint32_t>(); // TODO: FIX THESE LIKE THE STRING
                case opcua::NodeIdType::String:
                    return std::string(nodeId.identifier<opcua::String>().data(), nodeId.identifier<opcua::String>().length()); // will call identifier two times not good? might be performance problem
                case opcua::NodeIdType::Guid:
                    return nodeId.identifier<opcua::Guid>(); // TODO: FIX THESE LIKE THE STRING
                case opcua::NodeIdType::ByteString:
                    return nodeId.identifier<opcua::ByteString>(); // TODO: FIX THESE LIKE THE STRING
                default:
                    throw std::runtime_error("Unsupported NodeIdType");
            }
        }
    };


    //------------------------ NODE VALUE ------------------------//
    struct NodeValue {
        std::string SourceTimestamp;
        uint16_t SourcePicoseconds;
        std::string ServerTimestamp;
        uint16_t ServerPicoseconds;
        std::string StatusCode;
        std::string value;
    
        // Default constructor
        NodeValue() 
            : SourceTimestamp(""), SourcePicoseconds(0), ServerTimestamp(""), ServerPicoseconds(0), StatusCode("empty"), value() {}
    
        //Parameterized constructor
        NodeValue(const opcua::DataValue& dataValue)
            : SourceTimestamp(dataValue.sourceTimestamp().format("%Y-%m-%d %H:%M:%S")), 
                SourcePicoseconds(dataValue.sourcePicoseconds()), 
                ServerTimestamp(dataValue.serverTimestamp().format("%Y-%m-%d %H:%M:%S")), 
                ServerPicoseconds(dataValue.serverPicoseconds()), 
                StatusCode(std::string(dataValue.status().name()))
                {
                opcua::Variant data = dataValue.value();
                auto dataType = data.type();
                if (dataType == &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]) {
                    UA_LocalizedText *localizedText = static_cast<UA_LocalizedText*>(data.data());
                    value = std::string(reinterpret_cast<const char*>(localizedText->text.data), localizedText->text.length);
                } else if (dataType == &UA_TYPES[UA_TYPES_INT32]){
                    value = std::to_string(*static_cast<int32_t*>(data.data()));
                } else if (data.isScalar() | data.isArray()){
                    value = data.to<std::string>();} 
                else {
                    value = "EMPTY";
                }
                }
    };
    
    //------------------------ NodeDataType ------------------------//
    struct NodeDataType {
            //std::string datatype;
            size_t namespaceindex;
            std::string_view identifierType;
            std::variant<uint32_t, opcua::String, opcua::Guid, opcua::ByteString> identifier;

            NodeDataType()
                : namespaceindex(0), identifierType(""), identifier(uint32_t(0)) {}
// datatype("")
            NodeDataType(opcua::NodeId datatype)
                : //datatype(node.d),
                namespaceindex(datatype.namespaceIndex()), 
                identifierType(toString(datatype.identifierType())) 
                {
                    identifier = getIdentifierMir(datatype.identifierType(),datatype);
                } 
        };
    //------------------------ NodeDataType ------------------------//


    //------------------------ NODE VALUE ------------------------//
    struct NodeData {
    Mir::NodeId NodeId;
    std::string NodeClass;
    std::string BrowseName;
    std::string DisplayName;
    std::string Description;
    Mir::NodeValue value;
    Mir::NodeDataType datatype;
    };
    //------------------------ NODE VALUE ------------------------//

    class OpcuaClient
    {
    public:
        OpcuaClient(std::string_view serverAdress);
        ~OpcuaClient();
        void browseNode(opcua::Node<opcua::Client>& node, size_t depth);
        const std::vector<std::string>& GetNamespaces() { return m_Namespaces; };
    private:
        opcua::Client m_client;
        std::vector<std::string> m_Namespaces;
        std::map<std::string, NodeData> nodeMap; // Replace with actual types for NodeKey and NodeData
    private:
    };
}


 