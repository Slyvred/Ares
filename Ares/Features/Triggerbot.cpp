#include "../Hacks.h"
#include "../Memory.h"
#include "ItemDefinitionIndex.h"

void Hacks::Triggerbot(int vKey)
{
	if (config.triggerbot || GetAsyncKeyState(vKey))
	{
		if (GetAsyncKeyState(VK_LBUTTON)) return;

		int crossId = *(int*)(localPlayer + netvars::m_iCrosshairId);
		if (crossId && crossId < ClientEntityList->GetHighestEntityIndex())
		{
			Entity* ent = (Entity*)ClientEntityList->GetClientEntity(crossId);
			if (!ent->IsValid() || ent->IsImmune()) return; // In invalid return

			short weaponIndex = localPlayer->GetActiveWeapon();
			if (!weaponIndex) return;
			if ((weaponIndex == WEAPON_SSG08 || weaponIndex == WEAPON_AWP) && !localPlayer->IsScoped()) return; // If not scoping with sniper we will not shoot
			
			float distance = (*(Vector3*)(localPlayer + netvars::m_vecOrigin)).Distance(*(Vector3*)(ent + netvars::m_vecOrigin)); // Zeus bot
			if (weaponIndex == WEAPON_TASER && distance > 160) return;

			if (ent->GetTeam() != localPlayer->GetTeam()) // If the conditions above are met and it's an enemy or if we're in ffa we shoot
				*(int*)(client + signatures::dwForceAttack) = 6;
		}
	}
}