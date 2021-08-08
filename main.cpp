#include "Game.h"

struct ExecBase *SysBase;


int main() 
{
	SysBase = *((struct ExecBase**)4UL);

	Game game;

	game.Run();

	return 0;
}

