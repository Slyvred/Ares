#include "../Hacks.h"
#include "../Memory.h"

Vector3 oldPunch = { 0, 0, 0 };
Vector3 tempAngle = { 0, 0, 0 };

void Hacks::RCS()
{
    if (config.rcs)
    {
        int rcsAmounnt = 2; // amount of recoil control. 0 = none, 2 = full
        int shotsFired = *(int*)(localPlayer + netvars::m_iShotsFired);

        intptr_t dwClientState = *(intptr_t*)(engine + signatures::dwClientState);

        Vector3* viewAngles = (Vector3*)(dwClientState + signatures::dwClientState_ViewAngles);
        Vector3 punchAngle = *(Vector3*)(localPlayer + netvars::m_aimPunchAngle);

        if (shotsFired > 1 && GetAsyncKeyState(VK_LBUTTON))
        {
            tempAngle.x = (viewAngles->x + oldPunch.x) - (punchAngle.x * rcsAmounnt);
            tempAngle.y = (viewAngles->y + oldPunch.y) - (punchAngle.y * rcsAmounnt);

            tempAngle.Normalize();
            
            oldPunch.x = punchAngle.x * rcsAmounnt;
            oldPunch.y = punchAngle.y * rcsAmounnt;

            *viewAngles = tempAngle;
        }
        else
        {
            oldPunch.x = 0;
            oldPunch.y = 0;
        }
    }
}