#include "TerrainGeneration.h"

int main()
{
	TerrainGeneration *terrainGeneration = new TerrainGeneration();

	terrainGeneration->run();

	delete terrainGeneration;

	return 0;
}
