#include "Player.h"
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <psapi.h>

Player::Player() 
{
	this->window = FindWindowA(NULL, this->windowName.c_str());
	GetWindowThreadProcessId(this->window, &(this->pid));
	this->handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->pid);
}

DWORD_PTR Player::getModuleBaseAddr(TCHAR* moduleName)
{
	DWORD_PTR moduleBaseAddr = NULL;
	DWORD bytesRequired;

	bool success = EnumProcessModules(this->handle, NULL, 0, &bytesRequired);

	if (!success) {
		throw std::exception("unable to scan for modules");
	}

	LPBYTE moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

	unsigned int moduleCount;

	moduleCount = bytesRequired / sizeof(HMODULE);
	HMODULE* moduleArray = (HMODULE *)moduleArrayBytes;

	success = EnumProcessModules(this->handle, moduleArray, bytesRequired, &bytesRequired);

	if (!success) {
		throw std::exception("unable to scan for modules");
	}

	moduleBaseAddr = (DWORD_PTR)moduleArray[0];

	LocalFree(moduleArrayBytes);

	if (moduleBaseAddr == NULL) {
		throw std::exception("unable to find module");
	}

	return moduleBaseAddr;
}

DWORD Player::getPointerAddr(DWORD_PTR baseAddr, std::vector<DWORD> offsets)
{
	DWORD_PTR result = baseAddr;
	DWORD_PTR newValue;

	for (std::vector<DWORD_PTR>::iterator it = offsets.begin(), eit = offsets.end(); it != eit; ++it)
	{
		result = baseAddr + *it;
		bool read = ReadProcessMemory(this->handle, (LPCVOID)(result), &newValue, sizeof(newValue), NULL);

		if (!read) {
			throw new std::exception("unable to read memory");
		}

		baseAddr = newValue;
	}

	return result;
}

void Player::setVitality(float vitality)
{
	DWORD_PTR moduleBaseAddr = this->getModuleBaseAddr((TCHAR*)(this->mainModuleName.c_str()));
	DWORD vitalityAddr = this->getPointerAddr(moduleBaseAddr, this->vitalityOffsets);
	bool written = WriteProcessMemory(this->handle, (LPVOID)vitalityAddr, &vitality, sizeof(vitality), 0);

	if (!written) {
		throw new std::exception("unable to write to memory");
	}
}

void Player::setEndurance(float endurance)
{
	DWORD_PTR moduleBaseAddr = this->getModuleBaseAddr((TCHAR*)(this->mainModuleName.c_str()));
	DWORD enduranceAddr = this->getPointerAddr(moduleBaseAddr, this->enduranceOffsets);
	bool written = WriteProcessMemory(this->handle, (LPVOID)enduranceAddr, &endurance, sizeof(endurance), 0);

	if (!written) {
		throw new std::exception("unable to write to memory");
	}
}
