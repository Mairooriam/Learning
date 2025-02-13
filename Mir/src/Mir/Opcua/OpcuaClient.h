#pragma once

#include "Mirpch.h"

#include <open62541pp/open62541pp.hpp>
#include <map>
#include <open62541pp/client.hpp>
#include <open62541pp/node.hpp>
namespace Mir {


    struct NodeId{
        opcua::NodeId nodeIdHandle;
        uint16_t NamespaceIndex;
        std::string identifierType;
        uint32_t identifier;
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


 