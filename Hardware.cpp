#include "Hardware.h"


UWORD copper2[] __attribute__((section (".MEMF_CHIP"))) = {
	0x1b01, 0xff00, offsetof(struct Custom, color[0]), 0x0000,
	0x3201, 0xff00, offsetof(struct Custom, color[0]), 0x0001, 
	0x4801, 0xff00, offsetof(struct Custom, color[0]), 0x0002, 
	0x6401, 0xff00, offsetof(struct Custom, color[0]), 0x0003, 
	0x7d01, 0xff00, offsetof(struct Custom, color[0]), 0x0004,
	0x9601, 0xff00, offsetof(struct Custom, color[0]), 0x0005,
	0xaf01, 0xff00, offsetof(struct Custom, color[0]), 0x0006, 
	0xc801, 0xff00, offsetof(struct Custom, color[0]), 0x0007, 
	0xe101, 0xff00, offsetof(struct Custom, color[0]), 0x0008,
	0xfa01, 0xff00, offsetof(struct Custom, color[0]), 0x0009, 
	0xe901, 0xff00, offsetof(struct Custom, color[0]), 0x000a, 
	0xfa01, 0xff00, offsetof(struct Custom, color[0]), 0x000b,
	0xff01, 0xff00, offsetof(struct Custom, color[0]), 0x025d, 

	0xffff, 0xfffe // end copper list
};
Hardware::Hardware()
{

	//INCBIN(colors, "image.pal")
	VBR = 0;
	//SysBase = *((struct ExecBase**)4UL);
	

	// We will use the graphics library only to locate and restore the system copper list once we are through.
	GfxBase = (struct GfxBase *)OpenLibrary((CONST_STRPTR)"graphics.library",0);
	if (!GfxBase)
		Exit(0);
	

	// used for printing
	DOSBase = (struct DosLibrary*)OpenLibrary((CONST_STRPTR)"dos.library", 0);
	if (!DOSBase)
		Exit(0);

    TakeSystem();
	WaitVbl();

	copper1 = (USHORT*)AllocMem(1024, MEMF_CHIP);
	copPtr = copper1;

	copPtr = screenScanDefault(copPtr);
	//enable bitplanes	
	*copPtr++ = offsetof(struct Custom, bplcon0);
	*copPtr++ = (0<<10)/*dual pf*/|(1<<9)/*color*/|((1)<<12)/*num bitplanes*/|(1<<15)/*HIRES*/;
	*copPtr++ = offsetof(struct Custom, bplcon1);	//scrolling
	*copPtr++ = 0;
	*copPtr++ = offsetof(struct Custom, bplcon2);	//playfied priority
	*copPtr++ = 1<<6;//0x24;			//Sprites have priority over playfields

	//set bitplane modulo
	*copPtr++=offsetof(struct Custom, bpl1mod); //odd planes   1,3,5
	*copPtr++=0;// 0x0028;//0x0026;//0;
	*copPtr++=offsetof(struct Custom, bpl2mod); //even  planes 2,4
	*copPtr++=0;// 0x0028;//0x0026;//0;

	// set bitplane pointers
	const USHORT planeSize=640/8*256;
	const UBYTE* planes[1];
	
	for(int a=0;a<5;a++)
	{
		planes[a] = (UBYTE*)AllocMem(planeSize, MEMF_CHIP|MEMF_CLEAR);
		//planes[a]=(UBYTE*)(image + planeSize * a);
	}
	
	for(USHORT i = 0; i < 5; i++)
		copPtr = copSetPlanes(0, copPtr, planes, i);

	// set colors
	for(int a=0; a < 32; a++)
		copPtr = copSetColor(copPtr, a, ((USHORT*)colors)[a]);

	copPtr = copSetColor(copPtr, 1, 0x0FF0);
	// jump to copper2
	*copPtr++ = offsetof(struct Custom, copjmp2);
	*copPtr++ = 0x7fff;

	custom->cop1lc = (ULONG)copper1;
	custom->cop2lc = (ULONG)copper2;
	custom->dmacon = DMAF_BLITTER;//disable blitter dma for copjmp bug
	custom->copjmp1 = 0x7fff; //start coppper
	custom->dmacon = DMAF_SETCLR | DMAF_MASTER | DMAF_RASTER | DMAF_COPPER | DMAF_BLITTER;

	

}
Hardware::~Hardware()
{
	
	//FreeMem(copper1, copperSize);
}
void Hardware::TakeSystem() 
{
	ActiView = GfxBase->ActiView; //store current view
	OwnBlitter();
	WaitBlit();	
	Disable();
	
	//Save current interrupts and DMA settings so we can restore them upon exit. 
	SystemADKCON=custom->adkconr;
	SystemInts=custom->intenar;
	SystemDMA=custom->dmaconr;
	custom->intena=0x7fff;//disable all interrupts
	custom->intreq=0x7fff;//Clear any interrupts that were pending
	
	WaitVbl();
	WaitVbl();
	custom->dmacon=0x7fff;//Clear all DMA channels

	//set all colors black
	for(int a=0;a<32;a++)
		custom->color[a]=0;

	LoadView(0);
	WaitTOF();
	WaitTOF();

	WaitVbl();
	WaitVbl();

	VBR=GetVBR();
	SystemIrq=GetInterruptHandler(); //store interrupt register
}
void Hardware::WaitVbl() 
{
	while (1) {
		volatile ULONG vpos=*(volatile ULONG*)0xDFF004;
		vpos&=0x1ff00;
		if (vpos!=(311<<8))
			break;
	}
	while (1) {
		volatile ULONG vpos=*(volatile ULONG*)0xDFF004;
		vpos&=0x1ff00;
		if (vpos==(311<<8))
			break;
	}
}
APTR Hardware::GetVBR(void) {
	APTR vbr = 0;
	UWORD getvbr[] = { 0x4e7a, 0x0801, 0x4e73 }; // MOVEC.L VBR,D0 RTE

	if (SysBase->AttnFlags & AFF_68010) 
		vbr = (APTR)Supervisor((ULONG (*)())getvbr);

	return vbr;
}
APTR Hardware::GetInterruptHandler() {
	return *(volatile APTR*)(((UBYTE*)VBR)+0x6c);
}

USHORT* Hardware::screenScanDefault(USHORT* copListEnd) {
	const USHORT x=129;
	//const USHORT width=640;
	//const USHORT height=256;
	//const USHORT y=44;
	const USHORT RES=4; //8=lowres,4=hires
	//USHORT xstop = x+width;
	//USHORT ystop = y+height;
	USHORT fw=(x>>1)-RES;

	*copListEnd++ = offsetof(struct Custom, ddfstrt);
	*copListEnd++ = fw;
	*copListEnd++ = offsetof(struct Custom, ddfstop);
	*copListEnd++ = 0x00d4;//fw+(((width>>4)-1)<<3);
	*copListEnd++ = offsetof(struct Custom, diwstrt);
	*copListEnd++ = 0x2c81;//x+(y<<8);
	*copListEnd++ = offsetof(struct Custom, diwstop);
	*copListEnd++ = 0x2cc1;//(xstop-256)+((ystop-256)<<8);
	return copListEnd;
}

USHORT* Hardware::copSetPlanes(UBYTE bplPtrStart,USHORT* copListEnd,const UBYTE **planes, USHORT i) {
	ULONG addr=(ULONG)planes[i];

	*copListEnd++=offsetof(struct Custom, bplpt[i+bplPtrStart]);
	*copListEnd++=(UWORD)(addr>>16);
	*copListEnd++=offsetof(struct Custom, bplpt[i+bplPtrStart])+2;
	*copListEnd++=(UWORD)addr;

	return copListEnd;
}

USHORT* Hardware::copSetColor(USHORT* copListCurrent,USHORT index,USHORT color) {
	*copListCurrent++=offsetof(struct Custom, color[index]);
	*copListCurrent++=color;
	return copListCurrent;
}

void Hardware::FreeSystem() { 
	WaitVbl();
	WaitBlt();
	custom->intena=0x7fff;//disable all interrupts
	custom->intreq=0x7fff;//Clear any interrupts that were pending
	custom->dmacon=0x7fff;//Clear all DMA channels

	//restore interrupts
	SetInterruptHandler(SystemIrq);

	/*Restore system copper list(s). */
	custom->cop1lc=(ULONG)GfxBase->copinit;
	custom->cop2lc=(ULONG)GfxBase->LOFlist;
	custom->copjmp1=0x7fff; //start coppper

	/*Restore all interrupts and DMA settings. */
	custom->intena=SystemInts|0x8000;
	custom->dmacon=SystemDMA|0x8000;
	custom->adkcon=SystemADKCON|0x8000;

	

	LoadView(ActiView);
	WaitTOF();
	WaitTOF();
	WaitBlit();	
	DisownBlitter();
	Enable();
}

void Hardware::WaitBlt() {
	UWORD tst=*(volatile UWORD*)&custom->dmaconr; //for compatiblity a1000
	(void)tst;
	while (*(volatile UWORD*)&custom->dmaconr&(1<<14)) {} //blitter busy wait
}

void Hardware::SetInterruptHandler(APTR interrupt) {
	*(volatile APTR*)(((UBYTE*)VBR)+0x6c) = interrupt;
}




