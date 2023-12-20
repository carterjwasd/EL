#include "security.h"

#include "../utilities/directories/directories.h"
#include "../utilities/xor/xor.h"
#include "../utilities/tools/tools.h"
#include "../utilities/crt/crt.h"
#include "../winapi/winapi.h"

void anti_analysis()
{
	const std::vector<std::string> m_window_processes = {
		XOR_STRING("tcpview.exe"),
		XOR_STRING("wireshark.exe"),
		XOR_STRING("filemon.exe"),
		XOR_STRING("frida-helper-32.exe"),
		XOR_STRING("frida-helper-64.exe"),
		XOR_STRING("VmRemoteGuest.exe"),
		XOR_STRING("RemindersServer.exe"),
		XOR_STRING("Detonate.exe"),
		XOR_STRING("analyzer.exe"),
		XOR_STRING("avz.exe"),
		XOR_STRING("winalysis.exe"),
		XOR_STRING("winspy.exe"),
		XOR_STRING("ploader.exe"),
		XOR_STRING("user_imitator.exe"),
		XOR_STRING("dumper64.exe"),
		XOR_STRING("pestudio.exe"),
		XOR_STRING("pexplorer.exe")
	};

	for (auto &m_wp : m_window_processes)
	{
		if (g_tools.is_process_running(m_wp.c_str()))
			g_winapi.ExitProcess(1);
	}
}

void anti_user_debugging()
{
	const std::vector<std::string> m_window_processes = {
		XOR_STRING("httpdebugger.exe"),
		XOR_STRING("httpdebuggerui.exe"),
		XOR_STRING("ollydbg.exe"),
		XOR_STRING("ida.exe"),
		XOR_STRING("ida64.exe"),
		XOR_STRING("idaw.exe"),
		XOR_STRING("idaq.exe"),
		XOR_STRING("idaq64.exe"),
		XOR_STRING("idau.exe"),
		XOR_STRING("idau64.exe"),
		XOR_STRING("scylla.exe"),
		XOR_STRING("scylla_x86.exe"),
		XOR_STRING("scylla_x64.exe"),
		XOR_STRING("64dbg.exe"),
		XOR_STRING("32dbg.exe"),
		XOR_STRING("x32dbg.exe"),
		XOR_STRING("x64dbg.exe"),
		XOR_STRING("x96dbg.exe"),
		XOR_STRING("windbg.exe"),
		XOR_STRING("vboxservice.exe"),
		XOR_STRING("ollydbg.exe"),
		XOR_STRING("vmwareuser.exe"),
		XOR_STRING("vgauthservice.exe"),
		XOR_STRING("vmacthlp.exe"),
		XOR_STRING("joeboxserver.exe"),
		XOR_STRING("immunitydebugger.exe"),
		XOR_STRING("vboxtray.exe"),
		XOR_STRING("vmtoolsd.exe"),
		XOR_STRING("vmwaretray.exe"),
		XOR_STRING("vmsrvc.exe"),
		XOR_STRING("vmusrvc.exe"),
		XOR_STRING("qemu-ga.exe"),
		XOR_STRING("vdagent.exe"),
		XOR_STRING("vdservice.exe"),
		XOR_STRING("vmwareservice.exe"),
		XOR_STRING("vmware-hostd.exe"),
		XOR_STRING("httplog.exe"),
		XOR_STRING("LogHTTP.exe"),
		XOR_STRING("netsniffer.exe"),
		XOR_STRING("sniffhit.exe"),
		XOR_STRING("processhacker.exe"),
		XOR_STRING("sysanalyzer.exe"),
		XOR_STRING("vmware-usbarbitrator.exe"),
		XOR_STRING("vmware-usbarbitrator64.exe")
	};

	for (auto &m_wp : m_window_processes)
	{
		if (g_tools.is_process_running(m_wp.c_str()))
			g_winapi.ExitProcess(1);
	}
}

void anti_vm_modules()
{
	const std::vector<std::string> m_modules = {
		XOR_STRING("SbieDll.dll"),
		XOR_STRING("SxIn.dll"),
		XOR_STRING("Sf2.dll"),
		XOR_STRING("snxhk.dll"),
		XOR_STRING("cmdvrt32.dll"),
		XOR_STRING("vmcheck.dll"),
		XOR_STRING("dir_watch.dll"),
		XOR_STRING("pstorec.dll"),
		XOR_STRING("wpespy.dll"),
		XOR_STRING("api_log.dll"),
		XOR_STRING("avghooka.dll"),
		XOR_STRING("avghookx.dll")
	};

	for (auto &m_m : m_modules)
	{
		if (g_winapi.GetModuleHandleA(m_m.c_str()))
			g_winapi.ExitProcess(1);
	}
}

void anti_virustotal_dirs()
{
	const std::vector<std::string> m_virustotal_folders = {
		XOR_STRING("D:\\TOOLS\\"),
		XOR_STRING("D:\\OS2\\"),
		XOR_STRING("D:\\NT3X\\"),
		XOR_STRING("D:\\TRANSFER\\"),
		XOR_STRING("C:\\VMREMOTE\\"),
		XOR_STRING("C:\\VMRunner\\")
	};

	for (auto &m_vf : m_virustotal_folders)
	{
		if (g_tools.check_directory(m_vf.c_str()) == FILE_FOUNDED)
			g_winapi.ExitProcess(1);
	}
}

void check_remote_debugger_present()
{
	int dp = 0;
	g_winapi.CheckRemoteDebuggerPresent(g_winapi.GetCurrentProcess(), &dp);
	
	if (dp)
		g_winapi.ExitProcess(1);
}

void check_is_debugger_present()
{
	if (g_winapi.IsDebuggerPresent())
		g_winapi.ExitProcess(1);
}

void anti_virustotal_computernames()
{
	const std::vector<std::string> m_compnames = {
		XOR_STRING("WALKER-PC"),
		XOR_STRING("OFFICEPC01"),
		XOR_STRING("AMAZING-AVOCADO"),
		XOR_STRING("NORTHFIELD"),
		XOR_STRING("XC64ZB")
	};

	for (auto &m_cn : m_compnames)
	{
		if (g_tools.get_computer_name().c_str() == m_cn.c_str())
			g_winapi.ExitProcess(1);
	}
}

void mouse_interaction_input_check()
{
	POINT pos1, pos2;

	g_winapi.GetCursorPos(&pos1);
	g_winapi.Sleeping(2000);
	g_winapi.GetCursorPos(&pos2);

	if ((pos1.x == pos2.x) && (pos1.y == pos2.y))
		g_winapi.ExitProcess(1);
}