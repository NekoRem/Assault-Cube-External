#pragma once
#include "memory.h"

namespace assaultcube
{
	extern memory mem;
	extern module client;

	bool init();

	namespace localplayer
	{
		DWORD get_ent();
		DWORD get_health(DWORD lp);
		DWORD get_ammo(DWORD lp);
	}
}

DWORD health = 0xF8;
DWORD ammo = 0x150;