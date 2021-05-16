#include "../Hacks.h"
#include "../Memory.h"


void Hacks::NoscopeCrosshair(bool enable)
{
	if (enable)
	{
		if (localPlayer->IsValid()) cvars.weapon_debug_spread_show->setValue(localPlayer->IsScoped() ? 0 : 3);
		else cvars.weapon_debug_spread_show->setValue(0);
	}
}