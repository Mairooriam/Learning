#include "PythonTool.h"
#include <string>
#include <filesystem>

#include <iostream>

namespace Mir {

    PythonTool::PythonTool()
    {
        Py_Initialize(); // Initialize Python interpreter
    }

    PythonTool::~PythonTool()
    {
        Py_Finalize(); // Cleanup Python interpreter

        
    }
    bool PythonTool::RunPythonFunction(const std::string &moduleName, const std::string &functionName)
        {
            try {
                // Import the module
                PyObject* pModule = PyImport_ImportModule(moduleName.c_str());
                if (pModule == nullptr) {
                    std::cerr << "Failed to load module: " << moduleName << std::endl;
                    return false;
                }
    
                // Get the function object
                PyObject* pFunc = PyObject_GetAttrString(pModule, functionName.c_str());
                if (pFunc == nullptr || !PyCallable_Check(pFunc)) {
                    std::cerr << "Failed to load function: " << functionName << std::endl;
                    Py_XDECREF(pModule);
                    return false;
                }
    
                // Call the function
                PyObject* pResult = PyObject_CallObject(pFunc, nullptr);
                if (pResult == nullptr) {
                    std::cerr << "Function call failed" << std::endl;
                    Py_XDECREF(pFunc);
                    Py_XDECREF(pModule);
                    return false;
                }
    
                // Cleanup
                Py_XDECREF(pResult);
                Py_XDECREF(pFunc);
                Py_XDECREF(pModule);
    
                return true;
            }
            catch (...) {
                std::cerr << "An error occurred while running Python code" << std::endl;
                return false;
            }
        }


        bool PythonTool::RunPythonScript(const std::string& scriptPath, const std::vector<std::string>& args) {
            try {
                // Create Python list for arguments
                PyObject* sysArgv = PyList_New(args.size() + 1);
                
                // Add script path as first argument (argv[0])
                PyList_SetItem(sysArgv, 0, PyUnicode_FromString(scriptPath.c_str()));
                
                // Add additional arguments
                for (size_t i = 0; i < args.size(); i++) {
                    PyList_SetItem(sysArgv, i + 1, PyUnicode_FromString(args[i].c_str()));
                }
        
                // Get sys.argv and replace it
                PyObject* sysModule = PyImport_ImportModule("sys");
                PyObject_SetAttrString(sysModule, "argv", sysArgv);
                Py_DECREF(sysModule);
                Py_DECREF(sysArgv);
        
                // Execute the Python script
                FILE* file = fopen(scriptPath.c_str(), "r");
                if (!file) {
                    std::cerr << "Failed to open script file: " << scriptPath << std::endl;
                    return false;
                }
        
                PyRun_SimpleFile(file, scriptPath.c_str());
                fclose(file);
        
                return true;
            }
            catch (...) {
                std::cerr << "An error occurred while running Python script" << std::endl;
                return false;
            }
        }
        

} // namespace Mir