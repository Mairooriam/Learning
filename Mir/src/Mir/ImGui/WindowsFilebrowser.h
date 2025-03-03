#pragma once
#include <Windows.h>
#include <string>
#include <shobjidl.h> 
namespace Mir {
    struct FileSelection {
        std::string fileName;
        std::string filePath;
        void clear(){
            fileName = "";
            filePath = "";
        }
        FileSelection() : fileName(""), filePath("") {}
        FileSelection(const std::string& name, const std::string& path) : fileName(name), filePath(path) {}
    };


    class WindowsFileBrowser {
        private:
            std::string sSelectedFile;
            std::string sFilePath;
            bool isInitialized;
            std::string sInitialFileName;
        
        public:
            WindowsFileBrowser();
            ~WindowsFileBrowser();
        
            // Delete copy constructor and assignment operator due to COM handling
            WindowsFileBrowser(const WindowsFileBrowser&) = delete;
            WindowsFileBrowser& operator=(const WindowsFileBrowser&) = delete;
        
            FileSelection GetFileSelection();
            void SetInitialFileName(const std::string& filename);

            bool OpenFile(bool selectFolder = false, const std::vector<COMDLG_FILTERSPEC>& filters = {});
            bool SaveFile(const std::vector<COMDLG_FILTERSPEC>& filters, const wchar_t* defaultExt);
            bool IsInitialized() const;
        }; 
}
