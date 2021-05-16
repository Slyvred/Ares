#pragma once
#include <Windows.h>
#include "SDK/SDKIncludes.h"
#include "Inih/INIReader.h"
#include "Entity.h"
#include "SDK/Vector.h"

class Hacks 
{
public:
	struct Cvars
	{
		ConVar* sv_cheats;
		ConVar* cl_grenadepreview;
		ConVar* cl_ragdoll_gravity;
		ConVar* viewmodel_fov;
		ConVar* fog_override;
		ConVar* fog_enable;
		ConVar* mat_postprocess_enable;
		ConVar* panorama_disable_blur;
		ConVar* weapon_debug_spread_show;
		ConVar* cl_righthand;
		ConVar* mp_weapons_glow_on_ground;
	}cvars;

	struct Config
	{
		std::string Nametag = "0";
		int KnifeCT = 0;
		int SkinCT = 0;

		int KnifeT = 0;
		int SkinT = 0;

		bool triggerbot = false;
		bool chams = true;
		bool rcs = false;
		bool radar = true;
		bool legitbot = true;
		bool bunnyhop = true;
		bool glow = true;
		bool aimlock = false;
		bool oppositeHandKnife = false;
		float fov = 4.5f;
		float smooth = 25.0f;
	}config;

	struct skinInfo // We assign values here so we don't set skins on the wrong weapons ie the nades or the bomb
	{
		int paintkit = 0;
		int stattrak = 0;
		std::string nametag = "0";
	};

	/*struct Target
	{
		Entity* entity;
		Vector3 angles;
	};*/

public:
	intptr_t client = 0;
	intptr_t engine = 0;
	INIReader* reader = nullptr;
	Entity* localPlayer = nullptr;

	ICvar* Cvar = nullptr;
	ClientClass* clientClass = nullptr;
	IEngineClient* EngineClient = nullptr;
	IBaseClientDLL* BaseClientDLL = nullptr;
	IModelInfoClient* ModelInfoClient = nullptr;
	IClientEntityList* ClientEntityList = nullptr;

public:
	void Init(); // Standard functions
	void GetConfig();
	void SetupHooks();
	void Cleanup();
	void Update();
	void SetConvars();
	void ToggleFunction(int vKey, bool& toggle);

	void NoFlash(bool enable); // Standard features
	void Radarhack();
	//void ESP(bool enable);
	void Glowhack();
	//void Chams();
	void SetChams(Entity* ent, int R, int G, int B);

	void Bunnyhop(CUserCmd* cmd); // In UserCmd
	void AutoStrafe(CUserCmd* cmd);
	void RankReveal(CUserCmd* cmd);
	void RemoveCrouchCooldown(CUserCmd* cmd);
	void Triggerbot(int vKey, CUserCmd* cmd);
	void RCS(CUserCmd* cmd);
	void Moonwalk(CUserCmd* cmd);
	void Aimbot(CUserCmd* cmd);
	void Legitbot(CUserCmd* cmd);

	Entity* GetBestTarget(Vector3* viewAngles, int bone); // Aimbot stuff
	bool IsValidTarget(Entity* ent);

	void AgentChanger(ClientFrameStage_t stage); // In FrameStageNotify
	void Skinchanger(ClientFrameStage_t stage);
	void OppositeHandKnife(ClientFrameStage_t stage);
	void NoscopeCrosshair(ClientFrameStage_t stage);

	skinInfo GetWeaponSkin(const short weaponIndex); // Skinchanger stuff
	int GetModelIndex(const short itemIndex);
	void SetStatTrak(intptr_t dwWeapon, int kills, int xuidlow);
	void SetModel(intptr_t dwWeapon, int modelIndex, int itemIndex);
	void SetPaintKit(intptr_t dwWeapon, int skin, int seed);
	void SetNameTag(intptr_t dwWeapon, std::string nametag);
};