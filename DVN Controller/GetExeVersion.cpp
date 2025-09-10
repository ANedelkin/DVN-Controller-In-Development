#include "GetExeVersion.h"

wxString GetExeVersion()
{
    char filename[MAX_PATH];
    GetModuleFileNameA(NULL, filename, MAX_PATH);

    DWORD handle = 0;
    DWORD size = GetFileVersionInfoSizeA(filename, &handle);
    if (size == 0) return "Unknown";

    std::vector<char> data(size);
    if (!GetFileVersionInfoA(filename, handle, size, data.data())) return "Unknown";

    void* value = nullptr;
    UINT len = 0;

    if (!VerQueryValueA(data.data(), "\\StringFileInfo\\040904b0\\ProductVersion", &value, &len))
        return "Unknown";

    if (value == nullptr || len == 0) return "Unknown";

    return wxString(static_cast<char*>(value), wxConvUTF8);
}