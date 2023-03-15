//#include "TestGame.h"
#include "Portals.h"
//#include "LTiles.h"

int main()
{
	Portals *portals = new Portals();

	portals->run();

	delete portals;

	return 0;
}
