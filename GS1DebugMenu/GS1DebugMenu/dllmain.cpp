// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <shlwapi.h>
void patch(BYTE* ptr, BYTE* buf, size_t len) {
    DWORD curProtection;
    VirtualProtect(ptr, len, PAGE_EXECUTE_READWRITE, &curProtection);
    memcpy(ptr, buf, len);
    VirtualProtect(ptr, len, curProtection, &curProtection);
}

DWORD WINAPI AppThread(HMODULE hModule) {
    WCHAR fileName[512];
    DWORD size = 512;
    GetModuleFileNameW(NULL, fileName, size);
    std::cout << fileName << std::endl;
    if (CompareStringW(LOCALE_INVARIANT, 0, PathFindFileNameW(fileName), -1, L"逆転裁判　配信版.exe", -1) != CSTR_EQUAL)
    {
        return TRUE;
    }
    while (true)
    {
        if (GetAsyncKeyState('K'))
        {
            *(int*)0x1002ED84 = 0x10;
            Sleep(10000);
        }
    }
    return TRUE;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)AppThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

