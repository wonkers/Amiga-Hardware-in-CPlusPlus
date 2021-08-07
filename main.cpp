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

// struct ExecBase *SysBase;
// volatile struct Custom *custom;
// struct DosLibrary *DOSBase;
// struct GfxBase *GfxBase;

// //backup
// static UWORD SystemInts;
// static UWORD SystemDMA;
// static UWORD SystemADKCON;
// volatile static APTR VBR=0;
// static APTR SystemIrq;
 
// struct View *ActiView;

// static APTR GetVBR(void) {
// 	APTR vbr = 0;
// 	UWORD getvbr[] = { 0x4e7a, 0x0801, 0x4e73 }; // MOVEC.L VBR,D0 RTE

// 	if (SysBase->AttnFlags & AFF_68010) 
// 		vbr = (APTR)Supervisor((ULONG (*)())getvbr);

// 	return vbr;
// }

// void SetInterruptHandler(APTR interrupt) {
// 	*(volatile APTR*)(((UBYTE*)VBR)+0x6c) = interrupt;
// }

// APTR GetInterruptHandler() {
// 	return *(volatile APTR*)(((UBYTE*)VBR)+0x6c);
// }

// //vblank begins at vpos 312 hpos 1 and ends at vpos 25 hpos 1
// //vsync begins at line 2 hpos 132 and ends at vpos 5 hpos 18 
// void WaitVbl() {
// 	while (1) {
// 		volatile ULONG vpos=*(volatile ULONG*)0xDFF004;
// 		vpos&=0x1ff00;
// 		if (vpos!=(311<<8))
// 			break;
// 	}
// 	while (1) {
// 		volatile ULONG vpos=*(volatile ULONG*)0xDFF004;
// 		vpos&=0x1ff00;
// 		if (vpos==(311<<8))
// 			break;
// 	}
// }

// inline void WaitBlt() {
// 	UWORD tst=*(volatile UWORD*)&custom->dmaconr; //for compatiblity a1000
// 	(void)tst;
// 	while (*(volatile UWORD*)&custom->dmaconr&(1<<14)) {} //blitter busy wait
// }

// void TakeSystem() {
// 	ActiView=GfxBase->ActiView; //store current view
// 	OwnBlitter();
// 	WaitBlit();	
// 	Disable();
	
// 	//Save current interrupts and DMA settings so we can restore them upon exit. 
// 	SystemADKCON=custom->adkconr;
// 	SystemInts=custom->intenar;
// 	SystemDMA=custom->dmaconr;
// 	custom->intena=0x7fff;//disable all interrupts
// 	custom->intreq=0x7fff;//Clear any interrupts that were pending
	
// 	WaitVbl();
// 	WaitVbl();
// 	custom->dmacon=0x7fff;//Clear all DMA channels

// 	//set all colors black
// 	for(int a=0;a<32;a++)
// 		custom->color[a]=0;

// 	LoadView(0);
// 	WaitTOF();
// 	WaitTOF();

// 	WaitVbl();
// 	WaitVbl();

// 	VBR=GetVBR();
// 	SystemIrq=GetInterruptHandler(); //store interrupt register
// }

// void FreeSystem() { 
// 	WaitVbl();
// 	WaitBlt();
// 	custom->intena=0x7fff;//disable all interrupts
// 	custom->intreq=0x7fff;//Clear any interrupts that were pending
// 	custom->dmacon=0x7fff;//Clear all DMA channels

// 	//restore interrupts
// 	SetInterruptHandler(SystemIrq);

// 	/*Restore system copper list(s). */
// 	custom->cop1lc=(ULONG)GfxBase->copinit;
// 	custom->cop2lc=(ULONG)GfxBase->LOFlist;
// 	custom->copjmp1=0x7fff; //start coppper

// 	/*Restore all interrupts and DMA settings. */
// 	custom->intena=SystemInts|0x8000;
// 	custom->dmacon=SystemDMA|0x8000;
// 	custom->adkcon=SystemADKCON|0x8000;

	

// 	LoadView(ActiView);
// 	WaitTOF();
// 	WaitTOF();
// 	WaitBlit();	
// 	DisownBlitter();
// 	Enable();
// }

// inline short MouseLeft(){return !((*(volatile UBYTE*)0xbfe001)&64);}	
// inline short MouseRight(){return !((*(volatile UWORD*)0xdff016)&(1<<10));}

// // DEMO - INCBIN
// volatile short frameCounter = 0;
// INCBIN(colors, "image.pal")
// INCBIN_CHIP(image, "starfield.bpl") // load image into chipmem so we can use it without copying

// // put copperlist into chip mem so we can use it without copying
// short variance = -0x3000;
//  UWORD copper2[] __attribute__((section (".MEMF_CHIP"))) = {
// 	0x1b01, 0xff00, offsetof(struct Custom, color[0]), 0x0000,
// 	0x3201, 0xff00, offsetof(struct Custom, color[0]), 0x0001, 
// 	0x4801, 0xff00, offsetof(struct Custom, color[0]), 0x0002, 
// 	0x6401, 0xff00, offsetof(struct Custom, color[0]), 0x0003, 
// 	0x7d01, 0xff00, offsetof(struct Custom, color[0]), 0x0004,
// 	0x9601, 0xff00, offsetof(struct Custom, color[0]), 0x0005,
// 	0xaf01, 0xff00, offsetof(struct Custom, color[0]), 0x0006, 
// 	0xc801, 0xff00, offsetof(struct Custom, color[0]), 0x0007, 
// 	0xe101, 0xff00, offsetof(struct Custom, color[0]), 0x0008,
// 	0xfa01, 0xff00, offsetof(struct Custom, color[0]), 0x0009, 
// 	0xe901, 0xff00, offsetof(struct Custom, color[0]), 0x000a, 
// 	0xfa01, 0xff00, offsetof(struct Custom, color[0]), 0x000b,
// 	0xff01, 0xff00, offsetof(struct Custom, color[0]), 0x025d, 
	
// 	/*0x5201 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0002,
// 	0x5301 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0003,
// 	0x5401 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0003,
// 	0x5501 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0004,
// 	0x5601 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0104,
// 	0x5901 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0105,
// 	0x5c01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0205,
// 	0x5f01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0206,
// 	0x6201 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0306,
// 	0x6501 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0307,
// 	0x6801 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0407,
// 	0x6b01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0408,
// 	0x6e01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0508,
// 	0x7101 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0509,
// 	0x7401 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0609,
// 	0x7701 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x060a,
// 	0x7a01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x070a,
// 	0x7d01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x070b,
// 	0x8001 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x080b,
// 	0x8301 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x080c,
// 	0x8601 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x090c,
// 	0x8901 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x090d,
// 	0x8c01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0a0d,
// 	0x8f01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0a0e,
// 	0x9201 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0b0e,
// 	0x9501 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0b0f,
// 	0x9801 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0c0f,
// 	0x9b01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0d0f,
// 	0x9e01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0e0f,
// 	0xa101 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0d0f,
// 	0xa401 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0c0f,
// 	0xa701 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0b0f,
// 	0xaa01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0b0e,
// 	0xad01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0a0e,
// 	0xb001 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0a0d,
// 	0xb301 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x090d,
// 	0xb601 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x090c,
// 	0xb901 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x080c,
// 	0xbc01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x080b,
// 	0xbf01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x070b,
// 	0xc201 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x070a,
// 	0xc501 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x060a,
// 	0xc801 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0609,
// 	0xcb01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0509,
// 	0xce01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0508,
// 	0xd101 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0408,
// 	0xd401 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0407,
// 	0xd701 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0307,
// 	0xda01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0306,
// 	0xdd01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0206,
// 	0xe001 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0205,
// 	0xe301 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0105,
// 	0xe601 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0104,
// 	0xe901 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0004,
// 	0xec01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0003,
// 	0xef01 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0002,
// 	0xf201 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0001,
// 	0xf501 + variance, 0xff00, offsetof(struct Custom, color[0]), 0x0000,
// */



// 	0xffff, 0xfffe // end copper list
// };

// inline USHORT* copSetPlanes(UBYTE bplPtrStart,USHORT* copListEnd,const UBYTE **planes, USHORT i) {
// 	ULONG addr=(ULONG)planes[i];

// 	*copListEnd++=offsetof(struct Custom, bplpt[i+bplPtrStart]);
// 	*copListEnd++=(UWORD)(addr>>16);
// 	*copListEnd++=offsetof(struct Custom, bplpt[i+bplPtrStart])+2;
// 	*copListEnd++=(UWORD)addr;

// 	return copListEnd;
// }



// inline USHORT* copWaitXY(USHORT *copListEnd,USHORT x,USHORT i) {
// 	*copListEnd++=(i<<8)|(x<<1)|1;	//bit 1 means wait. waits for vertical position x<<8, first raster stop position outside the left 
// 	*copListEnd++=0xfffe;
// 	return copListEnd;
// }

// inline USHORT* copWaitY(USHORT* copListEnd,USHORT i) {
// 	*copListEnd++=(i<<8)|4|1;	//bit 1 means wait. waits for vertical position x<<8, first raster stop position outside the left 
// 	*copListEnd++=0xfffe;
// 	return copListEnd;
// }

// inline USHORT* copSetColor(USHORT* copListCurrent,USHORT index,USHORT color) {
// 	*copListCurrent++=offsetof(struct Custom, color[index]);
// 	*copListCurrent++=color;
// 	return copListCurrent;
// }

// static __attribute__((interrupt)) void interruptHandler() {
// 	custom->intreq=(1<<INTB_VERTB); custom->intreq=(1<<INTB_VERTB); //reset vbl req. twice for a4000 bug.
// 	// DEMO - increment frameCounter
// 	frameCounter++;
// }

// #ifdef __cplusplus
// 	class TestClass {
// 	public:
// 		TestClass(int y) {
// 			static int x = 7;
// 			i = y + x;
// 		}

// 		int i;
// 	};

// 	TestClass staticClass(4);
// #endif

// // set up a 320x256 lowres display
// inline USHORT* screenScanDefault(USHORT* copListEnd) {
// 	const USHORT x=129;
// 	const USHORT width=640;
// 	const USHORT height=256;
// 	const USHORT y=44;
// 	const USHORT RES=4; //8=lowres,4=hires
// 	USHORT xstop = x+width;
// 	USHORT ystop = y+height;
// 	USHORT fw=(x>>1)-RES;

// 	*copListEnd++ = offsetof(struct Custom, ddfstrt);
// 	*copListEnd++ = fw;
// 	*copListEnd++ = offsetof(struct Custom, ddfstop);
// 	*copListEnd++ = 0x00d4;//fw+(((width>>4)-1)<<3);
// 	*copListEnd++ = offsetof(struct Custom, diwstrt);
// 	*copListEnd++ = 0x2c81;//x+(y<<8);
// 	*copListEnd++ = offsetof(struct Custom, diwstop);
// 	*copListEnd++ = 0x2cc1;//(xstop-256)+((ystop-256)<<8);
// 	return copListEnd;
// }
// char *lookup[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
// int main() {
// 	SysBase = *((struct ExecBase**)4UL);
// 	custom = (struct Custom*)0xdff000;

// 	// We will use the graphics library only to locate and restore the system copper list once we are through.
// 	GfxBase = (struct GfxBase *)OpenLibrary((CONST_STRPTR)"graphics.library",0);
// 	if (!GfxBase)
// 		Exit(0);

// 	// used for printing
// 	DOSBase = (struct DosLibrary*)OpenLibrary((CONST_STRPTR)"dos.library", 0);
// 	if (!DOSBase)
// 		Exit(0);

// #ifdef __cplusplus
// 	KPrintF("Hello debugger from Amiga: %ld!\n", staticClass.i);
// #else
// 	KPrintF("Hello debugger from Amiga!\n");
// #endif
// 	Write(Output(), (APTR)"Hello Followers.  Amiga copper programming sample.  1 colour register!\n", strlen("Hello Followers.  Amiga copper programming sample.  1 colour register!\n"));
// 	Delay(50);

// 	warpmode(1);
// 	// TODO: precalc stuff here
	
// 	warpmode(0);

// 	TakeSystem();
// 	WaitVbl();

// 	USHORT* copper1 = (USHORT*)AllocMem(1024, MEMF_CHIP);
// 	USHORT* copPtr = copper1;

// 	copPtr = screenScanDefault(copPtr);
// 	//enable bitplanes	
// 	*copPtr++ = offsetof(struct Custom, bplcon0);
// 	*copPtr++ = (0<<10)/*dual pf*/|(1<<9)/*color*/|((1)<<12)/*num bitplanes*/|(1<<15)/*HIRES*/;
// 	*copPtr++ = offsetof(struct Custom, bplcon1);	//scrolling
// 	*copPtr++ = 0;
// 	*copPtr++ = offsetof(struct Custom, bplcon2);	//playfied priority
// 	*copPtr++ = 1<<6;//0x24;			//Sprites have priority over playfields

// 	//set bitplane modulo
// 	*copPtr++=offsetof(struct Custom, bpl1mod); //odd planes   1,3,5
// 	*copPtr++=0;// 0x0028;//0x0026;//0;
// 	*copPtr++=offsetof(struct Custom, bpl2mod); //even  planes 2,4
// 	*copPtr++=0;// 0x0028;//0x0026;//0;

// 	// set bitplane pointers
// 	const USHORT planeSize=640/8*256;
// 	const UBYTE* planes[1];
// 	for(int a=0;a<5;a++)
// 	{
		
// 		//planes[a] = (UBYTE*)AllocMem(planeSize, MEMF_CHIP|MEMF_CLEAR);
// 		planes[a]=(UBYTE*)(image + planeSize * a);
// 	}
	
// 	for(USHORT i = 0; i < 5; i++)
// 	copPtr = copSetPlanes(0, copPtr, planes, i);

// 	// set colors
// 	for(int a=0; a < 32; a++)
// 		copPtr = copSetColor(copPtr, a, ((USHORT*)colors)[a]);

// 	copPtr = copSetColor(copPtr, 1, 0x0FF0);
// 	// jump to copper2
// 	*copPtr++ = offsetof(struct Custom, copjmp2);
// 	*copPtr++ = 0x7fff;

// 	custom->cop1lc = (ULONG)copper1;
// 	custom->cop2lc = (ULONG)copper2;
// 	custom->dmacon = DMAF_BLITTER;//disable blitter dma for copjmp bug
// 	custom->copjmp1 = 0x7fff; //start coppper
// 	custom->dmacon = DMAF_SETCLR | DMAF_MASTER | DMAF_RASTER | DMAF_COPPER | DMAF_BLITTER;

// 	// DEMO
// 	SetInterruptHandler((APTR)interruptHandler);
// 	custom->intena=(1<<INTB_SETCLR)|(1<<INTB_INTEN)|(1<<INTB_VERTB);
// 	custom->intreq=1<<INTB_VERTB;//reset vbl req
// 	bool up = false;
// 	short count = 0;
// 	while(!MouseLeft()) {
// 		WaitVbl();
// 		int f = frameCounter & 255;

// 		// WinUAE debug overlay test
// 		//debug_clear();
// 		//debug_filled_rect(f + 100, 200*2, f + 400, 220*2, 0x0000ff00); // 0x00RRGGBB
// 		//debug_text(f+ 130, 209*2, lookup[f/32], 0x00ff00ff);
// 		/*for(int g = 1; g < 59; g++)
// 		{
// 			if(!up)
// 			 	copper2[g*4] += 0x0100;
// 			else
// 				copper2[g*4] -= 0x0100;
// 		}
// 		count++;
// 		if (count == 64)
// 		{
// 			up ^= true;
// 			count = 0;
// 		}*/
		
// 	}

// 	// END
// 	for(int a=0;a<5;a++)
// 	{
// 		FreeMem((UBYTE*)planes[a], planeSize);
// 	}
// 	FreeSystem();

// 	CloseLibrary((struct Library*)DOSBase);
// 	CloseLibrary((struct Library*)GfxBase);
// }
