#pragma once
#include <proto/exec.h>
#include <proto/graphics.h>
#include "Copper.h"

extern struct BitMap bitmap;
extern UWORD bitmapColours[32];

namespace Hardware
{
    namespace Playfield
    {
        using namespace Hardware::Copper;

        UWORD colors[32];
        PLANEPTR planes[5];  
        
        void screenSettings()
        {
            copper1[COP_DDFSTRT-1] 	= 0x092;
            copper1[COP_DDFSTRT] 	= 0x0038;
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
            copper1[COP_BPLCON0] 	= (0<<10) | (1<<9) | ((5) << 12);
            copper1[COP_BPLCON1-1] 	= 0x102;
            copper1[COP_BPLCON1] 	= 0;
            copper1[COP_BPLCON2-1] 	= 0x104;
            copper1[COP_BPLCON2] 	= 1 << 6;
            
            //set bitplane modulo
            copper1[COP_BPL1MOD-1] 	= 0x108;
            copper1[COP_BPL1MOD] 	= 0;
            copper1[COP_BPL2MOD-1] 	= 0x10a;
            copper1[COP_BPL1MOD] 	= 0;	

            for(int a=0;a<5;a++)
            {
                planes[a] = bitmap.Planes[a];
            }
            
            // set bitplane pointers
            for(USHORT i = 0; i < 5; i++)
            {
                copSetPlanes((PLANEPTR*)planes, i);	
            }		
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
    }
}