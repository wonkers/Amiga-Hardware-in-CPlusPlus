#include "Game.h"
#include "Libraries.h"


struct ExecBase *SysBase;
struct DosLibrary *DOSBase;
struct GfxBase *GfxBase;

//INCBIN(file1, "file.txt")

struct BitMap bitmap;
UWORD bitmapColours[32];


int main() 
{
	LibraryControl library;

	if(library.OpenLibraries())
	{
		SysBase = library.SysBase;
		GfxBase = library.GfxBase;
		DOSBase = library.DOSBase;


		Game game;

		game.Run();

	}


	return 0;
}

