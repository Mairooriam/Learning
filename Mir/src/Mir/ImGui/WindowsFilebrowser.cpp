#include "WindowsFilebrowser.h"

namespace Mir {

    WindowsFileBrowser::WindowsFileBrowser() : isInitialized(false) {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr)) {
            isInitialized = true;
        } else if (hr == S_FALSE) {
            // Already initialized in this thread, still mark as initialized
            isInitialized = true;
        }
    }

    WindowsFileBrowser::~WindowsFileBrowser() {
        if (isInitialized) {
            CoUninitialize();
        }
    }

bool WindowsFileBrowser::OpenFile(bool selectFolder, const std::vector<COMDLG_FILTERSPEC>& filters) {
    if (!isInitialized) {
        return false;
    }

    IFileOpenDialog* f_FileSystem;
    HRESULT f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                                    IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
    if (FAILED(f_SysHr)) {
        return false;
    }

    // Set options for folder selection if requested
    if (selectFolder) {
        DWORD dwOptions;
        f_FileSystem->GetOptions(&dwOptions);
        f_FileSystem->SetOptions(dwOptions | FOS_PICKFOLDERS);
    }

    // Set file types if filters are provided
    if (!filters.empty()) {
        f_FileSystem->SetFileTypes(static_cast<UINT>(filters.size()), filters.data());
    }

    // Rest of your existing code...
    f_SysHr = f_FileSystem->Show(NULL);
    if (f_SysHr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) {
        f_FileSystem->Release();
        return false;
    }

        IShellItem* f_Files;
        f_SysHr = f_FileSystem->GetResult(&f_Files);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            return false;
        }

        PWSTR f_Path;
        f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
        if (FAILED(f_SysHr)) {
            f_Files->Release();
            f_FileSystem->Release();
            return false;
        }

        std::wstring path(f_Path);
        sFilePath = std::string(path.begin(), path.end());
        size_t slash = sFilePath.find_last_of("/\\");
        sSelectedFile = sFilePath.substr(slash + 1);

        CoTaskMemFree(f_Path);
        f_Files->Release();
        f_FileSystem->Release();
        return true;
    }

    // In WindowsFileBrowser.cpp, add this new method
    FileSelection WindowsFileBrowser::GetFileSelection() {
        FileSelection result = {sSelectedFile, sFilePath};
        // Reset both values
        sSelectedFile.clear();
        sFilePath.clear();
        return result;
    }

    bool WindowsFileBrowser::IsInitialized() const {
        return isInitialized;
    }

}