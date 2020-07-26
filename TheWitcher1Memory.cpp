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
	DWORD vitalityBaseAddr = 0x2212C2D0;
	std::vector<DWORD> vitalityOffsets = { 0x00DC09E4, 0x29C, 0x4, 0x24, 0x0, 0x58 };

private:
	DWORD getModuleBaseAddr(TCHAR* moduleName)
	{
		DWORD moduleBaseAddress = 0x0;
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->pid);

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
		DWORD newValue;

		for (std::vector<DWORD>::iterator it = offsets.begin(), eit = offsets.end(); it != eit; ++it)
		{
			result = baseAddr + *it;
			std::cout << std::hex << result << std::endl;
			bool read = ReadProcessMemory(this->handle, (LPCVOID)(result), &newValue, sizeof(newValue), NULL);

			//if (!read) {
			//	throw new std::exception("unable to read memory");
			//}

			baseAddr = newValue;
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
		DWORD moduleBaseAddr = this->getModuleBaseAddr((TCHAR*)(this->moduleName.c_str()));
		DWORD vitalityAddr = this->getPointerAddr(moduleBaseAddr, this->vitalityOffsets);
		bool written = WriteProcessMemory(this->handle, (LPVOID)vitalityAddr, &vitality, sizeof(vitality), 0);

		//if (!written) {
		//	throw new std::exception("unable to write to memory");
		//}
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
