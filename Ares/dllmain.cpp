#include <Windows.h>
#include "Hacks.h"
#include "Memory.h"

Hacks Ares;

intptr_t WINAPI Main(HMODULE hMod) 
{	

#ifdef _DEBUG
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	SetConsoleTitle("Ares Debug");
#endif

	Ares.Init();
	Ares.SetupHooks();

	while (!GetAsyncKeyState(VK_END)) 
	{
		Ares.Update();
		Sleep(1);
	}

#ifdef _DEBUG
	fclose(f);
	FreeConsole();
#endif

	Ares.Cleanup();
	//delete Ares; // stopped using a pointer for the Hacks class so this is useless
	FreeLibraryAndExitThread(hMod, 0);
}

BOOL APIENTRY DllMain(HMODULE hMod, intptr_t dwReason, LPVOID lpReserved) 
{
	if (dwReason == DLL_PROCESS_ATTACH) 
	{
		DisableThreadLibraryCalls(hMod);
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, hMod, 0, nullptr));
	}
	return TRUE;
}