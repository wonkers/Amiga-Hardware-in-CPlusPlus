#include "support/gcc8_c_support.h"
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <exec/execbase.h>
#include <graphics/gfxmacros.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>
#include "Game.h"

struct ExecBase *SysBase;


int main() 
{
	SysBase = *((struct ExecBase**)4UL);

	KPrintF("Here before Game\n");
	Game game;

	game.Run();
	KPrintF("Here After Game\n");
	return 0;
}

