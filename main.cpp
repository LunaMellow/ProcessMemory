#include <iostream>
#include <windows.h>
#include <TlHelp32.h>

//Global Variables
HANDLE hProc = NULL;
DWORD pID;

bool attachProc(char* procName)
{
    PROCESSENTRY32 procEntry32;

    //Defining the size so we can populate it
    procEntry32.dwSize = sizeof(PROCESSENTRY32);

    //Taking a snapshot of all processes running
    auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcSnap == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to take snapshot of processes" << std::endl;
        return false;
    }

    while (Process32Next(hProcSnap, &procEntry32))
    {
        std::cout << procEntry32.szExeFile << std::endl;

        if (!strcmp(procName, procEntry32.szExeFile))
        {
            std::cout << "Found process " << procEntry32.szExeFile << "with process ID " << procEntry32.th32ProcessID << std::endl;
            hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
            pID = procEntry32.th32ProcessID;

            if (hProc == NULL)
            {
                std::cout << "Failed to handle to process." << std::endl;
            }

            CloseHandle(hProcSnap);
            return true;
        }
    }

    std::cout << "Couldn't find " << procName << " in the process snapshot." << std::endl;

    CloseHandle(hProcSnap);
    return false;
}

template <class dataType>
void wmp(dataType valToWrite, DWORD addressToWrite)
{
    WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);
}

template <class dataType>
dataType rmp(DWORD addressToRead)
{
    dataType rpmBuffer;

    ReadProcessMemory(hProc, (PVOID)addressToRead, &rpmBuffer, sizeof(dataType), 0);

    return rpmBuffer;
}

int main()
{
    DWORD memoryAddress = 0x000000;

    attachProc((char*)"Target application.exe");

    while (true)
    {
        wmp<int>(100, memoryAddress);
    }
}
