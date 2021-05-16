#include "Entity.h"
#include "Memory.h"
#include "Hacks.h"
#include "Features/ItemDefinitionIndex.h"
extern Hacks Ares;

int Entity::GetHp()
{
	return *(int*)(this + netvars::m_iHealth);
}

int Entity::GetTeam()
{
	return *(int*)(this + netvars::m_iTeamNum);
}

bool Entity::IsDormant()
{
	return *(bool*)(this + signatures::m_bDormant);
}

bool Entity::IsValid()
{
	if (this == nullptr || this->GetHp() <= 0 || this->IsDormant()) return false;
	else return true;
}

bool Entity::IsScoped()
{
	return *(bool*)(this + netvars::m_bIsScoped);
}

bool* Entity::IsSpotted()
{
	return (bool*)(this + netvars::m_bSpotted);
}

bool Entity::IsDefusing()
{
	return *(bool*)(this + netvars::m_bIsDefusing);
}

bool Entity::IsFlashed()
{
	return *(float*)(this + netvars::m_flFlashDuration) ? true : false;
}

bool Entity::IsImmune()
{
	return *(bool*)(this + netvars::m_bGunGameImmunity);
}

bool Entity::IsAlive()
{
	return (*(LifeState*)(this + netvars::m_lifeState) == LifeState::LIFE_ALIVE) ? true : false;
}
	
MoveType Entity::GetMoveType()
{
	return *(MoveType*)(this + netvars::m_nMoveType);
}

bool Entity::IsMoving()
{
	Vector3 vel = *(Vector3*)(this + netvars::m_vecVelocity);
	return (vel.x + vel.y + vel.z) ? true : false;
}

bool Entity::IsVisible()
{
	//player_info_t playerInfo;
	//hacks->EngineClient->getPlayerInfo(hacks->EngineClient->GetLocalPlayer(), playerInfo);
	intptr_t dwClientState = *(intptr_t*)(Ares.engine + signatures::dwClientState);
	int localPlayerID = *(int*)(dwClientState + signatures::dwClientState_GetLocalPlayer);
	int spottedByMask = *(int*)(this + netvars::m_bSpottedByMask);
	
	return (spottedByMask & (1 << localPlayerID)) ? true : false;
}

int Entity::GetClassId()
{
	intptr_t dwClientNetworkable = *(intptr_t*)(this + 0x8);
	intptr_t dwGetClientClassFn = *(intptr_t*)(dwClientNetworkable + 2 * 0x4);
	intptr_t dwEntityClientClass = *(intptr_t*)(dwGetClientClassFn + 1);
	return *(int*)(dwEntityClientClass + 20);
}

short Entity::GetActiveWeapon()
{
	intptr_t activeWeapon = *(intptr_t*)(this + netvars::m_hActiveWeapon);
	activeWeapon = (intptr_t)Ares.ClientEntityList->GetClientEntityFromHandle(activeWeapon);
	
	return activeWeapon ? *(short*)(activeWeapon + netvars::m_iItemDefinitionIndex) : NULL;
}

bool Entity::IsC4Owner()
{
	bool is_c4_owner = false;

	for (short i = 0; i < 8; i++) // Loop in m_hMyWeapons
	{
		intptr_t currentWeapon = *(intptr_t*)(this + netvars::m_hMyWeapons + i * 0x4);
		currentWeapon = (intptr_t)Ares.ClientEntityList->GetClientEntityFromHandle(currentWeapon);
		if (!currentWeapon) continue;

		short weaponIndex = *(short*)(currentWeapon + netvars::m_iItemDefinitionIndex);

		if (weaponIndex == WEAPON_C4) is_c4_owner = true;
	}

	return is_c4_owner;
}