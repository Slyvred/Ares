#include "../Hacks.h"
#include "../Memory.h"
#include "ItemDefinitionIndex.h"

void Hacks::AutoStrafe(CUserCmd* cmd)
{
	if (config.bunnyhop)
	{
		if (cmd->buttons & CUserCmd::IN_JUMP)
		{
			if (cmd->mousedx > 0)
				cmd->sidemove = 450.0f; // strafe right

			if (cmd->mousedx < 0)
				cmd->sidemove = -450.0f; // strafe left
		}
	}
}

void Hacks::RankReveal(CUserCmd* cmd)
{
	if (cmd->buttons & CUserCmd::IN_SCORE)
		BaseClientDLL->dispatchUserMessage(50, 0, 0, nullptr);
}

void Hacks::RemoveCrouchCooldown(CUserCmd* cmd)
{
    if (config.aimlock)
        cmd->buttons |= CUserCmd::IN_BULLRUSH;
}

void Hacks::Moonwalk(CUserCmd* cmd)
{
	if (localPlayer && localPlayer->GetMoveType() != MoveType::MOVETYPE_LADDER && config.aimlock)
		cmd->buttons ^= CUserCmd::IN_FORWARD | CUserCmd::IN_BACK | CUserCmd::IN_MOVELEFT | CUserCmd::IN_MOVERIGHT;
}

void Hacks::Bunnyhop(CUserCmd* cmd)
{
	if (!localPlayer) return;
	BYTE flags = *(BYTE*)(localPlayer + netvars::m_fFlags);
    int wasLastTimeOnGround = flags & 1;

	if (config.bunnyhop && !(flags & 1) && !wasLastTimeOnGround)
		cmd->buttons &= ~CUserCmd::IN_JUMP;

	wasLastTimeOnGround = flags & 1;
}

/*
void Hacks::NoscopeCrosshair(bool enable)
{
    if (enable)
    {
        if (localPlayer->IsValid()) cvars.weapon_debug_spread_show->setValue(localPlayer->IsScoped() ? 0 : 3);
        else cvars.weapon_debug_spread_show->setValue(0);
    }
}
*/

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

void Hacks::NoscopeCrosshair(ClientFrameStage_t stage)
{
    if (!localPlayer) return;

    if (stage != ClientFrameStage_t::FRAME_RENDER_START && stage != ClientFrameStage_t::FRAME_RENDER_END)
        return;

    if (stage == ClientFrameStage_t::FRAME_RENDER_START)
    {
        short weaponIndex = localPlayer->GetActiveWeapon();
        if (!weaponIndex) return;

        if ((weaponIndex == WEAPON_AWP || weaponIndex == WEAPON_SSG08 || weaponIndex == WEAPON_SCAR20 || weaponIndex == WEAPON_G3SG1) && !localPlayer->IsScoped())
            cvars.weapon_debug_spread_show->setValue(3);
    }
    else cvars.weapon_debug_spread_show->setValue(0);
}

#define TEAM_TERRORIST 2
#define TEAM_ANTI_TERRORIST 3

/*constexpr std::array models
{
    "models/player/custom_player/legacy/ctm_fbi_variantb.mdl",
        "models/player/custom_player/legacy/ctm_fbi_variantf.mdl",
        "models/player/custom_player/legacy/ctm_fbi_variantg.mdl",
        "models/player/custom_player/legacy/ctm_fbi_varianth.mdl",
        "models/player/custom_player/legacy/ctm_sas_variantf.mdl",
        "models/player/custom_player/legacy/ctm_st6_variante.mdl",
        "models/player/custom_player/legacy/ctm_st6_variantg.mdl",
        "models/player/custom_player/legacy/ctm_st6_varianti.mdl",
        "models/player/custom_player/legacy/ctm_st6_variantk.mdl",
        "models/player/custom_player/legacy/ctm_st6_variantm.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantf.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantg.mdl",
        "models/player/custom_player/legacy/tm_balkan_varianth.mdl",
        "models/player/custom_player/legacy/tm_balkan_varianti.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantj.mdl",
        "models/player/custom_player/legacy/tm_leet_variantf.mdl",
        "models/player/custom_player/legacy/tm_leet_variantg.mdl",
        "models/player/custom_player/legacy/tm_leet_varianth.mdl",
        "models/player/custom_player/legacy/tm_leet_varianti.mdl",
        "models/player/custom_player/legacy/tm_phoenix_variantf.mdl",
        "models/player/custom_player/legacy/tm_phoenix_variantg.mdl",
        "models/player/custom_player/legacy/tm_phoenix_varianth.mdl",

        "models/player/custom_player/legacy/tm_pirate.mdl",
        "models/player/custom_player/legacy/tm_pirate_varianta.mdl",
        "models/player/custom_player/legacy/tm_pirate_variantb.mdl",
        "models/player/custom_player/legacy/tm_pirate_variantc.mdl",
        "models/player/custom_player/legacy/tm_pirate_variantd.mdl",
        "models/player/custom_player/legacy/tm_anarchist.mdl",
        "models/player/custom_player/legacy/tm_anarchist_varianta.mdl",
        "models/player/custom_player/legacy/tm_anarchist_variantb.mdl",
        "models/player/custom_player/legacy/tm_anarchist_variantc.mdl",
        "models/player/custom_player/legacy/tm_anarchist_variantd.mdl",
        "models/player/custom_player/legacy/tm_balkan_varianta.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantb.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantc.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantd.mdl",
        "models/player/custom_player/legacy/tm_balkan_variante.mdl",
        "models/player/custom_player/legacy/tm_jumpsuit_varianta.mdl",
        "models/player/custom_player/legacy/tm_jumpsuit_variantb.mdl",
        "models/player/custom_player/legacy/tm_jumpsuit_variantc.mdl",

        "models/player/custom_player/legacy/tm_phoenix_varianti.mdl",
        "models/player/custom_player/legacy/ctm_st6_variantj.mdl",
        "models/player/custom_player/legacy/ctm_st6_variantl.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantk.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantl.mdl",
        "models/player/custom_player/legacy/ctm_swat_variante.mdl",
        "models/player/custom_player/legacy/ctm_swat_variantf.mdl",
        "models/player/custom_player/legacy/ctm_swat_variantg.mdl",
        "models/player/custom_player/legacy/ctm_swat_varianth.mdl",
        "models/player/custom_player/legacy/ctm_swat_varianti.mdl",
        "models/player/custom_player/legacy/ctm_swat_variantj.mdl",
        "models/player/custom_player/legacy/tm_professional_varf.mdl",
        "models/player/custom_player/legacy/tm_professional_varf1.mdl",
        "models/player/custom_player/legacy/tm_professional_varf2.mdl",
        "models/player/custom_player/legacy/tm_professional_varf3.mdl",
        "models/player/custom_player/legacy/tm_professional_varf4.mdl",
        "models/player/custom_player/legacy/tm_professional_varg.mdl",
        "models/player/custom_player/legacy/tm_professional_varh.mdl",
        "models/player/custom_player/legacy/tm_professional_vari.mdl",
        "models/player/custom_player/legacy/tm_professional_varj.mdl"
};*/

void Hacks::AgentChanger(ClientFrameStage_t stage)
{
    if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) return;

    if (!localPlayer) return;

    static int agentIndex;
    int* modelIndex = (int*)(localPlayer + netvars::m_nModelIndex);

    switch (localPlayer->GetTeam())
    {
    case TEAM_TERRORIST:
        agentIndex = ModelInfoClient->GetModelIndex("models/player/custom_player/legacy/tm_professional_varf4.mdl");
        break;
    case TEAM_ANTI_TERRORIST:
        agentIndex = ModelInfoClient->GetModelIndex("models/player/custom_player/legacy/ctm_st6_variantm.mdl");
        break;
    default:
        break;
    }

    if (agentIndex) *modelIndex = agentIndex;
}


Hacks::skinInfo Hacks::GetWeaponSkin(const short weaponIndex)
{
    // set our desired weapon skin values here
    skinInfo skin;
    switch (weaponIndex)
    {
    case WEAPON_DEAGLE:
        skin.paintkit = reader->GetInteger("SKINS", "DEAGLE", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "DEAGLE", 0);
        skin.nametag = reader->GetString("NAMETAG", "DEAGLE", "0");
        break;
    case WEAPON_GLOCK:
        skin.paintkit = reader->GetInteger("SKINS", "GLOCK", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "GLOCK", 0);
        skin.nametag = reader->GetString("NAMETAG", "GLOCK", "0");
        break;
    case WEAPON_AK47:
        skin.paintkit = reader->GetInteger("SKINS", "AK47", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "AK47", 0);
        skin.nametag = reader->GetString("NAMETAG", "AK47", "0");
        break;
    case WEAPON_AWP:
        skin.paintkit = reader->GetInteger("SKINS", "AWP", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "AWP", 0);
        skin.nametag = reader->GetString("NAMETAG", "AWP", "0");
        break;
    case WEAPON_M4A1:
        skin.paintkit = reader->GetInteger("SKINS", "M4A1", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "M4A1", 0);
        skin.nametag = reader->GetString("NAMETAG", "M4A1", "0");
        break;
    case WEAPON_SSG08:
        skin.paintkit = reader->GetInteger("SKINS", "SSG08", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "SSG08", 0);
        skin.nametag = reader->GetString("NAMETAG", "SSG08", "0");
        break;
    case WEAPON_M4A1_SILENCER:
        skin.paintkit = reader->GetInteger("SKINS", "M4A1_SILENCER", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "M4A1_SILENCER", 0);
        skin.nametag = reader->GetString("NAMETAG", "M4A1_SILENCER", "0");
        break;
    case WEAPON_USP_SILENCER:
        skin.paintkit = reader->GetInteger("SKINS", "USP_SILENCER", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "USP_SILENCER", 0);
        skin.nametag = reader->GetString("NAMETAG", "USP_SILENCER", "0");
        break;
    case WEAPON_NOVA:
        skin.paintkit = reader->GetInteger("SKINS", "NOVA", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "NOVA", 0);
        skin.nametag = reader->GetString("NAMETAG", "NOVA", "0");
        break;
    case WEAPON_P250:
        skin.paintkit = reader->GetInteger("SKINS", "P250", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "P250", 0);
        skin.nametag = reader->GetString("NAMETAG", "P250", "0");
        break;
    case WEAPON_SG556:
        skin.paintkit = reader->GetInteger("SKINS", "SG556", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "SG556", 0);
        skin.nametag = reader->GetString("NAMETAG", "SG556", "0");
        break;
    case WEAPON_XM1014:
        skin.paintkit = reader->GetInteger("SKINS", "XM1014", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "XM1014", 0);
        skin.nametag = reader->GetString("NAMETAG", "XM1014", "0");
        break;
    case WEAPON_UMP45:
        skin.paintkit = reader->GetInteger("SKINS", "UMP45", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "UMP45", 0);
        skin.nametag = reader->GetString("NAMETAG", "UMP45", "0");
        break;
    case WEAPON_TEC9:
        skin.paintkit = reader->GetInteger("SKINS", "TEC9", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "TEC9", 0);
        skin.nametag = reader->GetString("NAMETAG", "TEC9", "0");
        break;
    case WEAPON_SCAR20:
        skin.paintkit = reader->GetInteger("SKINS", "SCAR20", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "SCAR20", 0);
        skin.nametag = reader->GetString("NAMETAG", "SCAR20", "0");
        break;
    case WEAPON_SAWEDOFF:
        skin.paintkit = reader->GetInteger("SKINS", "SAWEDOFF", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "SAWEDOFF", 0);
        skin.nametag = reader->GetString("NAMETAG", "SAWEDOFF", "0");
        break;
    case WEAPON_REVOLVER:
        skin.paintkit = reader->GetInteger("SKINS", "REVOLVER", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "REVOLVER", 0);
        skin.nametag = reader->GetString("NAMETAG", "REVOLVER", "0");
        break;
    case WEAPON_BIZON:
        skin.paintkit = reader->GetInteger("SKINS", "BIZON", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "BIZON", 0);
        skin.nametag = reader->GetString("NAMETAG", "BIZON", "0");
        break;
    case WEAPON_P90:
        skin.paintkit = reader->GetInteger("SKINS", "P90", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "P90", 0);
        skin.nametag = reader->GetString("NAMETAG", "P90", "0");
        break;
    case WEAPON_HKP2000:
        skin.paintkit = reader->GetInteger("SKINS", "HKP2000", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "HKP2000", 0);
        skin.nametag = reader->GetString("NAMETAG", "HKP2000", "0");
        break;
    case WEAPON_NEGEV:
        skin.paintkit = reader->GetInteger("SKINS", "NEGEV", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "NEGEV", 0);
        skin.nametag = reader->GetString("NAMETAG", "NEGEV", "0");
        break;
    case WEAPON_MP9:
        skin.paintkit = reader->GetInteger("SKINS", "MP9", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "MP9", 0);
        skin.nametag = reader->GetString("NAMETAG", "MP9", "0");
        break;
    case WEAPON_MP7:
        skin.paintkit = reader->GetInteger("SKINS", "MP7", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "MP7", 0);
        skin.nametag = reader->GetString("NAMETAG", "MP7", "0");
        break;
    case WEAPON_MP5_SD:
        skin.paintkit = reader->GetInteger("SKINS", "MP5_SD", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "MP5_SD", 0);
        skin.nametag = reader->GetString("NAMETAG", "MP5_SD", "0");
        break;
    case WEAPON_MAG7:
        skin.paintkit = reader->GetInteger("SKINS", "MAG7", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "MAG7", 0);
        skin.nametag = reader->GetString("NAMETAG", "MAG7", "0");
        break;
    case WEAPON_MAC10:
        skin.paintkit = reader->GetInteger("SKINS", "MAC10", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "MAC10", 0);
        skin.nametag = reader->GetString("NAMETAG", "MAC10", "0");
        break;
    case WEAPON_M249:
        skin.paintkit = reader->GetInteger("SKINS", "M249", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "M249", 0);
        skin.nametag = reader->GetString("NAMETAG", "M249", "0");
        break;
    case WEAPON_GALILAR:
        skin.paintkit = reader->GetInteger("SKINS", "GALILAR", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "GALILAR", 0);
        skin.nametag = reader->GetString("NAMETAG", "GALILAR", "0");
        break;
    case WEAPON_G3SG1:
        skin.paintkit = reader->GetInteger("SKINS", "G3SG1", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "G3SG1", 0);
        skin.nametag = reader->GetString("NAMETAG", "G3SG1", "0");
        break;
    case WEAPON_FIVESEVEN:
        skin.paintkit = reader->GetInteger("SKINS", "FIVESEVEN", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "FIVESEVEN", 0);
        skin.nametag = reader->GetString("NAMETAG", "FIVESEVEN", "0");
        break;
    case WEAPON_FAMAS:
        skin.paintkit = reader->GetInteger("SKINS", "FAMAS", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "FAMAS", 0);
        skin.nametag = reader->GetString("NAMETAG", "FAMAS", "0");
        break;
    case WEAPON_ELITE:
        skin.paintkit = reader->GetInteger("SKINS", "ELITE", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "ELITE", 0);
        skin.nametag = reader->GetString("NAMETAG", "ELITE", "0");
        break;
    case WEAPON_AUG:
        skin.paintkit = reader->GetInteger("SKINS", "AUG", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "AUG", 0);
        skin.nametag = reader->GetString("NAMETAG", "AUG", "0");
        break;
    case WEAPON_CZ75A:
        skin.paintkit = reader->GetInteger("SKINS", "CZ75A", 0);
        skin.stattrak = reader->GetInteger("STATTRAK", "CZ75A", 0);
        skin.nametag = reader->GetString("NAMETAG", "CZ75A", "0");
        break;
    default:
        break;
    }
    return skin;
}

int Hacks::GetModelIndex(const short itemIndex)
{
    int ret = 0;
    switch (itemIndex)
    {
    case WEAPON_KNIFE:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
        break;
    case WEAPON_KNIFE_T:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_default_t.mdl");
        break;
    case WEAPON_KNIFE_BAYONET: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
        break;
    case WEAPON_KNIFE_FLIP: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_flip.mdl");
        break;
    case WEAPON_KNIFE_GUT: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_gut.mdl");
        break;
    case WEAPON_KNIFE_KARAMBIT: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_karam.mdl");
        break;
    case WEAPON_KNIFE_M9_BAYONET: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
        break;
    case WEAPON_KNIFE_TACTICAL: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_tactical.mdl");
        break;
    case WEAPON_KNIFE_FALCHION:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
        break;
    case WEAPON_KNIFE_SURVIVAL_BOWIE:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
        break;
    case WEAPON_KNIFE_BUTTERFLY:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
        break;
    case WEAPON_KNIFE_PUSH: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_push.mdl");
        break;
    case WEAPON_KNIFE_URSUS:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_ursus.mdl");
        break;
    case WEAPON_KNIFE_GYPSY_JACKKNIFE: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
        break;
    case WEAPON_KNIFE_STILETTO: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
        break;
    case WEAPON_KNIFE_WIDOWMAKER:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
        break;
    case WEAPON_KNIFE_CSS: //
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_css.mdl");
        break;
    case WEAPON_KNIFE_CORD:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_cord.mdl");
        break;
    case WEAPON_KNIFE_CANIS:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_canis.mdl");
        break;
    case WEAPON_KNIFE_OUTDOOR:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_outdoor.mdl");
        break;
    case WEAPON_KNIFE_SKELETON:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_knife_skeleton.mdl");
        break;
    case GLOVE_CT_SIDE:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl");
        break;
    case GLOVE_T_SIDE:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl");
        break;
    case GLOVE_SPORTY:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl");
        break;
    case GLOVE_HYDRA:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl");
        break;
    case GLOVE_LEATHER_WRAP:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl");
        break;
    case GLOVE_MOTORCYCLE:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl");
        break;
    case GLOVE_SLICK:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl");
        break;
    case GLOVE_SPECIALIST:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl");
        break;
    case GLOVE_STUDDED_BLOODHOUND:
        ret = ModelInfoClient->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl");
        break;
    default:
        break;
    }
    return ret;
}

void Hacks::SetStatTrak(intptr_t dwWeapon, int kills, int xuidlow)
{
    if (!kills) return;
    *(int*)(dwWeapon + netvars::m_iAccountID) = xuidlow;
    *(int*)(dwWeapon + netvars::m_iEntityQuality) = 9;
    *(int*)(dwWeapon + netvars::m_nFallbackStatTrak) = kills;
}

void Hacks::SetModel(intptr_t dwWeapon, int modelIndex, int itemIndex)
{
    *(int*)(dwWeapon + netvars::m_nModelIndex) = modelIndex;
    *(int*)(dwWeapon + netvars::m_iEntityQuality) = 3; // 3 = knife
    *(short*)(dwWeapon + netvars::m_iItemDefinitionIndex) = itemIndex;
}

void Hacks::SetPaintKit(intptr_t dwWeapon, int skin, int seed)
{
    *(int*)(dwWeapon + netvars::m_iItemIDHigh) = -1;
    *(float*)(dwWeapon + netvars::m_flFallbackWear) = 0.f;
    *(int*)(dwWeapon + netvars::m_nFallbackPaintKit) = skin;

    if (seed) *(int*)(dwWeapon + netvars::m_nFallbackSeed) = seed;
}

void Hacks::SetNameTag(intptr_t dwWeapon, std::string nametag)
{
    if (nametag != (std::string)"0")
        *(std::string*)(dwWeapon + netvars::m_szCustomName) = nametag;
}

struct ModelInfos
{
    int modelIndex = 0;
    short itemDefIndex = 0;
};

void Hacks::Skinchanger(ClientFrameStage_t stage)
{
    if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) return;
    if (!localPlayer) return;

    skinInfo weaponSkin;
    ModelInfos model;
    player_info_t playerInfo;
    EngineClient->getPlayerInfo(EngineClient->GetLocalPlayer(), playerInfo);

    for (short i = 0; i < 8; i++) // Loop in m_hMyWeapons
    {
        intptr_t currentWeapon = *(intptr_t*)(localPlayer + netvars::m_hMyWeapons + i * 0x4);
        currentWeapon = (intptr_t)ClientEntityList->GetClientEntityFromHandle(currentWeapon);
        if (!currentWeapon) continue;

        short weaponIndex = *(short*)(currentWeapon + netvars::m_iItemDefinitionIndex);

        weaponSkin = GetWeaponSkin(weaponIndex);

        if (weaponIndex == WEAPON_KNIFE || weaponIndex == WEAPON_KNIFE_T || weaponIndex == config.KnifeCT || weaponIndex == config.KnifeT) // Knife changer
        {
            weaponSkin.nametag = config.Nametag;

            if (weaponIndex == WEAPON_KNIFE || weaponIndex == config.KnifeCT)
            {
                model.itemDefIndex = config.KnifeCT;
                model.modelIndex = GetModelIndex(config.KnifeCT);
                weaponSkin.paintkit = config.SkinCT;
            }
            else if (weaponIndex == WEAPON_KNIFE_T || weaponIndex == config.KnifeT)
            {
                model.itemDefIndex = config.KnifeT;
                model.modelIndex = GetModelIndex(config.KnifeT);
                weaponSkin.paintkit = config.SkinT;
            }
            SetModel(currentWeapon, model.modelIndex, model.itemDefIndex);
        }
        SetNameTag(currentWeapon, weaponSkin.nametag);
        SetPaintKit(currentWeapon, weaponSkin.paintkit, NULL); // Skin changer
        SetStatTrak(currentWeapon, weaponSkin.stattrak, playerInfo.xuidlow);
        //if (weaponIndex == WEAPON_C4) SetNameTag(currentWeapon, "Big Yeet");
    }

    // Getting the model in use and changing it here if it's our knife
    if (short weaponIndex = localPlayer->GetActiveWeapon(); !weaponIndex || weaponIndex != model.itemDefIndex) return;

    intptr_t activeViewModel = *(intptr_t*)(localPlayer + netvars::m_hViewModel);
    activeViewModel = (intptr_t)ClientEntityList->GetClientEntityFromHandle(activeViewModel);
    *(int*)(activeViewModel + netvars::m_nModelIndex) = model.modelIndex;
}