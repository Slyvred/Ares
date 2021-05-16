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

void Hacks::Chams(Entity* ent, int R, int G, int B)
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

			if (ent->IsDefusing())
			{
				SetGlow(glowObjectManager, glowIndex, 0.f, 1.f, 0.f, 1.f);
				if (config.chams) Chams(ent, 0, 255, 0);
			}
			else if (ent->GetTeam() == localPlayer->GetTeam())
			{
				SetGlow(glowObjectManager, glowIndex, 0.f, 1.f, 1.f, 1.f);
				if (config.chams) Chams(ent, 0, 255, 255);
			}
			else
			{
				if (ent->IsScoped())
				{
					SetGlow(glowObjectManager, glowIndex, 1.f, 0.f, 1.f, 1.f);
					if (config.chams) Chams(ent, 255, 0, 255);
				}
		
				else if (ent->IsFlashed())
				{
					SetGlow(glowObjectManager, glowIndex, 1.f, 1.f, 1.f, 1.f);
					if (config.chams) Chams(ent, 255, 255, 255);
				}
				else
				{
					SetGlow(glowObjectManager, glowIndex, 1.f, 0.f, 0.f, 1.f);
					if (config.chams) Chams(ent, 255, 0, 0);
				}
			}

			*(bool*)((glowObjectManager)+((glowIndex * 0x38) + 0x24)) = true;
			*(bool*)((glowObjectManager)+((glowIndex * 0x38) + 0x25)) = false;

			if (config.chams) *(bool*)((glowObjectManager)+((glowIndex * 0x38) + 0x2C)) = true;
			else Chams(ent, 255, 255, 255);
		}
	}
}
