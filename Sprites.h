#pragma once
#include <proto/exec.h>

class MovableSprite
{
    private:
    UWORD size;

    void LoadHWSprite(UWORD *spriteData, struct BitMap *imageData, int plane, int offset)
    {
        int data = 2;
        for(int line = 0; line < imageData->Rows; line++)
        {
            spriteData[data] = imageData->Planes[plane][line * imageData->BytesPerRow + offset];
            spriteData[data] <<=8;
            spriteData[data] |= imageData->Planes[plane][line * imageData->BytesPerRow + offset + 1];

            spriteData[data + 1] = imageData->Planes[plane+1][line * imageData->BytesPerRow + offset];
            spriteData[data + 1] <<= 8;
            spriteData[data + 1] |= imageData->Planes[plane+1][line * imageData->BytesPerRow + offset + 1];

            data+=2;
        }
    }

    public:
    UWORD *sprPos;
    UWORD *sprCtl;
    UWORD *sprData;
    UWORD height;

    void CreateSprite(struct BitMap *bitmapData, int plane, int offset)
    {
        size = (4 + (bitmapData->Rows * 2)) * sizeof(UWORD);
        sprData = (UWORD*)AllocMem(size, MEMF_CLEAR|MEMF_CHIP);
        LoadHWSprite(sprData, bitmapData, plane, offset);
        
        //have sprPos/Ctl point to the addresses of spritedata->sprpos/ctl
        sprPos = &sprData[0];
        sprCtl = &sprData[1];
        height = bitmapData->Rows;
    }
    void DestroySprite()
    {
        FreeMem(sprData, size);
    }
    void MoveHorizontal(int value)
    {
        value *= 0x0001;
        *sprPos += value;
    }

    void MoveVertical(int value)
    {
        value *= 0x0200;
        *sprPos += value;
        *sprCtl += value;
    }

    
};

class MovableAttachedSprite 
{
    protected:
       
    public:
    MovableSprite *sprites;
    int Count;
    void Create(struct BitMap *bitmapData)
    {
        Count = 2;
        sprites = (MovableSprite *)AllocMem(sizeof(MovableSprite)*Count, MEMF_CLEAR);
        for(int i = 0; i < Count; i++)
            sprites[i].CreateSprite(bitmapData, i*2, 0);
    }
    void Destroy()
    {
        for(int i = 0; i < Count; i++)
            sprites[i].DestroySprite();
        FreeMem(sprites, sizeof(MovableSprite)*Count);
    }
    
    void MoveHorizontal(int value)
    {
        value *= 0x0001;

        if((*sprites[0].sprPos & 0x00ff) > 0
        || (*sprites[0].sprPos & 0x00ff) < 160)

        {
            for(int i = 0; i < Count; i++)
            {
                *sprites[i].sprPos += value;
            }
        }
    }
    void MoveVertical(int value)
    {
        value *= 0x0200;
        for(int i = 0; i < Count; i++)
        {
            *sprites[i].sprPos += value;
            *sprites[i].sprCtl += value;
        }
    }

    void SetPosition(UWORD x, UWORD y)
    {
        x += (Hardware::Copper::copper1[COP_DIWSTRT] & 0x00ff);//2;
        y += (Hardware::Copper::copper1[COP_DIWSTRT] & 0xff00) >> 8;

        *sprites[0].sprPos = y << 8;
        *sprites[0].sprPos |= x >> 1;
        *sprites[0].sprCtl = (y + sprites[0].height -1) << 8;
        *sprites[0].sprCtl |= (x & 0x0001);
        *sprites[0].sprCtl |= (y & 0x0100) >> 7;
        *sprites[0].sprCtl |= (y & 0x0100) >> 6;

        *sprites[1].sprPos = y << 8;
        *sprites[1].sprPos |= x >> 1;
        *sprites[1].sprCtl = (y + sprites[0].height -1) << 8;
        *sprites[1].sprCtl |= ((x & 0x0001) | SPRITE_ATTACHED);
        *sprites[1].sprCtl |= (y & 0x0100) >> 7;
        *sprites[1].sprCtl |= (y & 0x0100) >> 6;

        *sprites[2].sprPos = y << 8;
        *sprites[2].sprPos |= (x + 16) >> 1;
        *sprites[2].sprCtl = (y + sprites[0].height -1) << 8;
        *sprites[2].sprCtl |= (x & 0x0001);
        *sprites[2].sprCtl |= (y & 0x0100) >> 7;
        *sprites[2].sprCtl |= (y & 0x0100) >> 6;

        *sprites[3].sprPos = y << 8;
        *sprites[3].sprPos |= (x + 16) >> 1;
        *sprites[3].sprCtl = (y + sprites[0].height -1) << 8;
        *sprites[3].sprCtl |= ((x & 0x0001) | SPRITE_ATTACHED);
        *sprites[3].sprCtl |= (y & 0x0100) >> 7;
        *sprites[3].sprCtl |= (y & 0x0100) >> 6;
        
        /*for(int i = 0; i < Count; i++)
        {
            *sprites[i].sprPos = y << 8;
            *sprites[i].sprPos |= (x + ((i>>1)<<4))>>1;
            *sprites[i].sprCtl = ((y + sprites[i].height-1) << 8);

            *sprites[i].sprCtl |= (x & 0x0001);
            *sprites[i].sprCtl |= (y & 0x0100) >> 7;
            *sprites[i].sprCtl |= (y & 0x0100) >> 6;
        }
        *sprites[1].sprCtl |= SPRITE_ATTACHED;
        
        if(Count == 4)
            *sprites[3].sprCtl |= SPRITE_ATTACHED;*/
    }
};

class BigMovableAttachedSprite : public MovableAttachedSprite
{
    private:
    public:
    void Create(struct BitMap *bitmapData)
    {
        Count = 4;
        sprites = (MovableSprite *)AllocMem(sizeof(MovableSprite)*Count, MEMF_CLEAR);
        for(int i = 0; i < 2; i++)
            sprites[i].CreateSprite(bitmapData, i*2, 0);
        for(int i = 0; i < 2; i++)
            sprites[i+2].CreateSprite(bitmapData, i*2, 2);
    }

};

class BigMovableSprite : public MovableSprite
{
    public:
    MovableSprite *sprites;
    int Count;

    void Create(struct BitMap *bitmapData)
    {
        Count = 2;
        sprites = (MovableSprite *)AllocMem(sizeof(MovableSprite)*Count, MEMF_CLEAR);
        sprites[0].CreateSprite(bitmapData, 0, 0);
        sprites[1].CreateSprite(bitmapData, 0, 2);
    }
    void MoveHorizontal(int value)
    {
        value *= 0x0001;

        if((*sprites[0].sprPos & 0x00ff) > 0
        || (*sprites[0].sprPos & 0x00ff) < 160)

        {
            for(int i = 0; i < Count; i++)
            {
                *sprites[i].sprPos += value;
            }
        }
    }
    void MoveVertical(int value)
    {
        value *= 0x0200;
        for(int i = 0; i < Count; i++)
        {
            *sprites[i].sprPos += value;
            *sprites[i].sprCtl += value;
        }
    }

};