#include "../Hacks.h"
#include "../Memory.h"
#include "ItemDefinitionIndex.h"

const float pi = 3.14159265358979323846f;
static Vector3 oldPunch = { 0, 0, 0 };

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

void Hacks::Aimbot() // Aimlock actually
{
    if (config.aimlock)
    {
        intptr_t dwClientState = *(intptr_t*)(engine + signatures::dwClientState);
        Vector3* viewAngles = (Vector3*)(dwClientState + signatures::dwClientState_ViewAngles);

        Vector3 punchAngle = *(Vector3*)(localPlayer + netvars::m_aimPunchAngle),
            eyePos = *(Vector3*)(localPlayer + netvars::m_vecOrigin) + *(Vector3*)(localPlayer + netvars::m_vecViewOffset);

        Entity* target = GetBestTarget(viewAngles, 8);
        if (!target) return;

        int shotsFired = *(int*)(localPlayer + netvars::m_iShotsFired);

        if (shotsFired > 1 || config.triggerbot) // RCS
        {
            oldPunch.x = punchAngle.x * 2;
            oldPunch.y = punchAngle.y * 2;
        }
        else
        {
            oldPunch.x = 0; // reset old punch
            oldPunch.y = 0;
        }

        Vector3 angle = CalculateAngle(eyePos, GetBonePos(target, 8)) - oldPunch;

        if (GetAsyncKeyState('X'))
            if (localPlayer->IsAlive()) // So we don't crash if we die while changing the viewangles
                *viewAngles = angle;
    }
}

void Hacks::Legitbot()
{
    if (config.legitbot)
    {
        Vector3 punchAngle = *(Vector3*)(localPlayer + netvars::m_aimPunchAngle); // RCS
        int shotsFired = *(int*)(localPlayer + netvars::m_iShotsFired);

        if (shotsFired > 1 || config.triggerbot)
        {
            oldPunch.x = punchAngle.x * 2;
            oldPunch.y = punchAngle.y * 2;
        }
        else
        {
            oldPunch.x = 0; // reset old punch
            oldPunch.y = 0;
        }

        intptr_t dwClientState = *(intptr_t*)(engine + signatures::dwClientState); // Aimbot part
        Vector3* viewAngles = (Vector3*)(dwClientState + signatures::dwClientState_ViewAngles);
        Vector3 eyePos = *(Vector3*)(localPlayer + netvars::m_vecOrigin) + *(Vector3*)(localPlayer + netvars::m_vecViewOffset);
        Vector3 diff = { 0, 0, 0 };

        float oDistance = FLT_MAX;
        float nDistance = 0;

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
            Entity* target = GetBestTarget(viewAngles, bone);
            if (!target || !target->IsVisible()) return;

            Vector3 angle = CalculateAngle(eyePos, GetBonePos(target, bone)) - oldPunch;

            nDistance = viewAngles->Distance(angle);

            if (nDistance < oDistance) // Getting the closest bone of the closest entity to crosshair
            {
                oDistance = nDistance;

                if (nDistance <= config.fov) // fov and smooth
                    diff = angle - *viewAngles;
            }
        }

        if (GetAsyncKeyState(VK_LBUTTON) && localPlayer->IsAlive() && !localPlayer->IsFlashed())
            *viewAngles += diff / config.smooth; // fov and smooth
    }
}