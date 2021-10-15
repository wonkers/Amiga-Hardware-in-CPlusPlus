#pragma once
#include <proto/exec.h>
#include "System.h"

extern short currentFrame;

namespace Hardware
{
    namespace Interrupts
    {
        volatile  APTR VBR;
        
        void SetInterruptHandler(APTR interrupt)
        {
            //0x6c = interrupt level 3
            ////0x64-0x83 level 1-7 interrupts.
            //level 1 - disk block finished/serial-buffer empty interrupt
            //level 3 - copper/blit interrupt
            //level 4 - audio interrupt
            //level 5 - disk(sync)/Serial-buffer full interrupt
            *(volatile APTR*)(((UBYTE*)VBR)+0x6c) = interrupt;
        }

        APTR GetVBR(void) 
        {
            APTR vbr = 0;

            //VBR can be accessed from supervisor mode with he MOVEC function
            UWORD getvbr[] = { 0x4e7a, 0x0801, 0x4e73 }; // MOVEC.L VBR,D0 RTE

            if (SysBase->AttnFlags & AFF_68010) 
                vbr = (APTR)Supervisor((ULONG (*)())getvbr);

            return vbr;
        }

        APTR GetInterruptHandler()
        {
            //gets level 3 interrupt handler
            return *(volatile APTR*)(((UBYTE*)VBR)+0x6c);
        }

        //handle only Vertical Blank interrupts
        //have 25 horizontal lines to make updates.
        //227.5 color clocks per horizontal line.
        //4 cycles memory refresh
        //3 for disk dma
        //4 for audio dmo (2 bytes per channel)
        //16 sprite dma (2 words per channel)
        //80 for bitplane dma
        //high def takes all time - locking out 68k.  unless horizontal blank/vertical blank
        static __attribute__((interrupt)) void interruptHandler() 
        {
            volatile struct Custom *custom = (struct Custom*)0xdff000;
            custom->intreq=(1<<INTB_VERTB); 
            custom->intreq=(1<<INTB_VERTB); //reset vbl req. twice for a4000 bug.
            
            //REWRITE the SPRITE POINTERS here
            //custom->spr0pth = pointerToSpirte0data
            custom->sprpt[6] = 0;// healspell.sprData;
            custom->sprpt[4] = 0;//entityGhost.sprites[0].sprData;
            custom->sprpt[5] = 0;//entityGhost.sprites[1].sprData;
            custom->sprpt[7] = 0;
            //the current frame from 0-255
            //use it for debugging and/or animation
            currentFrame++;
            currentFrame &= 255;

            // UWORD col = custom->clxdat;
            // if((col & 0x0002) & 0x0002)
            // {
            //     KPrintF("hi");
            // }
        }
    }
}