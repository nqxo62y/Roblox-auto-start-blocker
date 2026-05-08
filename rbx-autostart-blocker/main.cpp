#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <thread>
#include <chrono>

void KillProcess(const std::wstring& procName) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);

    if (Process32FirstW(snap, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, procName.c_str()) == 0) {
                HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProc) {
                    TerminateProcess(hProc, 0);
                    CloseHandle(hProc);
                }
            }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
}

bool IsRunning(const std::wstring& procName) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);
    bool found = false;

    if (Process32FirstW(snap, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, procName.c_str()) == 0) {
                found = true;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return found;
}

void RemoveFromAutostart() {
    HKEY hKey;
    const wchar_t* regPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

    if (RegOpenKeyExW(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegDeleteValueW(hKey, L"Roblox");
        RegDeleteValueW(hKey, L"RobloxPlayer");
        RegDeleteValueW(hKey, L"roblox-player");
        RegCloseKey(hKey);
    }

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegDeleteValueW(hKey, L"Roblox");
        RegDeleteValueW(hKey, L"RobloxPlayer");
        RegCloseKey(hKey);
    }
}

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    RemoveFromAutostart();

    KillProcess(L"RobloxPlayer.exe");
    KillProcess(L"RobloxPlayerBeta.exe");
    KillProcess(L"RobloxCrashHandler.exe");

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        if (IsRunning(L"RobloxPlayer.exe") ||
            IsRunning(L"RobloxPlayerBeta.exe") ||
            IsRunning(L"RobloxCrashHandler.exe"))
        {
            KillProcess(L"RobloxPlayer.exe");
            KillProcess(L"RobloxPlayerBeta.exe");
            KillProcess(L"RobloxCrashHandler.exe");
        }
    }

    return 0;
}