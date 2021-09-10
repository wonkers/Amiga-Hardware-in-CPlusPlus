#pragma once
#include <proto/exec.h>

namespace Hardware
{
	namespace Copper
	{
		#define COP_DDFSTRT 1
		#define COP_DDFSTOP 3
		#define COP_DIWSTRT 5
		#define COP_DIWSTOP 7
		#define COP_BPLCON0 9
		#define COP_BPLCON1 11
		#define COP_BPLCON2 13
		#define COP_BPL1MOD 15
		#define COP_BPL2MOD 17

		#define COP_BPL1PTH 19
		#define COP_BPL1PTL 21
		#define COP_BPL2PTH 23
		#define COP_BPL2PTL 25
		#define COP_BPL3PTH 27
		#define COP_BPL3PTL 29
		#define COP_BPL4PTH 31
		#define COP_BPL4PTL 33
		#define COP_BPL5PTH 35
		#define COP_BPL5PTL 37

		#define COP_COLOR0 39
		#define COP_COLOR1 41
		#define COP_COLOR3 43
		#define COP_COLOR4 45
		#define COP_COLOR5 47
		#define COP_COLOR6 49
		#define COP_COLOR7 51
		#define COP_COLOR8 53
		#define COP_COLOR9 55
		#define COP_COLOR10 57
		#define COP_COLOR11 59
		#define COP_COLOR12 61
		#define COP_COLOR13 63
		#define COP_COLOR14 65
		#define COP_COLOR15 67
		#define COP_COLOR16 69
		#define COP_COLOR17 71
		#define COP_COLOR18 73
		#define COP_COLOR19 75
		#define COP_COLOR20 77
		#define COP_COLOR21 79
		#define COP_COLOR22 81
		#define COP_COLOR23 83
		#define COP_COLOR24 85
		#define COP_COLOR25 87
		#define COP_COLOR26 89
		#define COP_COLOR27 91
		#define COP_COLOR28 93
		#define COP_COLOR29 95
		#define COP_COLOR30 97
		#define COP_COLOR31 99

		#define COP_COPJUMP2 101

        USHORT* copper1;
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

            0xffff, 0xfffe // end copper list*/
        };

        void copSetPlanes(PLANEPTR *planes, USHORT i)
        {
            ULONG addr=(ULONG)planes[i];

            copper1[COP_BPL1PTH - 1 + (i*4)] 	= 0x0E0 + (i*4);
            copper1[COP_BPL1PTH + (i*4)] 		= (UWORD)(addr>>16);
            copper1[COP_BPL1PTL - 1 + (i*4)] 	= 0x0e2 + (i*4);
            copper1[COP_BPL1PTL + (i*4)] 		= (UWORD)addr;
        }

        void copSetColor(USHORT index, USHORT color)
        {
            copper1[COP_COLOR0 - 1 + (index*2)] 	= 0x180 + (index *2);
            copper1[COP_COLOR0 + (index*2)] 		= color;
        }

        void UpdateCopper(UWORD position, UWORD value)
        {
            copper2[position] = value;
        }
	}
}