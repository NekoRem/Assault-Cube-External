#include "memory.h"
#include "offsets.h"

memory assaultcube::mem;
module assaultcube::client;

bool assaultcube::init()
{
	if (assaultcube::mem.attach("ac_client.exe", PROCESS_ALL_ACCESS))
	{
		assaultcube::client = mem.get_module("ac_client.exe");
		if (assaultcube::client.base) { return true; }
	}
	return false;
}

DWORD assaultcube::localplayer::get_ent()
{
	return mem.read_memory<DWORD>(assaultcube::client.base + 0x10F4F4);
}

DWORD assaultcube::localplayer::get_health(DWORD lp)
{
	return mem.read_memory<DWORD>(lp + health);
}

DWORD assaultcube::localplayer::get_ammo(DWORD lp)
{
	return mem.read_memory<DWORD>(lp + ammo);
}

int main()
{
	printf("attaching...\n");
	assaultcube::init();

	DWORD lp = assaultcube::localplayer::get_ent();
	printf("get_ent(): %d\n", lp);
	printf("get_health(): %d\n", assaultcube::localplayer::get_health(lp));

	while (true)
	{
		assaultcube::mem.write_memory(lp + health, 1000);
		assaultcube::mem.write_memory(lp + ammo, 1000);
	}

	getchar();
	return 0;
}