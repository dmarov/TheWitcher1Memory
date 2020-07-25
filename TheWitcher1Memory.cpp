#include <iostream>
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>
//"witcher.exe"+00DC09E4
//29C
//4
//24
//0
//58

//<?xml version="1.0" encoding="utf-8"?>
//<CheatTable>
//  <CheatEntries>
//    <CheatEntry>
//      <ID>12</ID>
//      <Description>"pointerscan result"</Description>
//      <LastState Value="500.9629822" Activated="1" RealAddress="067F68A8"/>
//      <VariableType>Float</VariableType>
//      <Address>"witcher.exe"+00DC09E4</Address>
//      <Offsets>
//        <Offset>58</Offset>
//        <Offset>0</Offset>
//        <Offset>24</Offset>
//        <Offset>4</Offset>
//        <Offset>29C</Offset>
//      </Offsets>
//    </CheatEntry>
//  </CheatEntries>
//</CheatTable>
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

int main(int argc, char **argv)
{
	DWORD pid = 0;
	DWORD vitalityAddr;
	DWORD off1, off2, off3, off4;
	float newVitality = 10000;
	float curVitality;

	std::string moduleName = "witcher.exe";
	std::string windowName = "The Witcher (1.4.5.1304)";

	std::vector<int> v = { 0x00DC09E4, 0x29C, 0x4, 0x24, 0x0, 0x58 };
	
	//HWND window = FindWindow(NULL, (LPCWSTR)windowName.c_str());
	HWND window = FindWindowA(NULL, windowName.c_str());
	GetWindowThreadProcessId(window, &pid);

	std::cout << std::hex << pid << std::endl;
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	DWORD clientBaseAddr = 0x2207C2D0 - v[0];
	DWORD baseAddr = clientBaseAddr + v[0];

	ReadProcessMemory(handle, (LPCVOID)(baseAddr + v[1]), &off1, sizeof(off1), NULL);
	ReadProcessMemory(handle, (LPCVOID)(off1 + v[2]), &off2, sizeof(off2), NULL);
	ReadProcessMemory(handle, (LPCVOID)(off2 + v[3]), &off3, sizeof(off3), NULL);
	ReadProcessMemory(handle, (LPCVOID)(off3 + v[4]), &off4, sizeof(off4), NULL);

	vitalityAddr = off4 + v[5];

	ReadProcessMemory(handle, (LPCVOID)(vitalityAddr), &curVitality, sizeof(curVitality), NULL);

	while (true) {
		WriteProcessMemory(handle, (LPVOID)vitalityAddr, &newVitality, sizeof(newVitality), 0);
		Sleep(100);
	}
}
