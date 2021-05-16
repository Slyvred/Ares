#include "Memory.h"
#include "PATTERN SCANNER/PatternScanner.h"
#include "SDK/netvars_manager.h"

namespace signatures
{
    //std::ptrdiff_t dwForceJump;
	//std::ptrdiff_t dwForceAttack;
	std::ptrdiff_t dwGlowObjectManager;
	std::ptrdiff_t m_bDormant;
	std::ptrdiff_t dwClientState;
	//std::ptrdiff_t dwClientState_ViewAngles;
	std::ptrdiff_t dwClientState_GetLocalPlayer;
}

namespace netvars
{
	std::ptrdiff_t m_iTeamNum;
	std::ptrdiff_t m_iGlowIndex;
	std::ptrdiff_t m_iHealth;
	std::ptrdiff_t m_iCrosshairId;
	std::ptrdiff_t m_bIsDefusing;
	std::ptrdiff_t m_bIsScoped;
	std::ptrdiff_t m_bSpotted;
	std::ptrdiff_t m_flFlashDuration;
	std::ptrdiff_t m_flFlashMaxAlpha;
	std::ptrdiff_t m_fFlags;
	std::ptrdiff_t m_bGunGameImmunity;
	//std::ptrdiff_t m_bHasDefuser;
	std::ptrdiff_t m_lifeState;
	std::ptrdiff_t m_iAccountID;
	std::ptrdiff_t m_flFallbackWear;
	std::ptrdiff_t m_nFallbackPaintKit;
	std::ptrdiff_t m_iItemIDHigh;
	std::ptrdiff_t m_iEntityQuality;
	std::ptrdiff_t m_iItemDefinitionIndex;
	std::ptrdiff_t m_hActiveWeapon;
	std::ptrdiff_t m_hMyWeapons;
	std::ptrdiff_t m_nModelIndex;
	std::ptrdiff_t m_szCustomName;
	std::ptrdiff_t m_OriginalOwnerXuidLow;
	std::ptrdiff_t m_nFallbackStatTrak;
	std::ptrdiff_t m_nFallbackSeed;
	std::ptrdiff_t m_hViewModel;
	std::ptrdiff_t m_clrRender;
	std::ptrdiff_t m_iShotsFired;
	std::ptrdiff_t m_aimPunchAngle;
	std::ptrdiff_t m_iViewModelIndex;
	std::ptrdiff_t m_hMyWearables;
	std::ptrdiff_t m_vecVelocity;
	std::ptrdiff_t m_vecOrigin;
	std::ptrdiff_t m_bInitialized;
	std::ptrdiff_t m_bSpottedByMask;
	//std::ptrdiff_t m_nForceBone;
	std::ptrdiff_t m_dwBoneMatrix;
	std::ptrdiff_t m_vecViewOffset;
	std::ptrdiff_t m_nMoveType;
}

void GetSignatures()
{
	//signatures::dwForceJump = Scan("client.dll", "\x8B\x0D\x00\x00\x00\x00\x8B\xD6\x8B\xC1\x83\xCA\x02", "xx????xxxxxxx", 0x2, 0);
	//signatures::dwForceAttack = Scan("client.dll", "\x89\x0D\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\xF2\x8B\xC1\x83\xCE\x04", "xx????xx????xxxxxxx", 0x2, 0);
	signatures::m_bDormant = Scan("client.dll", "\x8A\x81\x00\x00\x00\x00\xC3\x32\xC0", "xx????xxx", 0x2, 0x8) & 0xFF;
	signatures::dwGlowObjectManager = Scan("client.dll", "\xA1\x00\x00\x00\x00\xA8\x01\x75\x4B", "x????xxxx", 0x1, 0x4);
	signatures::dwClientState = Scan("engine.dll", "\xA1\x00\x00\x00\x00\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0", "x????xxx?x?xxxx", 0x1, 0);
	//signatures::dwClientState_ViewAngles = Scan("engine.dll", "\xF3\x0F\x11\x80\x00\x00\x00\x00\xD9\x46\x04\xD9\x05", "xxxx????xxxxx", 0x4, 0) & 0xFFFF;
	signatures::dwClientState_GetLocalPlayer = Scan("engine.dll", "\x8B\x80\x00\x00\x00\x00\x40\xC3", "xx????xx", 0x2, 0) & 0xFFFF;
}

void GetNetvars(ClientClass* clientClass)
{
	netvars::m_iTeamNum = GetNetvarOffset("DT_BasePlayer", "m_iTeamNum", clientClass);
	netvars::m_iHealth = GetNetvarOffset("DT_BasePlayer", "m_iHealth", clientClass);
	netvars::m_bSpotted = GetNetvarOffset("DT_BasePlayer", "m_bSpotted", clientClass);
	netvars::m_hViewModel = GetNetvarOffset("DT_BasePlayer", "m_hViewModel[0]", clientClass);
	netvars::m_aimPunchAngle = GetNetvarOffset("DT_BasePlayer", "m_aimPunchAngle", clientClass);
	netvars::m_vecOrigin = GetNetvarOffset("DT_BasePlayer", "m_vecOrigin", clientClass);

	netvars::m_clrRender = GetNetvarOffset("DT_BaseEntity", "m_clrRender", clientClass);
	netvars::m_bSpottedByMask = GetNetvarOffset("DT_BaseEntity", "m_bSpottedByMask", clientClass);

	netvars::m_fFlags = GetNetvarOffset("DT_CSPlayer", "m_fFlags", clientClass);
	netvars::m_bIsScoped = GetNetvarOffset("DT_CSPlayer", "m_bIsScoped", clientClass);
	netvars::m_bGunGameImmunity = GetNetvarOffset("DT_CSPlayer", "m_bGunGameImmunity", clientClass);
	//netvars::m_bHasDefuser = GetNetvarOffset("DT_CSPlayer", "m_bHasDefuser", clientClass);
	netvars::m_flFlashDuration = GetNetvarOffset("DT_CSPlayer", "m_flFlashDuration", clientClass);
	netvars::m_flFlashMaxAlpha = GetNetvarOffset("DT_CSPlayer", "m_flFlashMaxAlpha", clientClass);
	netvars::m_bIsDefusing = GetNetvarOffset("DT_CSPlayer", "m_bIsDefusing", clientClass);
	netvars::m_lifeState = GetNetvarOffset("DT_CSPlayer", "m_lifeState", clientClass);
	netvars::m_vecVelocity = GetNetvarOffset("DT_CSPlayer", "m_vecVelocity[0]", clientClass);
	netvars::m_vecViewOffset = GetNetvarOffset("DT_CSPlayer", "m_vecViewOffset[0]", clientClass);
	netvars::m_iShotsFired = GetNetvarOffset("DT_CSPlayer", "m_iShotsFired", clientClass);

	netvars::m_iCrosshairId = GetNetvarOffset("DT_CSPlayer", "m_bHasDefuser", clientClass) + 92;
	netvars::m_iGlowIndex = GetNetvarOffset("DT_CSPlayer", "m_flFlashDuration", clientClass) + 24;
	netvars::m_dwBoneMatrix = GetNetvarOffset("DT_BaseAnimating", "m_nForceBone", clientClass) + 28;

	netvars::m_iAccountID = GetNetvarOffset("DT_BaseAttributableItem", "m_iAccountID", clientClass);
	netvars::m_flFallbackWear = GetNetvarOffset("DT_BaseAttributableItem", "m_flFallbackWear", clientClass);
	netvars::m_nFallbackPaintKit = GetNetvarOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit", clientClass);
	netvars::m_iItemIDHigh = GetNetvarOffset("DT_BaseAttributableItem", "m_iItemIDHigh", clientClass);
	netvars::m_iEntityQuality = GetNetvarOffset("DT_BaseAttributableItem", "m_iEntityQuality", clientClass);
	netvars::m_iItemDefinitionIndex = GetNetvarOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex", clientClass);
	netvars::m_szCustomName = GetNetvarOffset("DT_BaseAttributableItem", "m_szCustomName", clientClass);
	netvars::m_OriginalOwnerXuidLow = GetNetvarOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow", clientClass);
	netvars::m_nFallbackStatTrak = GetNetvarOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak", clientClass);
	netvars::m_nFallbackSeed = GetNetvarOffset("DT_BaseAttributableItem", "m_nFallbackSeed", clientClass);
	netvars::m_iAccountID = GetNetvarOffset("DT_BaseAttributableItem", "m_iAccountID", clientClass);
	netvars::m_bInitialized = GetNetvarOffset("DT_BaseAttributableItem", "m_bInitialized", clientClass);

	netvars::m_hActiveWeapon = GetNetvarOffset("DT_BaseCombatCharacter", "m_hActiveWeapon", clientClass);
	netvars::m_hMyWeapons = GetNetvarOffset("DT_BaseCombatCharacter", "m_hMyWeapons", clientClass);
	netvars::m_hMyWearables = GetNetvarOffset("DT_BaseCombatCharacter", "m_hMyWearables", clientClass);

	netvars::m_nModelIndex = GetNetvarOffset("DT_BaseViewModel", "m_nModelIndex", clientClass);
	//netvars::m_nForceBone = GetNetvarOffset("DT_BaseAnimating", "m_nForceBone", clientClass);
	netvars::m_iViewModelIndex = GetNetvarOffset("DT_BaseCombatWeapon", "m_iViewModelIndex", clientClass);
	netvars::m_nMoveType = GetNetvarOffset("DT_BaseEntity", "m_nRenderMode", clientClass) + 1;
}