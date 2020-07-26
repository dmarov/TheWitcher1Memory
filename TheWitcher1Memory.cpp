#include <iostream>
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <psapi.h>

class Player
{

private:
	DWORD pid;
	HWND window;
	HANDLE handle;

	std::string mainModuleName = "witcher.exe";
	std::string windowName = "The Witcher (1.4.5.1304)";
	std::vector<DWORD_PTR> vitalityOffsets = { 0x00DC09E4, 0x29C, 0x4, 0x24, 0x0, 0x58 };

private:
	DWORD_PTR getModuleBaseAddr(TCHAR* moduleName)
	{
		DWORD_PTR moduleBaseAddr = 0x0;
		HMODULE     *moduleArray;
		LPBYTE      moduleArrayBytes;
		DWORD       bytesRequired;

		if (this->handle)
		{
			if (EnumProcessModules(this->handle, NULL, 0, &bytesRequired))
			{
				if ( bytesRequired )
				{
					moduleArrayBytes = (LPBYTE)LocalAlloc( LPTR, bytesRequired );

					if ( moduleArrayBytes )
					{
						unsigned int moduleCount;

						moduleCount = bytesRequired / sizeof( HMODULE );
						moduleArray = (HMODULE *)moduleArrayBytes;

						if ( EnumProcessModules( this->handle, moduleArray, bytesRequired, &bytesRequired ) )
						{
							moduleBaseAddr = (DWORD_PTR)moduleArray[0];
						}

						LocalFree( moduleArrayBytes );
					}
				}
			}
		}

		return moduleBaseAddr;
	}

	DWORD getPointerAddr(DWORD_PTR baseAddr, std::vector<DWORD> offsets)
	{
		DWORD_PTR result = baseAddr;
		DWORD_PTR newValue;

		for (std::vector<DWORD_PTR>::iterator it = offsets.begin(), eit = offsets.end(); it != eit; ++it)
		{
			result = baseAddr + *it;
			std::cout << std::hex << result << std::endl;
			bool read = ReadProcessMemory(this->handle, (LPCVOID)(result), &newValue, sizeof(newValue), NULL);

			if (!read) {
				throw new std::exception("unable to read memory");
			}

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
		DWORD_PTR moduleBaseAddr = this->getModuleBaseAddr((TCHAR*)(this->mainModuleName.c_str()));
		std::cout << std::hex << moduleBaseAddr << std::endl;
		DWORD vitalityAddr = this->getPointerAddr(moduleBaseAddr, this->vitalityOffsets);
		bool written = WriteProcessMemory(this->handle, (LPVOID)vitalityAddr, &vitality, sizeof(vitality), 0);

		if (!written) {
			throw new std::exception("unable to write to memory");
		}
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
		Sleep(100);
	}
}
