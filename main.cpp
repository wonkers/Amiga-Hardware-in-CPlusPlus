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


		BitplaneImage bpl;
        Write(Output(), (APTR)"loading", strlen("loading"));
        bpl.LoadBitMap((UBYTE*)"dh0:aus.iff", &bitmap, bitmapColours);
        Write(Output(), (APTR)"loaded", strlen("loaded"));
		
		

		Game game;

		game.Run();

	}


	return 0;
}

