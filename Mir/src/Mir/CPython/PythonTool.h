
#include <Python.h>
#include <string>
#include <iostream>
namespace Mir {
    class PythonTool {
        public:
            // Constructor - initializes Python interpreter
            PythonTool();
            
            // Destructor - cleanup Python interpreter
            ~PythonTool();
            
            /**
             * Runs a Python function from a specified module
             * @param moduleName The name of the Python module to import
             * @param functionName The name of the function to call within the module
             * @return true if the function was successfully called, false otherwise
             */
            bool RunPythonFunction(const std::string& moduleName, const std::string& functionName);
            
            // New function to run Python script with arguments
            bool RunPythonScript(const std::string& scriptPath, const std::vector<std::string>& args = {});


            // Delete copy constructor and assignment operator
            // Python interpreter should not be copied
            PythonTool(const PythonTool&) = delete;
            PythonTool& operator=(const PythonTool&) = delete;
        };
}

