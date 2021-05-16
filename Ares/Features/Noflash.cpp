#include "../Hacks.h"
#include "../Memory.h"

void Hacks::NoFlash(bool enable)
{
	if (enable)
	{
		/*float* flashDur = (float*)(localPlayer + netvars::m_flFlashDuration);
		if (*flashDur) *flashDur = 0.f;*/

		float* flashAlpha = (float*)(localPlayer + netvars::m_flFlashMaxAlpha); // Replaced the antiflash with a flash reducer since this a legit focused cheat
		if (*flashAlpha > 75.0f) *flashAlpha = 75.0f;
	}
}