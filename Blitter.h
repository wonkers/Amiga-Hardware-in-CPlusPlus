#pragma once
#include <proto/exec.h>
#include "Vectors.h"

class Blitter
{
    private:
        volatile struct Custom *custom = (struct Custom*)0xdff000;
    
    public:
    Blitter()
    {

    }
    void Blit()
    {

    }
    void StraightBlit(struct BitMap *source, UWORD sourceX, UWORD sourceY, 
                    struct BitMap *destination, UWORD destX, UWORD destY, UWORD width, UWORD height)
    {

        for(int i = 0; i < source->Depth; i++)
        {
            custom->bltcon0     = (0x7 << 8) | 0xc0;
            custom->bltcon1     = 0x0000;
            
            custom->bltbmod     = source->BytesPerRow - (width/8);
            custom->bltcmod     = destination->BytesPerRow - (width/8);
            custom->bltdmod     = destination->BytesPerRow - (width/8);

            ULONG sourceAddr       = (ULONG)source->Planes[i];
            ULONG destinationAddr  = (ULONG)destination->Planes[i];

            sourceAddr += (sourceY * source->BytesPerRow) + (sourceX/8);
            destinationAddr += (destY * destination->BytesPerRow) + (destX/8) ;

            custom->bltbpt      = (APTR)sourceAddr;
            custom->bltcpt      = (APTR)destinationAddr;
            custom->bltdpt      = (APTR)destinationAddr;

            custom->bltsize     = (height << 6) | width/16;
        }
    }

    void CookieCutterBlit(struct BitMap *mask, struct BitMap *source, UWORD sourceX, UWORD sourceY, 
                    struct BitMap *destination, UWORD destX, UWORD destY, UWORD width, UWORD height)
    {
        for(int i = 0; i < source->Depth; i++)
        {
            custom->bltcon0         = (0xf << 8) | 0xca;
            custom->bltcon1         = 0x0000;
            
            custom->bltamod         = source->BytesPerRow - (width/8);
            custom->bltbmod         = source->BytesPerRow - (width/8);
            custom->bltcmod         = destination->BytesPerRow - (width/8);
            custom->bltdmod         = destination->BytesPerRow - (width/8);

            ULONG maskAddr          = (ULONG)mask->Planes[i];
            ULONG sourceAddr        = (ULONG)source->Planes[i];
            ULONG destinationAddr   = (ULONG)destination->Planes[i];

            maskAddr += (sourceY * source->BytesPerRow) + (sourceX/8);
            sourceAddr += (sourceY * source->BytesPerRow) + (sourceX/8);
            destinationAddr += (destY * destination->BytesPerRow) + (destX/8) ;

            custom->bltapt          = (APTR)maskAddr;
            custom->bltbpt          = (APTR)sourceAddr;
            custom->bltcpt          = (APTR)destinationAddr;
            custom->bltdpt          = (APTR)destinationAddr;

            custom->bltsize         = (height << 6) | width/16;
        }
    }
};

class BlitterObject
{
    public:
    struct BitMap imageData;
    struct BitMap mask;
    struct BitMap backstate;
    int height;
    int width;
    Vector position;
    Vector oldPosition;
    Blitter blitter;

    void Create(struct BitMap *source, int x, int y, int sizeX, int sizeY)
    {
        height = sizeY;
        width = sizeX;
        int size = sizeX*sizeY/8;
        InitBitMap(&imageData, source->Depth, sizeX, sizeY);
        InitBitMap(&mask, source->Depth, sizeX, sizeY);
        InitBitMap(&backstate, source->Depth, sizeX, sizeY);

        for(int i = 0; i < source->Depth; i++)
        {
            imageData.Planes[i] = (PLANEPTR)AllocMem(size, MEMF_CHIP|MEMF_CLEAR);
            mask.Planes[i] = (PLANEPTR)AllocMem(size, MEMF_CHIP|MEMF_CLEAR);
            backstate.Planes[i] = (PLANEPTR)AllocMem(size, MEMF_CHIP|MEMF_CLEAR);
        }

        DisownBlitter();
        BltBitMap(source, x, y, &imageData, 0, 0, sizeX, sizeY, 0xC0, 0xff, NULL);
        BltBitMap(source, x, y, &mask, 0, 0, sizeX, sizeY, 0xC0, 0xff, NULL);
        OwnBlitter();

        //blitter.StraightBlit(source, x, y, &imageData, 0, 0, sizeX, sizeY);
        //blitter.StraightBlit(&imageData, 0, 0, &mask, 0, 0, sizeX, sizeY);

        for(int i = 0; i < size; i++)
        {
            mask.Planes[0][i] = mask.Planes[0][i] | mask.Planes[1][i] | mask.Planes[2][i] | mask.Planes[3][i] | mask.Planes[4][i];
            mask.Planes[1][i] = mask.Planes[0][i] | mask.Planes[1][i] | mask.Planes[2][i] | mask.Planes[3][i] | mask.Planes[4][i];
            mask.Planes[2][i] = mask.Planes[0][i] | mask.Planes[1][i] | mask.Planes[2][i] | mask.Planes[3][i] | mask.Planes[4][i];
            mask.Planes[3][i] = mask.Planes[0][i] | mask.Planes[1][i] | mask.Planes[2][i] | mask.Planes[3][i] | mask.Planes[4][i];
            mask.Planes[4][i] = mask.Planes[0][i] | mask.Planes[1][i] | mask.Planes[2][i] | mask.Planes[3][i] | mask.Planes[4][i];
        }
    }

    void DrawBob(struct BitMap *Destination, int x, int y)
    {
        oldPosition = position;
        position.x = x;
        position.y = y;
        
        //paste in background of previous location.
       // blitter.StraightBlit(&backstate, 0, 0, Destination, oldPosition.x, oldPosition.y, backstate.BytesPerRow*8, backstate.Rows);
        
        //save background of new location
       // blitter.StraightBlit(Destination, x, y, &backstate, 0, 0, backstate.BytesPerRow*8, backstate.Rows);

        //draw through the mask
        blitter.CookieCutterBlit(&mask, &imageData, 0, 0, Destination, x, y, imageData.BytesPerRow*8, imageData.Rows);
    }
};