#include "TestGame.h"

int main()
{
	TestGame *testGame = new TestGame();

	testGame->run();

	delete testGame;

	return 0;
}
