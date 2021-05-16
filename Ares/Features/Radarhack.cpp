#include "../Hacks.h"
#include "../Memory.h"

void Hacks::Radarhack()
{
	if (config.radar)
	{
		for (int i = 1; i < ClientEntityList->GetHighestEntityIndex(); i++)
		{
			Entity* ent = (Entity*)ClientEntityList->GetClientEntity(i);
			if (!ent || ent->IsDormant() || *ent->IsSpotted()) continue; // Not using !ent->IsValid() here so spot the bomb
			*ent->IsSpotted() = 1;
		}
	}
}