#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <shlwapi.h>

#include <iostream>
#include "process.hpp"

#pragma warning(disable : 4244)

namespace
{
    bool EnableDebugPrivilege(bool bEnable)
    {
        HANDLE hToken = nullptr;
        LUID luid;

        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
        {
            return false;
        }
        if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
        {
            return false;
        }

        TOKEN_PRIVILEGES tokenPriv;
        tokenPriv.PrivilegeCount = 1;
        tokenPriv.Privileges[0].Luid = luid;
        tokenPriv.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

        if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
        {
            return false;
        }

        return true;
    }
}

struct ProcessError
{
    ProcessError(DWORD code);
    ~ProcessError();

    LPTSTR message;
    DWORD code;
};

std::ostream &operator<<(std::ostream &out, const ProcessError &err)
{
    return out << err.message << " (" << err.code << ")";
}

class ProcessReader
{
public:
    ProcessReader();
    ~ProcessReader();

    void set_filename(int pid);
    std::string get_filename() const;

private:
    bool set_filename1(int pid);
    bool set_filename2(int pid);

    HANDLE processHandle;
    TCHAR filename[MAX_PATH];
};

ProcessReader::ProcessReader() : processHandle(nullptr)
{
    memset(filename, 0, sizeof(filename));
}

ProcessReader::~ProcessReader()
{
    if (processHandle)
    {
        CloseHandle(processHandle);
    }
}

void ProcessReader::set_filename(int pid)
{
    EnableDebugPrivilege(true);

    if (set_filename1(pid))
    {
        return;
    }
    if (set_filename2(pid))
    {
        return;
    }

    EnableDebugPrivilege(false);
}

bool ProcessReader::set_filename1(int pid)
{
    DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;

    processHandle = OpenProcess(dwDesiredAccess, FALSE, pid);
    if (!processHandle)
    {
        std::cerr << "Failed open process " << pid << ": " << ProcessError(GetLastError()) << std::endl;
        return false;
    }

    if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) == 0)
    {
        std::cerr << "Failed to get module filename: " << ProcessError(GetLastError()) << std::endl;
        return false;
    }

    return true;
}

bool ProcessReader::set_filename2(int pid)
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);

    processHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Could not open process snapshot: " << ProcessError(GetLastError()) << std::endl;
        return false;
    }

    BOOL bResult = Process32First(processHandle, &pe32);
    if (!bResult)
    {
        std::cerr << "Could not open first snapshot: " << ProcessError(GetLastError()) << std::endl;
    }

    while (bResult)
    {
        if (pe32.th32ProcessID == pid)
        {
            //
            // The filename's returned is just the filename without any path. Use
            // PathFindOnPath() to search for the binary in standard location such as
            // the system map and all directories in the PATH.
            //
            PathFindOnPath(pe32.szExeFile, 0);
            memcpy(filename, pe32.szExeFile, MAX_PATH);
            break;
        }
        bResult = Process32Next(processHandle, &pe32);
    }

    return true;
}

std::string ProcessReader::get_filename() const
{
    std::string result;

#ifndef UNICODE
    result = filename;
#else
    std::wstring ff(filename);
    result = std::string(ff.begin(), ff.end());
#endif

    return result;
}

ProcessError::ProcessError(DWORD code) : code(code), message('\0')
{
    DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_IGNORE_INSERTS;
    DWORD dwLength = FormatMessage(
        dwFlags,
        0, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&message, 0, 0);

    if (dwLength)
    {
        message[dwLength - 3] = '\0';
    }
}

ProcessError::~ProcessError()
{
    LocalFree(message);
}

void Process::set_path(int pid)
{
    ProcessReader reader;

    reader.set_filename(pid);
    path.assign(reader.get_filename());
}
