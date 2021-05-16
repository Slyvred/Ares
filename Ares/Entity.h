#pragma once

enum class LifeState
{
	LIFE_ALIVE,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY
};

enum class MoveType
{
	MOVETYPE_NONE,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM
};

class Entity 
{
public:
	int GetHp();
	int GetTeam();
	bool IsDormant();
	bool IsValid();
	bool IsScoped();
	bool* IsSpotted();
	bool IsDefusing();
	bool IsFlashed();
	bool IsImmune();
	bool IsAlive();
	bool IsMoving();
	bool IsVisible();
	int GetClassId();
	short GetActiveWeapon();
	bool IsC4Owner();
	MoveType GetMoveType();
};