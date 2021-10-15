#pragma once
#include <proto/exec.h>
#include <proto/graphics.h>
#include "Copper.h"


extern UWORD bitmapColours[32];
extern struct GfxBase *GfxBase;

namespace Hardware
{
    namespace Playfield
    {
        using namespace Hardware::Copper;

        UWORD colors[32];
        PLANEPTR bplpt;
        struct BitMap MainBitMap;
        PLANEPTR planes[5];  
        
        void screenSettings()
        {
            copper1[COP_DDFSTRT-1] 	= 0x092;
            copper1[COP_DDFSTRT] 	= 0x0030;
            copper1[COP_DDFSTOP-1] 	= 0x094;
            copper1[COP_DDFSTOP] 	= 0x00d0;
            copper1[COP_DIWSTRT-1] 	= 0x08e;
            copper1[COP_DIWSTRT] 	= 0x2c81;
            copper1[COP_DIWSTOP-1] 	= 0x90;
            copper1[COP_DIWSTOP] 	= 0x2cc1;

        }   

        void BitplaneSettings()
        {
            //enable bitplanes	
            copper1[COP_BPLCON0-1] 	= 0x100;
            copper1[COP_BPLCON0] 	= (0<<15)| (0<<10) | (1<<9) | ((5) << 12);
            copper1[COP_BPLCON1-1] 	= 0x102;
            copper1[COP_BPLCON1] 	= 0xff;
            copper1[COP_BPLCON2-1] 	= 0x104;
            copper1[COP_BPLCON2] 	= 0x24;
            
            //set bitplane modulo
            copper1[COP_BPL1MOD-1] 	= 0x108;
            copper1[COP_BPL1MOD] 	= 38;
            copper1[COP_BPL2MOD-1] 	= 0x10a;
            copper1[COP_BPL2MOD] 	= 38;

            InitBitMap(&MainBitMap, 5, 640, 512);


            for(int a=0;a<5;a++)
            {
                //planes[a] = bitmap.Planes[a];
                MainBitMap.Planes[a] = (PLANEPTR)AllocMem(512*640/8, MEMF_CHIP|MEMF_CLEAR);
               // MainBitMap.Planes[a] = AllocRaster(320, 256);
                
            }
            
            // set bitplane pointers
            for(USHORT i = 0; i < 5; i++)
            {
                copSetPlanes((PLANEPTR*)MainBitMap.Planes, i);	
            }	

            bplpt = MainBitMap.Planes[0];	
        }

        void SetColours()
        {
            for(int i = 0; i < 32; i++)
            {
                colors[i] = bitmapColours[i];
            }
            // set colors
            for(int a=0; a < 32; a++)
                copSetColor(a, ((USHORT*)colors)[a]);
        }

        void ScrollVertically(WORD value)
        {
            for(int i = 0; i < 5; i++)
            {
                UWORD high = 0;
                UWORD low = 0;
                ULONG addr = 0;
                high = Hardware::Copper::copper1[COP_BPL1PTH + (i*4)];
                low = Hardware::Copper::copper1[COP_BPL1PTL + (i*4)];
                addr = high;
                addr <<= 16;
                addr |= low;

                addr += (MainBitMap.BytesPerRow * value);
                Hardware::Copper::copper1[COP_BPL1PTH + (i*4)] = (UWORD)(addr>>16);
                Hardware::Copper::copper1[COP_BPL1PTL + (i*4)] = (UWORD)addr;
            }
        }
        void ScrollHorizontally(WORD value)
        {
            UWORD currentValue = copper1[COP_BPLCON1];  
            //copper1[COP_BPLCON1] -= (0x11 * value);          

            if(currentValue == 0x0000 && value > 0)
            {
                for(int i = 0; i < 5; i++)
                {
                    UWORD high = copper1[COP_BPL1PTH + (i*4)];
                    UWORD low = copper1[COP_BPL1PTL + (i*4)];
                    ULONG addr = high;
                    addr <<= 16;
                    addr |= low;

                    addr += (value*2);

                    copper1[COP_BPL1PTH + (i*4)] = (addr>>16);
                    copper1[COP_BPL1PTL + (i*4)] = (UWORD)addr;
                }
                copper1[COP_BPLCON1] = 0xff;
            }
            else if(currentValue == 0xff && value < 0)
            {
                for(int i = 0; i < 5; i++)
                {
                    UWORD high = copper1[COP_BPL1PTH + (i*4)];
                    UWORD low = copper1[COP_BPL1PTL + (i*4)];
                    ULONG addr = high;
                    addr <<= 16;
                    addr |= low;

                    addr += (value*2);
                    copper1[COP_BPL1PTH + (i*4)] = (addr>>16);
                    copper1[COP_BPL1PTL + (i*4)] = (UWORD)addr;
                }
                copper1[COP_BPLCON1] = 0x00;
            }
            else
            {
                copper1[COP_BPLCON1] -= (0x11 * value);
            }
        }
    }
}