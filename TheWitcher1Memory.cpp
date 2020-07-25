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
			}
		} while (Module32Next(handle, &entry));
	}

	CloseHandle(handle);

	return moduleBaseAddress;
}

int main(int argc, char **argv)
{
	DWORD pid;
	DWORD vitalityAddr;
	DWORD baseAddr;
	HANDLE handle;
	float newVitality = 10000;
	float curViotality;

	std::string moduleName = "witcher.exe";
	std::string windowName = "The Witcher (1.4.5.1304)";

	std::vector<int> v = { 0x00DC09E4, 0x29C, 0x4, 0x24, 0x0, 0x58 };
	
	HWND window = FindWindow(NULL, (LPCWSTR)windowName.c_str());
	GetWindowThreadProcessId(window, &pid);

	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	DWORD clientBaseAddr = getModuleBaseAddress((TCHAR*)moduleName.c_str(), pid);

	ReadProcessMemory(handle, (LPCVOID)(clientBaseAddr + v[0]), &baseAddr, sizeof(baseAddr), NULL);

	std::cout << std::hex << baseAddr << std::endl;

	std::cin.get();
}
