#pragma once
#include <Windows.h>
#include "SDK/netvars_manager.h"

namespace signatures
{
	//extern std::ptrdiff_t dwForceJump;
	//extern std::ptrdiff_t dwForceAttack;
	extern std::ptrdiff_t dwGlowObjectManager;
	extern std::ptrdiff_t m_bDormant;
	extern std::ptrdiff_t dwClientState;
	//extern std::ptrdiff_t dwClientState_ViewAngles;
	extern std::ptrdiff_t dwClientState_GetLocalPlayer;
}

namespace netvars
{
	extern std::ptrdiff_t m_iTeamNum;
	extern std::ptrdiff_t m_iGlowIndex;
	extern std::ptrdiff_t m_iHealth;
	extern std::ptrdiff_t m_iCrosshairId;
	extern std::ptrdiff_t m_bIsDefusing;
	extern std::ptrdiff_t m_bIsScoped;
	extern std::ptrdiff_t m_bSpotted;
	extern std::ptrdiff_t m_flFlashDuration;
	extern std::ptrdiff_t m_flFlashMaxAlpha;
	extern std::ptrdiff_t m_fFlags;
	extern std::ptrdiff_t m_bGunGameImmunity;
	//extern std::ptrdiff_t m_bHasDefuser;
	extern std::ptrdiff_t m_lifeState;
	extern std::ptrdiff_t m_iAccountID;
	extern std::ptrdiff_t m_flFallbackWear;
	extern std::ptrdiff_t m_nFallbackPaintKit;
	extern std::ptrdiff_t m_iItemIDHigh;
	extern std::ptrdiff_t m_iEntityQuality;
	extern std::ptrdiff_t m_iItemDefinitionIndex;
	extern std::ptrdiff_t m_hActiveWeapon;
	extern std::ptrdiff_t m_hMyWeapons;
	extern std::ptrdiff_t m_nModelIndex;
	extern std::ptrdiff_t m_szCustomName;
	extern std::ptrdiff_t m_OriginalOwnerXuidLow;
	extern std::ptrdiff_t m_nFallbackStatTrak;
	extern std::ptrdiff_t m_nFallbackSeed;
	extern std::ptrdiff_t m_hViewModel;
	extern std::ptrdiff_t m_iShotsFired;
	extern std::ptrdiff_t m_clrRender;
	extern std::ptrdiff_t m_aimPunchAngle;
	extern std::ptrdiff_t m_iViewModelIndex;
	extern std::ptrdiff_t m_aimPunchAngle;
	extern std::ptrdiff_t m_hMyWearables;
	extern std::ptrdiff_t m_vecVelocity;
	extern std::ptrdiff_t m_vecOrigin;
	extern std::ptrdiff_t m_bInitialized;
	extern std::ptrdiff_t m_bSpottedByMask;
	//extern std::ptrdiff_t m_nForceBone;
	extern std::ptrdiff_t m_dwBoneMatrix;
	extern std::ptrdiff_t m_vecViewOffset;
	extern std::ptrdiff_t m_nMoveType;
}

void GetSignatures();
void GetNetvars(ClientClass* clientClass);