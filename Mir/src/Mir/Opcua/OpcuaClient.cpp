#include "opcuaClient.h"

#include <open62541/client_highlevel.h>

namespace Mir {
    OpcuaClient::OpcuaClient(){
        m_UaClient = UA_Client_new();
        UA_ClientConfig_setDefault(UA_Client_getConfig(m_UaClient));


    }

    void OpcuaClient::Connect(const char* endpointAdress){
        // Connect to the server
        UA_StatusCode status = UA_Client_connect(m_UaClient, endpointAdress);
        if (status != UA_STATUSCODE_GOOD) {
            UA_Client_delete(m_UaClient);
            MIR_CORE_TRACE("Failed to connect to server: {0}", UA_StatusCode_name(status));
        } else{
            m_UaEndpointAdress = endpointAdress;
            MIR_CORE_TRACE("Connected to OPC UA Server at: {0}", endpointAdress);
            BrowseNamespaces();

        }   
    }

    void OpcuaClient::BrowseNamespaces() {
        UA_Variant value;
        UA_Variant_init(&value);

        UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY);
        UA_StatusCode status = UA_Client_readValueAttribute(m_UaClient, nodeId, &value);
        if (status == UA_STATUSCODE_GOOD && UA_Variant_hasArrayType(&value, &UA_TYPES[UA_TYPES_STRING])) {
            size_t arrayLength = value.arrayLength;
            UA_String* namespaceArray = (UA_String*)value.data;

            for (size_t i = 0; i < arrayLength; ++i) {
                m_Namespaces.push_back(std::string((char*)namespaceArray[i].data, namespaceArray[i].length));
            }
        }
        UA_Variant_clear(&value);
    }

    void OpcuaClient::GetOpcUaServerCurrentTime(int namespaceIndex) {
        UA_Variant value; /* Variants can hold scalar values and arrays of any type */
        UA_Variant_init(&value);
        UA_StatusCode retval;
        /* NodeId of the variable holding the current time */
        const UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
        retval = UA_Client_readValueAttribute(m_UaClient, nodeId, &value);

        if(retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DATETIME])) {
            UA_DateTime raw_date = *(UA_DateTime *) value.data;
            UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
            MIR_CORE_INFO("date is: {0}-{1}-{2} {3}:{4}:{5}.{6}",
                        dts.day, dts.month, dts.year, dts.hour,
                        dts.min, dts.sec, dts.milliSec);
        } else {
            MIR_CORE_INFO("Reading the value failed with status code %s", UA_StatusCode_name(retval));
        }
    }

    void OpcuaClient::GetNodesInObjectFolder(){
        /* Browse some objects */
        printf("Browsing nodes in objects folder:\n");
        UA_BrowseRequest bReq;
        UA_BrowseRequest_init(&bReq);
        bReq.requestedMaxReferencesPerNode = 0;
        bReq.nodesToBrowse = UA_BrowseDescription_new();
        bReq.nodesToBrowseSize = 1;
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); /* browse objects folder */
        bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; /* return everything */
        UA_BrowseResponse bResp = UA_Client_Service_browse(m_UaClient, bReq);
        printf("%-9s %-16s %-16s %-16s\n", "NAMESPACE", "NODEID", "BROWSE NAME", "DISPLAY NAME");
        for(size_t i = 0; i < bResp.resultsSize; ++i) {
            for(size_t j = 0; j < bResp.results[i].referencesSize; ++j) {
                UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);
                if(ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
                    printf("%-9u %-16u %-16.*s %-16.*s\n", ref->nodeId.nodeId.namespaceIndex,
                        ref->nodeId.nodeId.identifier.numeric, (int)ref->browseName.name.length,
                        ref->browseName.name.data, (int)ref->displayName.text.length,
                        ref->displayName.text.data);
                } else if(ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING) {
                    printf("%-9u %-16.*s %-16.*s %-16.*s\n", ref->nodeId.nodeId.namespaceIndex,
                        (int)ref->nodeId.nodeId.identifier.string.length,
                        ref->nodeId.nodeId.identifier.string.data,
                        (int)ref->browseName.name.length, ref->browseName.name.data,
                        (int)ref->displayName.text.length, ref->displayName.text.data);
                }
                /* TODO: distinguish further types */
            }
        }


        UA_BrowseRequest_clear(&bReq);
        UA_BrowseResponse_clear(&bResp);
    }

    void OpcuaClient::GetNodesAtNodeID(UA_NodeId nodeId){
            /* Browse some objects */
        printf("Browsing nodes in objects folder:\n");
        UA_BrowseRequest bReq;
        UA_BrowseRequest_init(&bReq);
        bReq.requestedMaxReferencesPerNode = 0;
        bReq.nodesToBrowse = UA_BrowseDescription_new();
        bReq.nodesToBrowseSize = 1;
        bReq.nodesToBrowse[0].nodeId = nodeId; /* browse specified node */
        bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; /* return everything */
        UA_BrowseResponse bResp = UA_Client_Service_browse(m_UaClient, bReq);
        printf("%-9s %-16s %-16s %-16s\n", "NAMESPACE", "NODEID", "BROWSE NAME", "DISPLAY NAME");
        for(size_t i = 0; i < bResp.resultsSize; ++i) {
            for(size_t j = 0; j < bResp.results[i].referencesSize; ++j) {
                UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);
                if(ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
                    printf("%-9u %-16u %-16.*s %-16.*s\n", ref->nodeId.nodeId.namespaceIndex,
                        ref->nodeId.nodeId.identifier.numeric, (int)ref->browseName.name.length,
                        ref->browseName.name.data, (int)ref->displayName.text.length,
                        ref->displayName.text.data);
                } else if(ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING) {
                    printf("%-9u %-16.*s %-16.*s %-16.*s\n", ref->nodeId.nodeId.namespaceIndex,
                        (int)ref->nodeId.nodeId.identifier.string.length,
                        ref->nodeId.nodeId.identifier.string.data,
                        (int)ref->browseName.name.length, ref->browseName.name.data,
                        (int)ref->displayName.text.length, ref->displayName.text.data);
                }
                /* TODO: distinguish further types */
            }
        }
        //UA_BrowsePath_deleteMembers(&bReq);
        UA_BrowseResponse_clear(&bResp);
    }

    void OpcuaClient::BrowseNodesRecursive(const UA_NodeId& nodeId) {
        UA_BrowseRequest bReq;
        UA_BrowseRequest_init(&bReq);
        bReq.requestedMaxReferencesPerNode = 0;
        bReq.nodesToBrowse = UA_BrowseDescription_new();
        bReq.nodesToBrowseSize = 1;
        bReq.nodesToBrowse[0].nodeId = nodeId;
        bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;

        UA_BrowseResponse bResp = UA_Client_Service_browse(m_UaClient, bReq);
        for (size_t i = 0; i < bResp.resultsSize; ++i) {
            for (size_t j = 0; j < bResp.results[i].referencesSize; ++j) {
                UA_ReferenceDescription* ref = &(bResp.results[i].references[j]);
                std::string nodeName((char*)ref->displayName.text.data, ref->displayName.text.length);
                MIR_CORE_TRACE("Node found: {0}", nodeName);

                UA_NodeId childNodeId;
                switch (ref->nodeId.nodeId.identifierType) {
                    case UA_NODEIDTYPE_NUMERIC:
                        childNodeId = UA_NODEID_NUMERIC(ref->nodeId.nodeId.namespaceIndex, ref->nodeId.nodeId.identifier.numeric);
                        break;
                    case UA_NODEIDTYPE_STRING:
                        childNodeId = UA_NODEID_STRING(ref->nodeId.nodeId.namespaceIndex, (char*)ref->nodeId.nodeId.identifier.string.data);
                        break;
                    case UA_NODEIDTYPE_GUID:
                        childNodeId = UA_NODEID_GUID(ref->nodeId.nodeId.namespaceIndex, ref->nodeId.nodeId.identifier.guid);
                        break;
                    case UA_NODEIDTYPE_BYTESTRING:
                        childNodeId = UA_NODEID_BYTESTRING(ref->nodeId.nodeId.namespaceIndex, (char*)ref->nodeId.nodeId.identifier.byteString.data);
                        break;
                    default:
                        MIR_CORE_TRACE("Unknown NodeId type: {0}");
                        continue;
                }

                if (ref->nodeClass == UA_NODECLASS_OBJECT || ref->nodeClass == UA_NODECLASS_VARIABLE) {
                    BrowseNodesRecursive(childNodeId);
                }
            }
        }

        UA_BrowseResponse_clear(&bResp);
        UA_BrowseRequest_clear(&bReq);
    }

    OpcuaClient::~OpcuaClient() {
        UA_Client_disconnect(m_UaClient);
        UA_Client_delete(m_UaClient);
    }

    void OpcuaClient::NodeIteratorTest (){
        UA_NodeId *parent = UA_NodeId_new();
        *parent = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
        UA_Client_forEachChildNodeCall(m_UaClient, UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
                                    nodeIter, (void *) parent);
        UA_NodeId_delete(parent);
    }
    
    void OpcuaClient::WriteBooleanToNode(int namespaceIndex, UA_UInt32 nodeId, bool valueToWrite) {
        UA_NodeId node = UA_NODEID_NUMERIC(namespaceIndex, nodeId);

        // Read the current value to check its type
        UA_Variant value;
        UA_Variant_init(&value);
        UA_StatusCode status = UA_Client_readValueAttribute(m_UaClient, node, &value);
        if (status != UA_STATUSCODE_GOOD) {
            MIR_CORE_TRACE("Failed to read value from node: ns={0}, id={1}, status={2}", namespaceIndex, nodeId, UA_StatusCode_name(status));
            return;
        }

        // Check if the type is boolean
        if (UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            UA_Boolean booleanValue = valueToWrite;
            UA_Variant_setScalar(&value, &booleanValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
            status = UA_Client_writeValueAttribute(m_UaClient, node, &value);
            if (status == UA_STATUSCODE_GOOD) {
                MIR_CORE_TRACE("Successfully wrote boolean value to node: ns={0}, id={1}", namespaceIndex, nodeId);
            } else {
                MIR_CORE_TRACE("Failed to write boolean value to node: ns={0}, id={1}, status={2}", namespaceIndex, nodeId, UA_StatusCode_name(status));
            }
        } else {
            MIR_CORE_TRACE("Node type is not boolean: ns={0}, id={1}", namespaceIndex, nodeId);
        }

       // UA_Variant_clear(&value); might cayuse memory leak
    }

    void OpcuaClient::ReadValueFromNode(int namespaceIndex, UA_UInt32 nodeId) {
        UA_NodeId node = UA_NODEID_NUMERIC(namespaceIndex, nodeId);

        // Read the value
        UA_Variant value;
        UA_Variant_init(&value);
        UA_StatusCode status = UA_Client_readValueAttribute(m_UaClient, node, &value);
        if (status == UA_STATUSCODE_GOOD) {
            //if (UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_BOOLEAN])) {
            if (strcmp(value.type->typeName, "Boolean") == 0) {
                UA_Boolean booleanValue = *(UA_Boolean*)value.data;
                MIR_CORE_TRACE("Read boolean value from node: ns={0}, id={1}, value={2}, type={3}", namespaceIndex, nodeId, booleanValue, value.type->typeName);
            } else if (UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32])) {
                UA_Int32 intValue = *(UA_Int32*)value.data;
                MIR_CORE_TRACE("Read int32 value from node: ns={0}, id={1}, value={2}, type={3}", namespaceIndex, nodeId, intValue, value.type->typeName);
            } else {
                MIR_CORE_TRACE("Read value from node: ns={0}, id={1}, type={2}", namespaceIndex, nodeId, value.type->typeName);
            }
        } else {
            MIR_CORE_TRACE("Failed to read value from node: ns={0}, id={1}, status={2}", namespaceIndex, nodeId, UA_StatusCode_name(status));
        }

        UA_Variant_clear(&value);
    }

}
