#include "../Hacks.h"
#include "../Memory.h"
#include "ItemDefinitionIndex.h"

extern Hacks* Ares;

Hacks::Vec2 Window;
float viewMatrix[16];
HWND hwndCSGO;
COLORREF textColor = RGB(255, 255, 255);
HBRUSH Brush;
HDC hdcCSGO;
HFONT Font;

void GetWindow(Hacks::Vec2 &window)
{
	RECT res;
	GetClientRect(hwndCSGO, &res);
	window.x = res.right;
	window.y = res.bottom;
}

void DrawFilledRect(int x, int y, int w, int h)
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdcCSGO, &rect, Brush);
}


void DrawBorderBox(int x, int y, int w, int h, int thickness)
{
	DrawFilledRect(x, y, w, thickness);
	DrawFilledRect(x, y, thickness, h);
	DrawFilledRect((x + w), y, thickness, h);
	DrawFilledRect(x, y + h, w + thickness, thickness);
}

void DrawString(int x, int y, COLORREF color, const char* text)
{
	SetTextAlign(hdcCSGO, TA_CENTER | TA_NOUPDATECP);
	SetBkColor(hdcCSGO, RGB(0, 0, 0));
	SetBkMode(hdcCSGO, TRANSPARENT);
	SetTextColor(hdcCSGO, color);
	SelectObject(hdcCSGO, Font);
	TextOutA(hdcCSGO, x, y, text, strlen(text));
	DeleteObject(Font);
}

bool WorldToScreen(Vector3 pos, Hacks::Vec2 &screen)
{
	Hacks::Vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (Window.x / 2 * NDC.x) + (NDC.x + Window.x / 2);
	screen.y = -(Window.y / 2 * NDC.y) + (NDC.y + Window.y / 2);
	return true;
}

void Hacks::ESP(bool enable)
{
	if (enable)
	{
		hwndCSGO = FindWindow(0, ("Counter-Strike: Global Offensive"));
		GetWindow(Window);
		hdcCSGO = GetDC(hwndCSGO);
		memcpy(&viewMatrix, (PBYTE*)(client + 0x4D94834), sizeof(viewMatrix));
		Vec2 vScreen;
		Brush = CreateSolidBrush(RGB(255, 000, 000));

		for (short i = 0; i < ClientEntityList->GetHighestEntityIndex(); i++)
		{
			Entity* ent = (Entity*)ClientEntityList->GetClientEntity(i);
			if (!ent || ent->IsDormant()) continue;
			Vector3 pos = *(Vector3*)(ent + netvars::m_vecOrigin);
			
			if (WorldToScreen(pos, vScreen))
			{
				switch (ent->GetClassId())
				{
				case (int)ClassId::C4:
					DrawString(vScreen.x, vScreen.y, textColor, "Dropped C4");
					break;
				case (int)ClassId::PlantedC4:
					DrawString(vScreen.x, vScreen.y, textColor, "Planted C4");
					break;
				case (int)ClassId::Awp:
					DrawString(vScreen.x, vScreen.y, textColor, "AWP");
					break;
				case (int)ClassId::CSPlayer:
					DrawBorderBox(vScreen.x, vScreen.y, 200 , pos.y - pos.x, 4);
					break;
				default:
					break;
				}
			}
		}
		DeleteObject(hdcCSGO);
		DeleteObject(Brush);
	}
}