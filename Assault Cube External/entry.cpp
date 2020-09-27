#include "memory.h"
#include "offsets.h"

memory assaultcube::mem;
module assaultcube::client;

bool assaultcube::init()
{
	if (assaultcube::mem.attach("ac_client.exe", PROCESS_ALL_ACCESS)) // Open process handle
	{
		assaultcube::client = mem.get_module("ac_client.exe"); // Get module address
		if (assaultcube::client.base) { return TRUE; }
	}
	return FALSE;
}

DWORD assaultcube::localplayer::get_ent() // Get local entity
{
	return mem.read_memory<DWORD>(assaultcube::client.base + 0x10F4F4);
}

DWORD assaultcube::localplayer::get_health(DWORD lp) // Get local entity health
{
	return mem.read_memory<DWORD>(lp + health);
}

DWORD assaultcube::localplayer::get_ammo(DWORD lp) // Get local entity ammo
{
	return mem.read_memory<DWORD>(lp + ammo);
}

int main()
{
	printf("attaching...\n");
	assaultcube::init();

	DWORD localent = assaultcube::localplayer::get_ent(); // Retrive local entity address
	printf("get_ent(): %d\n", localent); // Printf address
	printf("get_health(): %d\n", assaultcube::localplayer::get_health(localent));// Printf address
	printf("get_ammo(): %d\n", assaultcube::localplayer::get_ammo(localent));// Printf address

	while (TRUE) // Loop through these so the value doesn't change
	{
		assaultcube::mem.write_memory(localent + health, 1000); // Changes health value to 1000
		assaultcube::mem.write_memory(localent + ammo, 1000); // Changes ammo value to 1000
	}

	getchar();
	return FALSE;
}