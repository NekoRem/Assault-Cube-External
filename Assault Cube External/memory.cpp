#include "memory.h"

memory::memory()
{
}

memory::memory(const char* process_name, DWORD access_rights)
{
	attach(process_name, access_rights);
}

memory::~memory()
{
	detach();
}

bool memory::attach(const char* process_name, DWORD access_rights)
{
	HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (ss)
	{
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(ss, &pe))
		{
			do
			{
				if (!strcmp(pe.szExeFile, process_name))
				{
					this->process_handle = OpenProcess(access_rights, false, pe.th32ProcessID);
				}
			} while (Process32Next(ss, &pe));
		}
		CloseHandle(ss);
	}
	printf("process_handle(%s): 0x%x\n", process_name, (DWORD)this->process_handle);
	if (this->process_handle) { return TRUE; } else { return FALSE; }	
}

bool memory::detach()
{
	printf("detaching...");
	return CloseHandle(this->process_handle);
}

module memory::get_module(const char* module_name)
{
	module mod = { 0 };
	HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(this->process_handle));
	if (ss)
	{
		MODULEENTRY32 me;
		me.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(ss, &me))
		{
			do
			{
				if (!strcmp(me.szModule, module_name))
				{
					mod.base = (DWORD)me.modBaseAddr;
					mod.size = (DWORD)me.modBaseSize;
					break;
				}
			} while (Module32Next(ss, &me));
		}
		CloseHandle(ss);
	}
	return mod;
}

bool memory::compare_memory(const byte* data, const char* pattern)
{
	for (; *pattern; *pattern != ' ' ? ++data : data, ++pattern)
	{
		if (*pattern == ' ' || *pattern == '?') continue;
		if (*data != get_byte(pattern)) return FALSE;
		++pattern;
	}
	return TRUE;
}

DWORD memory::pattern_scan(module mod, const char* pattern, int offset, int extra, bool relative, bool subtract)
{
	DWORD address = 0;

	byte* data = new byte[mod.size];
	ReadProcessMemory(this->process_handle, (LPCVOID)mod.base, data, mod.size, NULL);

	for (DWORD i = 0x1000; i < mod.size; i++)
	{
		if (compare_memory((const byte*)(data + i), pattern))
		{
			address = mod.base + i + offset;
			if (relative) { ReadProcessMemory(this->process_handle, LPCVOID(address), &address, sizeof(DWORD), NULL); }
			if (subtract) { address -= mod.base; }
			address += extra;
			break;
		}
	}

	delete[] data;
	return address;
}