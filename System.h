#pragma once
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
#include "graphics.h"
#include "Copper.h"
#include "Interrupts.h"
#include "Playfield.h"

short currentFrame = 0;

namespace Hardware
{
	using namespace Copper;
	using namespace Interrupts;
	using namespace Playfield;

	//screen stuff - not used yet
    USHORT width;
    USHORT height;
    USHORT resolution;

    //backups
    UWORD SystemInts;
    UWORD SystemDMA;
    UWORD SystemADKCON;
	APTR SystemIrq;
	struct View *ActiView;

    volatile struct Custom *custom = (struct Custom*)0xdff000;
    
	void WaitVbl()
    {
        while (true) 
        {
            volatile ULONG vpos=*(volatile ULONG*)0xDFF004; //vposr
            vpos &= 0x1ff00;
            if (vpos != (0x13700))
                break;
        }
        while (true) 
        {
            volatile ULONG vpos=*(volatile ULONG*)0xDFF004; //vposr
            vpos &= 0x1ff00;
            if (vpos == (0x13700))
                break;
        }
    }
    
    void FreeSystem()
    { 
		WaitVbl();
		WaitBlit();
		custom->intena	= 0x7fff; 
		custom->intreq	= 0x7fff; 
		custom->dmacon	= 0x7fff; 

		SetInterruptHandler(SystemIrq);

		custom->cop1lc	= (ULONG)GfxBase->copinit;
		custom->cop2lc	= (ULONG)GfxBase->LOFlist;
		custom->copjmp1	= 0x7fff; 

		custom->intena	= SystemInts | 0x8000;
		custom->dmacon	= SystemDMA | 0x8000;
		custom->adkcon	= SystemADKCON | 0x8000;		

		LoadView(ActiView);
		WaitTOF();
		WaitTOF();
		WaitBlit();	
		DisownBlitter();
		Enable();
	}
    
	void TakeSystem()
    {
		ActiView = GfxBase->ActiView; //store current view
		OwnBlitter();
		WaitBlit();	
		Disable();
		
		SystemADKCON	= custom->adkconr;
		SystemInts		= custom->intenar;
		SystemDMA		= custom->dmaconr;
		custom->intena	= 0x7fff;
		custom->intreq	= 0x7fff;
		
		WaitVbl();
		WaitVbl();
		custom->dmacon	= 0x7fff;

		//set all colors black
		for(int a=0;a<32;a++)
			custom->color[a] = 0;

		LoadView(0);
		WaitTOF();
		WaitTOF();

		WaitVbl();
		WaitVbl();

		VBR			= GetVBR();
		SystemIrq	= GetInterruptHandler(); 
	}

    void Initialize()
    {
        VBR = 0;

		TakeSystem();
		WaitVbl();

		copper1 = (USHORT*)AllocMem(1024, MEMF_CHIP);

		screenSettings();

		BitplaneSettings();

		SetColours();


		// jump to copper2
		copper1[COP_COPJUMP2-1] = 0x08A;
		copper1[COP_COPJUMP2] 	= 0x7fff;

		//initialize
		custom->cop1lc 			= (ULONG)copper1;
		custom->cop2lc 			= (ULONG)copper2;
		custom->dmacon 			= DMAF_BLITTER;//disable blitter dma for copjmp bug
		custom->copjmp1 		= 0x7fff; //start coppper
		custom->dmacon 			= DMAF_SETCLR | DMAF_MASTER | DMAF_RASTER | DMAF_COPPER | DMAF_BLITTER;

    }



}