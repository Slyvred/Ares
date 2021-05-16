#include "Hacks.h"
#include "Memory.h"
#include "Hooks/Hooks.h"

char* clientModeInterface = nullptr;
void* clientMode = nullptr;
CVMTHookManager* baseClientDLLVMT = nullptr;
CVMTHookManager* clientModeVMT = nullptr;
FrameStageNotify oFrameStageNotify = nullptr;
CreateMove oCreateMove = nullptr;

void Hacks::ToggleFunction(int vKey, bool& toggle)
{
	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(vKey) & 1))
	{
		toggle = !toggle;
		EngineClient->clientCmdUnrestricted("playvol buttons/arena_switch_press_02 .5", 0);
		EngineClient->clientCmdUnrestricted(toggle ? "echo [Ares] Feature enabled" : "echo [Ares] Feature disabled", 0);
	}
}

void Hacks::GetConfig()
{
	reader = new INIReader("Ares.ini");

	config.Nametag = reader->GetString("KNIVES", "Nametag", "0");
	config.KnifeCT = reader->GetInteger("KNIVES", "KnifeCT", 0);
	config.SkinCT = reader->GetInteger("KNIVES", "SkinCT", 0);
	config.KnifeT = reader->GetInteger("KNIVES", "KnifeT", 0);
	config.SkinT = reader->GetInteger("KNIVES", "SkinT", 0);

	config.chams = reader->GetBoolean("FEATURES", "Chams", 0);
	config.glow = reader->GetBoolean("FEATURES", "Glow", 0);
	config.legitbot = reader->GetBoolean("FEATURES", "Legitbot", 0);
	config.radar = reader->GetBoolean("FEATURES", "Radarhack", 0);
	config.rcs = reader->GetBoolean("FEATURES", "RCS", 0);
	config.oppositeHandKnife = reader->GetBoolean("FEATURES", "Opposite hand knife", 0);
	config.bunnyhop = reader->GetBoolean("FEATURES", "Bunnyhop", 0);
	config.triggerbot = reader->GetBoolean("FEATURES", "Triggerbot", 0);
	config.fov = reader->GetReal("FEATURES", "Legitbot fov", 0);
	config.smooth = reader->GetReal("FEATURES", "Legitbot smooth", 0);
	config.aimlock = reader->GetBoolean("FEATURES", "Aimlock", 0);
}

void Hacks::Init()
{
	client = (intptr_t)GetModuleHandle("client.dll");
	engine = (intptr_t)GetModuleHandle("engine.dll");

	ClientEntityList = (IClientEntityList*)GetInterface("client.dll", "VClientEntityList003");
	EngineClient = (IEngineClient*)GetInterface("engine.dll", "VEngineClient014");
	BaseClientDLL = (IBaseClientDLL*)GetInterface("client.dll", "VClient018");
	ModelInfoClient = (IModelInfoClient*)GetInterface("engine.dll", "VModelInfoClient004");
	Cvar = (ICvar*)GetInterface("vstdlib.dll", "VEngineCvar007");
	clientClass = BaseClientDLL->GetAllClasses();

#ifdef _DEBUG
	std::cout << "[+] Found ClientEntityList Interface at 0x" << std::hex << ClientEntityList << std::endl;
	std::cout << "[+] Found EngineClient Interface at 0x" << std::hex << EngineClient << std::endl;
	std::cout << "[+] Found BaseClientDLL Interface at 0x" << std::hex << BaseClientDLL << std::endl;
	std::cout << "[+] Found Cvar Interface at 0x" << std::hex << Cvar << std::endl;
	std::cout << "[+] Found clientClass at 0x" << std::hex << clientClass << std::endl;
#endif // DEBUG

	GetConfig(), GetSignatures(), GetNetvars(clientClass), SetConvars();

	EngineClient->clientCmdUnrestricted("clear\necho [Ares] Cheat Initialized !", 0);
	/*EngineClient->clientCmdUnrestricted("echo [Ares] Hold A to use the triggerbot or press ctrl + F1 to toggle it", 0);
	EngineClient->clientCmdUnrestricted("echo [Ares] Hold X to use the aimlock", 0);
	EngineClient->clientCmdUnrestricted("echo [Ares] Press ctrl + F2 to switch to chams !", 0);
	EngineClient->clientCmdUnrestricted("echo [Ares] Press ctrl + F3 to toggle the RCS !", 0);
	EngineClient->clientCmdUnrestricted("echo [Ares] Press ctrl + F4 to toggle the radarhack !", 0);
	EngineClient->clientCmdUnrestricted("echo [Ares] Press ctrl + F5 to toggle the legitbot !", 0);
	EngineClient->clientCmdUnrestricted("echo [Ares] Press ctrl + F6 to toggle the aimlock !", 0);*/
	EngineClient->clientCmdUnrestricted("echo [Ares] Press END to eject !\necho", 0);

	EngineClient->clientCmdUnrestricted("cl_fullupdate", 0);
}

void Hacks::Update() 
{
	if (!EngineClient->isInGame() && !EngineClient->IsHLTV())
		SetConvars();

	localPlayer = (Entity*)ClientEntityList->GetClientEntity(EngineClient->GetLocalPlayer());
	if (!localPlayer) return;

	/*
	ToggleFunction(VK_F1, config.triggerbot);
	ToggleFunction(VK_F2, config.chams);
	ToggleFunction(VK_F3, config.rcs);
	ToggleFunction(VK_F4, config.radar);
	ToggleFunction(VK_F5, config.legitbot);
	ToggleFunction(VK_F6, config.aimlock);
	*/

	Glowhack();
	Radarhack();
	NoFlash(true);
}

extern Hacks Ares;
void __stdcall hkFrameStageNotify(ClientFrameStage_t curStage)
{
	Ares.Skinchanger(curStage);
	Ares.AgentChanger(curStage);
	Ares.OppositeHandKnife(curStage);
	Ares.NoscopeCrosshair(curStage);

	oFrameStageNotify(curStage); // restore original function
}

void __stdcall hkCreateMove(float sampleTime, CUserCmd* cmd)
{
	Ares.AutoStrafe(cmd);
	Ares.RankReveal(cmd);
	Ares.RemoveCrouchCooldown(cmd);
	Ares.Moonwalk(cmd);
	Ares.Bunnyhop(cmd);
	Ares.RCS(cmd);
	Ares.Aimbot(cmd);
	Ares.Legitbot(cmd);
	Ares.Triggerbot('A', cmd);

	oCreateMove(sampleTime, cmd); // restore original function
}

void Hacks::SetupHooks()
{
	clientMode = **(void***)((*(uintptr_t**)(BaseClientDLL))[10] + 0x5);

	baseClientDLLVMT = new CVMTHookManager((DWORD**)BaseClientDLL);
	clientModeVMT = new CVMTHookManager((DWORD**)clientMode);

	oFrameStageNotify = (FrameStageNotify)baseClientDLLVMT->HookMethod((DWORD)hkFrameStageNotify, 37);
	oCreateMove = (CreateMove)clientModeVMT->HookMethod((DWORD)hkCreateMove, 24);

#ifdef _DEBUG
	std::cout << "[+] Found clientMode Interface at 0x" << std::hex << clientModeInterface << std::endl;
	std::cout << "[+] Found clientMode at 0x" << std::hex << clientMode << std::endl;
	std::cout << "[+] Found baseClientDLLVMT at 0x" << std::hex << baseClientDLLVMT << std::endl;
	std::cout << "[+] Found clientModeVMT at 0x" << std::hex << clientModeVMT << std::endl;
	std::cout << "[+] Hooked FrameStageNotify at 0x" << std::hex << oFrameStageNotify << std::endl;
	std::cout << "[+] Hooked CreateMove at 0x" << std::hex << oCreateMove << std::endl;
#endif // DEBUG
}

void Hacks::Cleanup()
{
	for (int i = 0; i < ClientEntityList->GetHighestEntityIndex(); i++)
	{
		Entity* ent = (Entity*)ClientEntityList->GetClientEntity(i);
		if (!ent->IsValid() || ent == localPlayer) continue;
		SetChams(ent, 255, 255, 255);
	}
	clientModeVMT->UnHook();
	baseClientDLLVMT->UnHook();

	delete clientModeVMT;
	delete baseClientDLLVMT;
	delete reader;
	EngineClient->clientCmdUnrestricted("cl_fullupdate", 0);
}

void Hacks::SetConvars()
{
	while (!EngineClient->isInGame()) continue;

	cvars.sv_cheats = Cvar->findVar("sv_cheats");
	cvars.sv_cheats->setValue(1);

	cvars.cl_grenadepreview = Cvar->findVar("cl_grenadepreview");
	cvars.cl_grenadepreview->setValue(1);

	cvars.cl_ragdoll_gravity = Cvar->findVar("cl_ragdoll_gravity");
	cvars.cl_ragdoll_gravity->setValue(-600);

	cvars.fog_override = Cvar->findVar("fog_override");
	cvars.fog_override->setValue(1);

	cvars.fog_enable = Cvar->findVar("fog_enable");
	cvars.fog_enable->setValue(0);

	cvars.mat_postprocess_enable = Cvar->findVar("mat_postprocess_enable");
	cvars.mat_postprocess_enable->setValue(0);

	cvars.panorama_disable_blur = Cvar->findVar("@panorama_disable_blur");
	cvars.panorama_disable_blur->setValue(1);

	cvars.weapon_debug_spread_show = Cvar->findVar("weapon_debug_spread_show");
	//cvars.weapon_debug_spread_show->setValue(3);

	cvars.viewmodel_fov = Cvar->findVar("viewmodel_fov");
	cvars.viewmodel_fov->setValue(68);

	cvars.mp_weapons_glow_on_ground = Cvar->findVar("mp_weapons_glow_on_ground");
	cvars.mp_weapons_glow_on_ground->setValue((config.glow && !config.chams) ? 1 : 0);

	cvars.cl_righthand = Cvar->findVar("cl_righthand");
}