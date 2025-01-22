#pragma once

#include "Mirpch.h"

#include <open62541/client.h>
#include <open62541/client_config_default.h>

//#include <open62541/client_highlevel.h>

namespace Mir {
    class OpcuaClient
    {
    public:
        OpcuaClient(/* args */);
        ~OpcuaClient();
        void Connect(const char* endpointAdress);
        const std::vector<std::string>& GetNamespaces() { return m_Namespaces; };
        std::string GetEndpointAdress() { return std::string(m_UaEndpointAdress); }
        void GetOpcUaServerCurrentTime(int namespaceIndex);
        void GetNodesInObjectFolder();
        void GetNodesAtNodeID(UA_NodeId nodeId);
        void NodeIteratorTest();
        void WriteBooleanToNode(int namespaceIndex, UA_UInt32 nodeId, bool valueToWrite);
        void ReadValueFromNode(int namespaceIndex, UA_UInt32 nodeId);
        void BrowseNodesRecursive(const UA_NodeId& nodeId);
    private:
        void BrowseNamespaces();
        
        UA_Client* m_UaClient;
        const char* m_UaEndpointAdress;
        std::vector<std::string> m_Namespaces;
    private:
        static UA_StatusCode
        nodeIter(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId, void *handle) {
            if(isInverse)
                return UA_STATUSCODE_GOOD;
            UA_NodeId *parent = (UA_NodeId *)handle;
            printf("%u, %u --- %u ---> NodeId %u, %u\n",
                parent->namespaceIndex, parent->identifier.numeric,
                referenceTypeId.identifier.numeric, childId.namespaceIndex,
                childId.identifier.numeric);
            return UA_STATUSCODE_GOOD;
        }
    };
}


 