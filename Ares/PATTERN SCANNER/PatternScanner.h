#pragma once
#include <Windows.h>
#include <Psapi.h>

MODULEINFO GetModuleInfo(const char* szModule);
intptr_t FindPattern(const char* mod, const char* pattern, const char* mask);
intptr_t Scan(const char* mod, const char* pattern, const char* mask, int offset, int extra);