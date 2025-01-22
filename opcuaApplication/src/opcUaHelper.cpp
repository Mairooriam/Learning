#include <open62541/client.h>
#include <iostream>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <vector>

namespace opc{
    void browseNamespaces(UA_Client* uaClient, std::vector<std::string>& namespaces) {
        UA_Variant value;
        UA_Variant_init(&value);

        UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY);
        UA_StatusCode status = UA_Client_readValueAttribute(uaClient, nodeId, &value);
        if (status == UA_STATUSCODE_GOOD && UA_Variant_hasArrayType(&value, &UA_TYPES[UA_TYPES_STRING])) {
            size_t arrayLength = value.arrayLength;
            UA_String* namespaceArray = (UA_String*)value.data;

            for (size_t i = 0; i < arrayLength; ++i) {
                namespaces.push_back(std::string((char*)namespaceArray[i].data, namespaceArray[i].length));
            }
        }
        m_SelectedNamespace.resize(namespaces.size(), false);
        UA_Variant_clear(&value);
    }
} // namespace opc

