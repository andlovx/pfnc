// Path for program using network port
// Copyright (C) 2024 Anders Lövgren, Xertified AB
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <iostream>
#include "process.hpp"
#include "error.hpp"

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
        EnableDebugPrivilege(false);
        return;
    }
    if (set_filename2(pid))
    {
        EnableDebugPrivilege(false);
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
        error.write("Failed open process", pid);
        return false;
    }

    if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) == 0)
    {
        error.write("Failed to get module filename");
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
        error.write("Could not open process snapshot");
        return false;
    }

    BOOL bResult = Process32First(processHandle, &pe32);
    if (!bResult)
    {
        error.write("Could not open first snapshot");
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

void Process::set_path(int pid)
{
    ProcessReader reader;

    reader.set_filename(pid);
    path.assign(reader.get_filename());
}
