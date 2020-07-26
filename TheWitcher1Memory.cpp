#include <iostream>
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>

class Player
{

private:
	DWORD pid;
	HWND window;
	HANDLE handle;

	std::string moduleName = "witcher.exe";
	std::string windowName = "The Witcher (1.4.5.1304)";
	DWORD vitalityBaseAddr = 0x20C9C2D0;
	std::vector<DWORD> vitalityOffsets = { 0x29C, 0x4, 0x24, 0x0, 0x58 };

private:
	DWORD getModuleBaseAddress(TCHAR* moduleName, DWORD pid)
	{
		DWORD moduleBaseAddress = 0x0;
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

		MODULEENTRY32 entry = { 0 };
		entry.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(handle, &entry))
		{
			do {
				if (_tcscmp(entry.szModule, moduleName) == 0)
				{
					moduleBaseAddress = (DWORD)entry.modBaseAddr;
					break;
				}
			} while (Module32Next(handle, &entry));
		}

		CloseHandle(handle);

		return moduleBaseAddress;
	}

	DWORD getPointerAddr(DWORD baseAddr, std::vector<DWORD> offsets)
	{
		DWORD result = baseAddr;

		for (std::vector<DWORD>::iterator it = offsets.begin(), eit = offsets.end(); it != eit; ++it)
		{
			result = baseAddr + *it;
			std::cout << std::hex << result << std::endl;
			ReadProcessMemory(this->handle, (LPCVOID)(result), &baseAddr, sizeof(baseAddr), NULL);
		}

		return result;
	}

public:

	Player() 
	{
		this->window = FindWindowA(NULL, this->windowName.c_str());
		GetWindowThreadProcessId(this->window, &(this->pid));
		this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->pid);
	}

	void setVitality(float vitality)
	{
		DWORD vitalityAddr = this->getPointerAddr(this->vitalityBaseAddr, this->vitalityOffsets);
		WriteProcessMemory(handle, (LPVOID)vitalityAddr, &vitality, sizeof(vitality), 0);
	}

	void setMana(float mana)
	{
	
	}
};

int main(int argc, char **argv)
{
	Player player = Player();

	while (true) {
		player.setVitality(10000);
		Sleep(10000);
	}
}
