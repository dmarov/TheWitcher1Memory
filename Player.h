#pragma once
#include <vector>
#include <string>
#include <windows.h>

class Player
{
	private:
		std::string mainModuleName = "witcher.exe";
		std::string windowName = "The Witcher (1.4.5.1304)";
		std::vector<DWORD_PTR> vitalityOffsets = { 0x00DC09E4, 0x29C, 0x4, 0x24, 0x0, 0x58 };
		std::vector<DWORD_PTR> enduranceOffsets = { 0x00DC09E4, 0x29C, 0x4, 0x24, 0x0, 0x1BC };
		DWORD pid;
		HWND window;
		HANDLE handle;

	private:
		DWORD_PTR getModuleBaseAddr(TCHAR* moduleName);
		DWORD getPointerAddr(DWORD_PTR baseAddr, std::vector<DWORD_PTR> offsets);

	public:
		Player();
		void setVitality(float vitality);
		void setEndurance(float endurance);
};

