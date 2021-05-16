#include "../Hacks.h"
#include "../Memory.h"
#include "ItemDefinitionIndex.h"

void SetGlow(intptr_t glowObject, int glowIndex, float R, float G, float B, float A)
{
	*(float*)((glowObject)+((glowIndex * 0x38) + 0x4)) = R;
	*(float*)((glowObject)+((glowIndex * 0x38) + 0x8)) = G;
	*(float*)((glowObject)+((glowIndex * 0x38) + 0xC)) = B;
	*(float*)((glowObject)+((glowIndex * 0x38) + 0x10)) = A;
}

void Hacks::SetChams(Entity* ent, int R, int G, int B)
{
	*(int*)(ent + netvars::m_clrRender) = R;
	*(int*)(ent + netvars::m_clrRender + 0x1) = G;
	*(int*)(ent + netvars::m_clrRender + 0x2) = B;
}

void Hacks::Glowhack()
{
	if (config.glow)
	{
		intptr_t glowObjectManager = *(intptr_t*)(client + signatures::dwGlowObjectManager);

		for (int i = 1; i < ClientEntityList->GetHighestEntityIndex(); i++)
		{
			Entity* ent = (Entity*)ClientEntityList->GetClientEntity(i);
			if (!ent->IsValid() || ent == localPlayer) continue;

			int glowIndex = *(int*)(ent + netvars::m_iGlowIndex);

			/*bool is_c4_owner = false;
			for (short i = 0; i < 8; i++) // Loop in m_hMyWeapons
			{
				intptr_t currentWeapon = *(intptr_t*)(ent + netvars::m_hMyWeapons + i * 0x4);
				currentWeapon = (intptr_t)ClientEntityList->GetClientEntityFromHandle(currentWeapon);
				if (!currentWeapon) continue;

				short weaponIndex = *(short*)(currentWeapon + netvars::m_iItemDefinitionIndex);
				if (weaponIndex == WEAPON_C4) is_c4_owner = true;
			}*/

			// For everyone
			if (ent->IsDefusing())
			{
				SetGlow(glowObjectManager, glowIndex, 0.f, 1.f, 0.f, 1.f);
				if (config.chams) SetChams(ent, 0, 255, 0);
			}

			else if (ent->IsC4Owner())
			{
				SetGlow(glowObjectManager, glowIndex, 1.f, 0.5f, 0.f, 1.f);
				if (config.chams) SetChams(ent, 255, 128, 0);
			}

			// For allies
			else if (ent->GetTeam() == localPlayer->GetTeam())
			{
				SetGlow(glowObjectManager, glowIndex, 0.f, 1.f, 1.f, 1.f);
				if (config.chams) SetChams(ent, 0, 255, 255);
			}

			// For enemies
			else
			{
				if (ent->IsScoped())
				{
					SetGlow(glowObjectManager, glowIndex, 1.f, 0.f, 1.f, 1.f);
					if (config.chams) SetChams(ent, 255, 0, 255);
				}

				else if (ent->IsFlashed())
				{
					SetGlow(glowObjectManager, glowIndex, 1.f, 1.f, 1.f, 1.f);
					if (config.chams) SetChams(ent, 255, 255, 255);
				}

				else
				{
					SetGlow(glowObjectManager, glowIndex, 1.f, 0.f, 0.f, 1.f);
					if (config.chams) SetChams(ent, 255, 0, 0);
				}
			}

			*(bool*)((glowObjectManager)+((glowIndex * 0x38) + 0x24)) = true;
			*(bool*)((glowObjectManager)+((glowIndex * 0x38) + 0x25)) = false;
			if (config.chams) *(bool*)((glowObjectManager)+((glowIndex * 0x38) + 0x2C)) = true;
			else SetChams(ent, 255, 255, 255);
		}
	}
}

/*
void Hacks::Chams()
{
	if (config.chams)
	{
		intptr_t glowObjectManager = *(intptr_t*)(client + signatures::dwGlowObjectManager);

		for (int i = 1; i < ClientEntityList->GetHighestEntityIndex(); i++)
		{
			Entity* ent = (Entity*)ClientEntityList->GetClientEntity(i);
			if (!ent->IsValid() || ent == localPlayer) continue;

			int glowIndex = *(int*)(ent + netvars::m_iGlowIndex);

			bool is_c4_owner = false;
			for (short i = 0; i < 8; i++) // Loop in m_hMyWeapons
			{
				intptr_t currentWeapon = *(intptr_t*)(ent + netvars::m_hMyWeapons + i * 0x4);
				currentWeapon = (intptr_t)ClientEntityList->GetClientEntityFromHandle(currentWeapon);
				if (!currentWeapon) continue;

				short weaponIndex = *(short*)(currentWeapon + netvars::m_iItemDefinitionIndex);
				if (weaponIndex == WEAPON_C4) is_c4_owner = true;
			}

			if (ent->IsDefusing())
			{
				SetGlow(glowObjectManager, glowIndex, 0.f, 1.f, 0.f, 1.f);
				SetChams(ent, 0, 255, 0);
			}
			else if (is_c4_owner)
			{
				SetGlow(glowObjectManager, glowIndex, 1.f, 0.5f, 0.f, 1.f);
				SetChams(ent, 255, 128, 0);
			}
			else if (ent->GetTeam() == localPlayer->GetTeam())
			{
				SetGlow(glowObjectManager, glowIndex, 0.f, 1.f, 1.f, 1.f);
				SetChams(ent, 0, 255, 255);
			}
			else
			{
				if (ent->IsScoped())
				{
					SetGlow(glowObjectManager, glowIndex, 1.f, 0.f, 1.f, 1.f);
					SetChams(ent, 255, 0, 255);
				}
				
				else if (ent->IsFlashed())
				{
					SetGlow(glowObjectManager, glowIndex, 1.f, 1.f, 1.f, 1.f);
					SetChams(ent, 255, 255, 255);
				}

				else
				{
					SetGlow(glowObjectManager, glowIndex, 1.f, 0.f, 0.f, 1.f);
					SetChams(ent, 255, 0, 0);
				}
			}

			*(bool*)((glowObjectManager)+((glowIndex * 0x38) + 0x24)) = true;
			*(bool*)((glowObjectManager)+((glowIndex * 0x38) + 0x25)) = false;
			*(bool*)((glowObjectManager)+((glowIndex * 0x38) + 0x2C)) = true;
		}
	}
}
*/

void Hacks::Radarhack()
{
	if (config.radar)
	{
		for (int i = 1; i < ClientEntityList->GetHighestEntityIndex(); i++)
		{
			Entity* ent = (Entity*)ClientEntityList->GetClientEntity(i);
			if (!ent || ent->IsDormant() || *ent->IsSpotted()) continue; // Not using !ent->IsValid() here so we can spot the bomb
			*ent->IsSpotted() = 1;
		}
	}
}

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