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

		#define COP_COPJUMP2 117

		#define COP_SPRPT00 101
		#define COP_SPRPT01 103
		#define COP_SPRPT02 105
		#define COP_SPRPT03 107
		#define COP_SPRPT04 109
		#define COP_SPRPT05 111
		#define COP_SPRPT06 113
		#define COP_SPRPT07 115

		#define COP_CLXCON	117

        USHORT* copper1;
        UWORD copper2[] __attribute__((section (".MEMF_CHIP"))) = {
			/*0x1b01, 0xff00, offsetof(struct Custom, color[0]), 0x000,
			0x3201, 0xff00, offsetof(struct Custom, color[0]), 0x010, 
			0x4801, 0xff00, offsetof(struct Custom, color[0]), 0x020, 
			0x6431, 0xfffe, offsetof(struct Custom, color[0]), 0x030, 
			0x7d01, 0xff00, offsetof(struct Custom, color[0]), 0x040,
			0x9601, 0xff00, offsetof(struct Custom, color[0]), 0x050,
			0xaf01, 0xff00, offsetof(struct Custom, color[0]), 0x060,
			0xc801, 0xff00, offsetof(struct Custom, color[0]), 0x6de,
			0xc901, 0xff00, offsetof(struct Custom, color[0]), 0x7de,
			0xca01, 0xff00, offsetof(struct Custom, color[0]), 0x8dd,
			0xca01, 0xff00, offsetof(struct Custom, color[0]), 0x7df,
			0xcb01, 0xff00, offsetof(struct Custom, color[0]), 0x9dd,
			0xcc01, 0xff00, offsetof(struct Custom, color[0]), 0xadc,
			0xcd01, 0xff00, offsetof(struct Custom, color[0]), 0xaec,
			0xce01, 0xff00, offsetof(struct Custom, color[0]), 0xbec,
			0xcf01, 0xff00, offsetof(struct Custom, color[0]), 0xceb,
			0xd001, 0xff00, offsetof(struct Custom, color[0]), 0xdeb,
			0xd101, 0xff00, offsetof(struct Custom, color[0]), 0xeea,
			0xd201, 0xff00, offsetof(struct Custom, color[0]), 0xffa,
			0xe101, 0xff00, offsetof(struct Custom, color[0]), 0x080,
			0xfa01, 0xff00, offsetof(struct Custom, color[0]), 0x090, 
			0xe901, 0xff00, offsetof(struct Custom, color[0]), 0x0a0, 
			0xff01, 0xff00, offsetof(struct Custom, color[0]), 0x00,*/
           // 0xff01, 0xff00, offsetof(struct Custom, color[15]), 0x025d, 

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