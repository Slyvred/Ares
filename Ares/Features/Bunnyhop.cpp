#include "../Hacks.h"
#include "../Memory.h"

#define FL_ONGROUND (1 << 0)

void Hacks::Bunnyhop(bool enable)
{
	if (enable)
	{
		BYTE flags = *(BYTE*)(localPlayer + netvars::m_fFlags);
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && flags & FL_ONGROUND)
			if (localPlayer->IsMoving())
				*(int*)(client + signatures::dwForceJump) = 6;
	}
}