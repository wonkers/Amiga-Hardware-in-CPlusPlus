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
            
            custom->bltbmod     = source->BytesPerRow - (width>>3);
            custom->bltcmod     = destination->BytesPerRow - (width>>3);
            custom->bltdmod     = destination->BytesPerRow - (width>>3);

            ULONG sourceAddr       = (ULONG)source->Planes[i];
            ULONG destinationAddr  = (ULONG)destination->Planes[i];

            sourceAddr += (sourceY * source->BytesPerRow) + (sourceX>>3);
            destinationAddr += (destY * destination->BytesPerRow) + (destX>>3) ;

            custom->bltbpt      = (APTR)sourceAddr;
            custom->bltcpt      = (APTR)destinationAddr;
            custom->bltdpt      = (APTR)destinationAddr;

            custom->bltsize     = (height << 6) | width>>4;
        }
    }

    void CookieCutterBlit(struct BitMap *mask, struct BitMap *source, UWORD sourceX, UWORD sourceY, 
                    struct BitMap *destination, UWORD destX, UWORD destY, UWORD width, UWORD height)
    {
        for(int i = 0; i < source->Depth; i++)
        {
            custom->bltcon0         = (0xf << 8) | 0xca;
            custom->bltcon1         = 0x0000;
            
            custom->bltamod         = source->BytesPerRow - (width>>3);
            custom->bltbmod         = source->BytesPerRow - (width>>3);
            custom->bltcmod         = destination->BytesPerRow - (width>>3);
            custom->bltdmod         = destination->BytesPerRow - (width>>3);

            ULONG maskAddr          = (ULONG)mask->Planes[i];
            ULONG sourceAddr        = (ULONG)source->Planes[i];
            ULONG destinationAddr   = (ULONG)destination->Planes[i];

            maskAddr += (sourceY * source->BytesPerRow) + (sourceX>>3);
            sourceAddr += (sourceY * source->BytesPerRow) + (sourceX>>3);
            destinationAddr += (destY * destination->BytesPerRow) + (destX>>3) ;

            custom->bltapt          = (APTR)maskAddr;
            custom->bltbpt          = (APTR)sourceAddr;
            custom->bltcpt          = (APTR)destinationAddr;
            custom->bltdpt          = (APTR)destinationAddr;

            custom->bltsize         = (height << 6) | (width>>4);
        }
    }
};

class BlitterObject
{
    

    protected:
    struct BitMap imageData;
    struct BitMap mask;
    
    int height;
    int width;
    int size;
    Vector position;
    Vector oldPosition;
    Blitter *blitter;

   public:
   BlitterObject(){}
   BlitterObject(int sizeX, int sizeY)
   {
        height = sizeY;
        width = sizeX;
        size = width*height>>3;
   }

    void Create(Blitter *device, struct BitMap *source, int x, int y)
    {
        blitter = device;
        InitBitMap(&imageData, source->Depth, width, height);
        InitBitMap(&mask, source->Depth, width, height);
       

        for(int i = 0; i < source->Depth; i++)
        {
            imageData.Planes[i] = (PLANEPTR)AllocMem(size, MEMF_CHIP|MEMF_CLEAR);
            mask.Planes[i] = (PLANEPTR)AllocMem(size, MEMF_CHIP|MEMF_CLEAR);
           
        }

        DisownBlitter();
        BltBitMap(source, x, y, &imageData, 0, 0, width, height, 0xC0, 0xff, NULL);
        BltBitMap(source, x, y, &mask, 0, 0, width, height, 0xC0, 0xff, NULL);
        OwnBlitter();

        //blitter.StraightBlit(source, x, y, &imageData, 0, 0, sizeX, sizeY);
        //blitter.StraightBlit(&imageData, 0, 0, &mask, 0, 0, sizeX, sizeY);
        SetMask();
        
    }

    void SetMask()
    {
        for(int i = 0; i < size; i++)
        {
            for(int plane = 0; plane < mask.Depth; plane++)
            {
                for(int maskplane = 0; maskplane < mask.Depth; maskplane++)
                {
                     mask.Planes[plane][i] |= mask.Planes[maskplane][i];
                }
            }
        }
    }

    void DrawBob(struct BitMap *Destination, int x, int y)
    {
        blitter->CookieCutterBlit(&mask, &imageData, 0, 0, Destination, x, y, imageData.BytesPerRow<<3, imageData.Rows);
    }

    
};

class AnimatedBob : public BlitterObject
{
    protected:
    short frames=1;
    short currentFrame=0;
    short lastFrame = 0;
    bool savedFirstBack = false;
    struct BitMap backstate;
    struct BitMap *imageData;
    struct BitMap *mask;

    public:

    AnimatedBob(int sizeX, int sizeY) : BlitterObject(sizeX, sizeY){}

    ~AnimatedBob()
    {
        //whatif no data set?
        FreeMem(imageData, sizeof(struct BitMap)*frames);
        FreeMem(mask, sizeof(struct BitMap)*frames);
    }

    void Create(Blitter *device, struct BitMap *source, int x, int y, short numberOfFrames)
    {
        blitter = device;
        frames = numberOfFrames;

        imageData = (struct BitMap *)AllocMem(sizeof(struct BitMap)*frames, 0x00);
        mask = (struct BitMap *)AllocMem(sizeof(struct BitMap)*frames, 0x00);

        for(short i = 0; i < frames; i++)
        {
            InitBitMap(&imageData[i], source->Depth, width, height);
            InitBitMap(&mask[i], source->Depth, width, height);
        }

        
        InitBitMap(&backstate, source->Depth, width, height);

        for(int i = 0; i < source->Depth; i++)
        {
            for(short image = 0; image < frames; image++)
            {
                imageData[image].Planes[i] = (PLANEPTR)AllocMem(size, MEMF_CHIP|MEMF_CLEAR);
                mask[image].Planes[i] = (PLANEPTR)AllocMem(size, MEMF_CHIP|MEMF_CLEAR);
            }
            backstate.Planes[i] = (PLANEPTR)AllocMem(size, MEMF_CHIP|MEMF_CLEAR);
        }

        DisownBlitter();
        for(short i = 0; i < frames; i++)
        {
            BltBitMap(source, x+(i*width), y, &imageData[i], 0, 0, width, height, 0xC0, 0xff, NULL);
            BltBitMap(source, x+(i*width), y, &mask[i], 0, 0, width, height, 0xC0, 0xff, NULL);
            SetMask(&mask[i]);
        }
        OwnBlitter();

        //blitter.StraightBlit(source, x, y, &imageData, 0, 0, sizeX, sizeY);
        //blitter.StraightBlit(&imageData, 0, 0, &mask, 0, 0, sizeX, sizeY);
        
        
    }

    void SetMask(struct BitMap * _mask)
    {
        for(int i = 0; i < size; i++)
        {
            for(int plane = 0; plane < _mask->Depth; plane++)
            {
                for(int maskplane = 0; maskplane < _mask->Depth; maskplane++)
                {
                     _mask->Planes[plane][i] |= _mask->Planes[maskplane][i];
                }
            }
        }
    }

    void DrawBob(struct BitMap *Destination, int x, int y, int sysFrame)
    {
        oldPosition = position;
        position.x = x;
        position.y = y;

        if(!savedFirstBack)
        {
            DisownBlitter();
            BltBitMap(Destination, x, y, &backstate, 0, 0, backstate.BytesPerRow<<3, backstate.Rows, 0xc0, 0xff, NULL);
            OwnBlitter();
            savedFirstBack = true;
            blitter->CookieCutterBlit(&mask[0], &imageData[0], 0, 0, Destination, x, y, imageData[0].BytesPerRow<<3, imageData[0].Rows);
        }
        else
        {
            //short interval = (255) / frames;
            
            this->currentFrame = (sysFrame&63)>>4; //(sysFrame)/(interval+1);

            if(currentFrame != lastFrame)
            {
                lastFrame = this->currentFrame;
                DisownBlitter();
                //paste in background of previous location.
                //blitter.StraightBlit(&backstate, 0, 0, Destination, oldPosition.x, oldPosition.y, backstate.BytesPerRow*8, backstate.Rows);
                BltBitMap(&backstate, 0, 0, Destination, oldPosition.x, oldPosition.y, backstate.BytesPerRow<<3, backstate.Rows, 0xc0, 0xff, NULL);
                //save background of new location
                //blitter.StraightBlit(Destination, x, y, &backstate, 0, 0, backstate.BytesPerRow*8, backstate.Rows);
                BltBitMap(Destination, oldPosition.x, oldPosition.y, &backstate, 0,0, backstate.BytesPerRow<<3, backstate.Rows, 0xc0, 0xff, NULL);
                OwnBlitter();
                //draw through the mask
                blitter->CookieCutterBlit(&mask[currentFrame], &imageData[currentFrame], 0, 0, Destination, x, y, imageData[currentFrame].BytesPerRow<<3, imageData[currentFrame].Rows);
            }

            if(currentFrame > frames)
                currentFrame= 0;
        }
    }
};