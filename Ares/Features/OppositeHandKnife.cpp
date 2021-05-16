#include "../Hacks.h"
#include "../Memory.h"

void Hacks::OppositeHandKnife(ClientFrameStage_t stage)
{
	if (config.oppositeHandKnife)
	{
		if (!localPlayer) return;

		if (stage != ClientFrameStage_t::FRAME_RENDER_START && stage != ClientFrameStage_t::FRAME_RENDER_END)
			return;

		static bool original;

		if (stage == ClientFrameStage_t::FRAME_RENDER_START)
		{
			original = cvars.cl_righthand->getInt();

			short weaponIndex = localPlayer->GetActiveWeapon();
			if (!weaponIndex) return;

			if (weaponIndex == config.KnifeCT || weaponIndex == config.KnifeT)
				cvars.cl_righthand->setValue(!original);
		}
		else cvars.cl_righthand->setValue(original);
	}
}