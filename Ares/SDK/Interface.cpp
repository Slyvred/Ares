#include "Interface.h"

void* GetInterface(const char* dllname, const char* interfacename)
{
	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllname), "CreateInterface");

	int returnCode = 0;
	void* Interface = CreateInterface(interfacename, &returnCode);

	return Interface;
}