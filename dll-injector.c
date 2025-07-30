#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

int InjectDLL(DWORD processId, const char *dllPath)
{

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL)
    {
        printf("Failed to open process: %d\n", GetLastError());
        return 1;
    }

    LPVOID aLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    if (aLoadLibrary == NULL)
    {
        printf("Failed to resolve LoadLibraryA: %d\n", GetLastError());
        CloseHandle(hProcess);
        return 1;
    }

    SIZE_T size = strlen(dllPath) + 1;
    LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (pDllPath == NULL)
    {
        printf("Failed to allocate memory in target process: %d\n", GetLastError());
        CloseHandle(hProcess);
        return 1;
    }

    if (WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath, size, NULL) == 0)
    {
        printf("Failed to write memory in target process: %d\n", GetLastError());
        CloseHandle(hProcess);
        return 1;
    }

    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)aLoadLibrary, pDllPath, 0, NULL);
    if (hRemoteThread == NULL)
    {
        printf("Failed to create remote thread: %d\n", GetLastError());
        VirtualFreeEx(hProcess, pDllPath, size, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    WaitForSingleObject(hRemoteThread, INFINITE);

    printf("DLL injection process finished, cleaning up...\n");
    VirtualFreeEx(hProcess, pDllPath, size, MEM_RELEASE);
    CloseHandle(hRemoteThread);
    CloseHandle(hProcess);
    printf("Exiting...");
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <PID> <DLL path>", argv[0]);
        return 1;
    }

    DWORD processId = atoi(argv[1]);
    if (processId == 0)
    {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (Process32First(hSnapshot, &entry) == TRUE)
        {
            while (Process32Next(hSnapshot, &entry) == TRUE)
            {
                if (stricmp(entry.szExeFile, argv[1]) == 0)
                {
                    processId = entry.th32ProcessID;
                }
            }
        }
        CloseHandle(hSnapshot);
        if (processId == 0)
        {
            printf("The provided process id/name could not be found.");
            return 1;
        }
        else
        {
            printf("Resolved process name for %s to process id %d.\n", argv[1], processId);
        }
    }

    const char *dllPath = argv[2];
    if (dllPath == NULL || strlen(dllPath) == 0)
    {
        printf("Invalid DLL path provided.");
        return 1;
    }

    const char *extension = strrchr(dllPath, '.');
    if (!extension || strcmpi(extension, ".dll"))
    {
        printf("The provided path does not point to a DLL file.");
        return 1;
    }

    DWORD attributes = GetFileAttributesA(dllPath);
    if (attributes == INVALID_FILE_ATTRIBUTES || (attributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        printf("The DLL file does not exist or cannot be accessed: %s", dllPath);
        return 1;
    }

    return InjectDLL(processId, dllPath);
}