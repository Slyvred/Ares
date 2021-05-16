#include "../Hacks.h"
#include "../Memory.h"
#include "ItemDefinitionIndex.h"
#include <algorithm>

void Hacks::Triggerbot(int vKey, CUserCmd* cmd)
{
	if (config.triggerbot || GetAsyncKeyState(vKey) & 0x8000)
	{
		if (cmd->buttons & CUserCmd::IN_ATTACK) return;

		int crossId = *(int*)(localPlayer + netvars::m_iCrosshairId);
		if (crossId && crossId < ClientEntityList->GetHighestEntityIndex())
		{
			Entity* ent = (Entity*)ClientEntityList->GetClientEntity(crossId);
			if (!ent->IsValid() || ent->IsImmune()) return; // In invalid return

			short weaponIndex = localPlayer->GetActiveWeapon();
			if (!weaponIndex) return;
			if ((weaponIndex == WEAPON_SSG08 || weaponIndex == WEAPON_AWP) && !localPlayer->IsScoped()) return; // If not scoping with sniper we will not shoot

			float distance = (*(Vector3*)(localPlayer + netvars::m_vecOrigin)).Distance(*(Vector3*)(ent + netvars::m_vecOrigin)); // Zeus bot
			if (weaponIndex == WEAPON_TASER && distance > 160) return;

			if (ent->GetTeam() != localPlayer->GetTeam()) // If the conditions above are met and it's an enemy or if we're in ffa we shoot
                cmd->buttons |= CUserCmd::IN_ATTACK; //*(int*)(client + signatures::dwForceAttack) = 6;
		}
	}
}

const float pi = 3.14159265358979323846f;

Vector3 CalculateAngle(Vector3 src, Vector3 dst)
{
    Vector3 angle, e = { dst.x - src.x, dst.y - src.y, dst.z - src.z };
    float eh = sqrt(e.x * e.x + e.y * e.y);
    angle.x = atan2(-e.z, eh) * 180 / pi;
    angle.y = atan2(e.y, e.x) * 180 / pi;
    angle.z = 0;
    return angle;
}

Vector3 GetBonePos(Entity* entity, int bone)
{
    intptr_t bonePtr = *(intptr_t*)(entity + netvars::m_dwBoneMatrix);
    Vector3 bonePos = { 0, 0, 0 };

    bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);
    bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);
    bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);

    return bonePos;
}

bool Hacks::IsValidTarget(Entity* ent)
{
    if (ent->IsValid() && ent->GetTeam() != localPlayer->GetTeam() &&
        ent != localPlayer && !ent->IsImmune())
    {
        return true;
    }
    else return false;
}

Entity* Hacks::GetBestTarget(Vector3* viewAngles, int bone)
{
    float oldDistance = FLT_MAX;
    float newDistance = 0;
    Entity* target = nullptr;

    for (int i = 1; i < ClientEntityList->GetHighestEntityIndex(); i++)
    {
        Entity* currEnt = (Entity*)ClientEntityList->GetClientEntity(i);
        if (IsValidTarget(currEnt))
        {
            Vector3 eyePos = *(Vector3*)(localPlayer + netvars::m_vecOrigin) + *(Vector3*)(localPlayer + netvars::m_vecViewOffset),
                angleTo = CalculateAngle(eyePos, GetBonePos(currEnt, bone));

            newDistance = viewAngles->Distance(angleTo);

            if (newDistance < oldDistance) // newDistance < fov (if we want to add an fov check)
            {
                oldDistance = newDistance;
                target = currEnt;
            }
        }
    }
    return target;
}

static Vector3 AimBotoldPunch = { 0, 0, 0 };

void Hacks::Aimbot(CUserCmd* cmd) // Aimlock actually
{
    if (config.aimlock)
    {
        if (!localPlayer) return;

        intptr_t dwClientState = *(intptr_t*)(engine + signatures::dwClientState);

        Vector3 punchAngle = *(Vector3*)(localPlayer + netvars::m_aimPunchAngle),
            eyePos = *(Vector3*)(localPlayer + netvars::m_vecOrigin) + *(Vector3*)(localPlayer + netvars::m_vecViewOffset);

        Entity* target = GetBestTarget(&cmd->viewangles, 8);
        if (!target) return;

        int shotsFired = *(int*)(localPlayer + netvars::m_iShotsFired);

        if (shotsFired > 1 || config.triggerbot) // RCS
        {
            AimBotoldPunch.x = punchAngle.x * 2;
            AimBotoldPunch.y = punchAngle.y * 2;
        }
        else
        {
            AimBotoldPunch.x = 0; // reset old punch
            AimBotoldPunch.y = 0;
        }


        Vector3 delta = (CalculateAngle(eyePos, GetBonePos(target, 8)) - AimBotoldPunch) - cmd->viewangles;

        if (GetAsyncKeyState('X') & 0x8000 || cmd->buttons & CUserCmd::IN_ATTACK)
            cmd->viewangles += delta;
    }
}

void Hacks::Legitbot(CUserCmd* cmd)
{
    if (config.legitbot)
    {
        if (!localPlayer) return;

        Vector3 punchAngle = *(Vector3*)(localPlayer + netvars::m_aimPunchAngle); // RCS
        int shotsFired = *(int*)(localPlayer + netvars::m_iShotsFired);

        if (shotsFired > 1 || config.triggerbot)
        {
            AimBotoldPunch.x = punchAngle.x * 2;
            AimBotoldPunch.y = punchAngle.y * 2;
        }
        else
        {
            AimBotoldPunch.x = 0; // reset old punch
            AimBotoldPunch.y = 0;
        }

        intptr_t dwClientState = *(intptr_t*)(engine + signatures::dwClientState); // Aimbot part
        Vector3 eyePos = *(Vector3*)(localPlayer + netvars::m_vecOrigin) + *(Vector3*)(localPlayer + netvars::m_vecViewOffset);
        Vector3 diff = { 0, 0, 0 };

        float oDistance = FLT_MAX;
        float nDistance = 0;
        Entity* target = nullptr;

        // Per weapon config system
        /* float weaponFov = fov;
         float weaponSmooth = smooth;

         switch (localPlayer->GetActiveWeapon())
         {
         case WEAPON_AWP:
             weaponFov = 2.0f;
             weaponSmooth = 5.0f;
             break;
         case WEAPON_SSG08:
             weaponFov = 1.0f;
             weaponSmooth = 2.5f;
             break;
         case WEAPON_DEAGLE:
             weaponFov = 1.0f;
             weaponSmooth = 5.0f;
             break;
         default:
             weaponFov = fov;
             weaponSmooth = smooth;
             break;
         }*/

        for (auto bone : { 8, 4, 3, 7, 6, 5 }) // Iterating trought the selected bones
        {
            target = GetBestTarget(&cmd->viewangles, bone);
            if (!target) return;

            Vector3 angle = CalculateAngle(eyePos, GetBonePos(target, bone)) - AimBotoldPunch;

            nDistance = cmd->viewangles.Distance(angle);

            if (nDistance < oDistance) // Getting the closest bone of the closest entity to crosshair
            {
                oDistance = nDistance;

                if (nDistance <= config.fov) // fov and smooth
                    diff = angle - cmd->viewangles;
            }
        }

        diff /= config.smooth; // Everything is pre calculated to we just set the view angles on attack (= faster)

        if ((cmd->buttons & CUserCmd::IN_ATTACK) && !localPlayer->IsFlashed() && target->IsVisible())
            cmd->viewangles += diff; // fov and smooth
    }
}

Vector3 RCSoldPunch = { 0, 0, 0 };
Vector3 tempAngle = { 0, 0, 0 };

void Hacks::RCS(CUserCmd* cmd)
{
    if (config.rcs)
    {
        int rcsAmounnt = 2; // amount of recoil control. 0 = none, 2 = full
        int shotsFired = *(int*)(localPlayer + netvars::m_iShotsFired);

        intptr_t dwClientState = *(intptr_t*)(engine + signatures::dwClientState);

        Vector3* viewAngles = &cmd->viewangles;//(Vector3*)(dwClientState + signatures::dwClientState_ViewAngles);
        Vector3 punchAngle = *(Vector3*)(localPlayer + netvars::m_aimPunchAngle);

        if (shotsFired > 1 && (cmd->buttons & CUserCmd::IN_ATTACK))
        {
            tempAngle.x = (viewAngles->x + RCSoldPunch.x) - (punchAngle.x * rcsAmounnt);
            tempAngle.y = (viewAngles->y + RCSoldPunch.y) - (punchAngle.y * rcsAmounnt);

            tempAngle.Normalize();

            RCSoldPunch.x = punchAngle.x * rcsAmounnt;
            RCSoldPunch.y = punchAngle.y * rcsAmounnt;

            *viewAngles = tempAngle;
        }
        else
        {
            RCSoldPunch.x = 0;
            RCSoldPunch.y = 0;
        }
    }
}