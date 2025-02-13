#pragma once

#include "Mirpch.h"

#include <open62541pp/open62541pp.hpp>
#include <map>
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

    struct NodeData {
        Mir::NodeId NodeId;
        std::string NodeClass;
        std::string BrowseName;
        std::string DisplayName;
        std::string Description;
        //opcua::DataValue value;
        opcua::Variant value;
    };


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


 