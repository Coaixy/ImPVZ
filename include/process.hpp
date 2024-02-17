#include <Windows.h>
#include <cstdint>
#include <vector>
#include <iostream>

template <typename var>
bool write_mem(IN HANDLE processHandle, IN int64_t address, IN var value) {
	if (hackSettings.b_debug_disableWriteMemory) {
		return true;
	}
	return WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(var), NULL);
}

template <typename var>
var read_mem(IN HANDLE processHandle, IN int64_t address, int test = 0) {
	if (test == 1)printf("%ld - test read \n", address);
	var value;
	ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(var), NULL);
	return value;
}

DWORD GetProcessId(const char* Title)
{
	HWND hWnd = FindWindow(NULL, Title);
	DWORD pid = 0;
	if (hWnd)
	{
		GetWindowThreadProcessId(hWnd, &pid);
	}
	return pid;
}

bool isAddressInMemoryRegions(HANDLE processHandle, IN int64_t address) {
	MEMORY_BASIC_INFORMATION info;
	VirtualQueryEx(processHandle, (LPCVOID)address, &info, sizeof(info));
	return info.State == MEM_COMMIT;
}
int64_t FindPointer(IN HANDLE processHandle, IN int64_t baseAddress, IN std::vector<int64_t> offsets)
{
	if (offsets.size() == 0) {
		return NULL;
	}

	int64_t Address = baseAddress;
	Address = read_mem<long>(processHandle, Address);


	for (int i = 0; i < offsets.size(); i++) //Loop trough the offsets
	{
		Address += offsets[i];
		Address = read_mem<long>(processHandle, Address);
	}
	return Address;
}
