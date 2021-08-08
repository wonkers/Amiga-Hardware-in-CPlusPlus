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


class Hardware
{
private:
    USHORT* copper1; 
	USHORT* copPtr;
    USHORT width;
    USHORT height;
    USHORT resolution;
    const UBYTE* planes[5];

    //struct ExecBase *SysBase;
    
    struct DosLibrary *DOSBase;
    struct GfxBase *GfxBase;

    //backup
    UWORD SystemInts;
    UWORD SystemDMA;
    UWORD SystemADKCON;
    volatile  APTR VBR;//=0;
     APTR SystemIrq;
    
    struct View *ActiView;

    void TakeSystem();
    
    APTR GetVBR(void);
    APTR GetInterruptHandler();
    USHORT* screenScanDefault(USHORT* copListEnd);
    USHORT* copSetPlanes(UBYTE bplPtrStart,USHORT* copListEnd,const UBYTE **planes, USHORT i);
    USHORT* copSetColor(USHORT* copListCurrent,USHORT index, USHORT color);
    volatile struct Custom *custom = (struct Custom*)0xdff000;
    
    
    void WaitBlt();

public:
    UWORD colors[32] ;//= {0x0000, 0xffff} ;
    
    short variance = -0x3000;

    Hardware();
    ~Hardware();
    void WaitVbl();
    void FreeSystem();
    void SetInterruptHandler(APTR interrupt);
};