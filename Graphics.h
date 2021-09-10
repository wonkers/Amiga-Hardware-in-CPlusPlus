#pragma once
//#include "support/gcc8_c_support.h"
#include <proto/exec.h>
#include <proto/dos.h>
#include "iff.h"

extern struct DosLibrary *DOSBase;

namespace Graphics
{
    typedef struct{
        unsigned pad1 :4, red :4, green:4, blue:4;
    }Color4;
    
    class BitplaneImage
    {
    private:
        UWORD depth;
        UWORD width;
        UWORD height;
        UWORD *imageData;

    public:    
        VOID LoadBitMap(UBYTE *file, struct BitMap *mybm, UWORD colors[32])
        {
            int i, x, y, counter, plane;
            unsigned char code, temp;
            BPTR filePtr;
            CHUNK chunk;
            BODY Body;
            CMAP CMap;
            char id[2];
            
            unsigned char * buffer;

            filePtr = Open((CONST_STRPTR)file, (LONG)MODE_OLDFILE);
            if(filePtr == 0)
            {
                Write(Output(), (APTR)"Could not open file\n", strlen("Could not open file\n"));
                Close(filePtr);
                Exit(1);
            }
   
            Seek(filePtr, 0, OFFSET_BEGINNING);
            Read(filePtr, (VOID *)&chunk, sizeof(CHUNK));

            while(id[0] != 'C' && id[1] != 'M')
            {
                Read(filePtr, (VOID *)&id, 2);
            }
            CMap.ChunkId[0] = 'C';
            CMap.ChunkId[1] = 'M';
            Read(filePtr, (VOID *)&CMap.ChunkId[2],2);
            Read(filePtr, (VOID *)&CMap.Size,4);
           
            for(int c = 0; c < 32; c++)
            {
                UBYTE red;
                UBYTE green;
                UBYTE blue;
                Read(filePtr, (VOID *)&red, 1);
                Read(filePtr, (VOID *)&green, 1);
                Read(filePtr, (VOID *)&blue, 1);

                colors[c] |= red/16;
                colors[c] <<= 4;
                colors[c] |= green/16;
                colors[c] <<= 4;
                colors[c] |= blue/16;
       
            }


            while(id[0] != 'B' && id[1] != 'O')
            {
                Read(filePtr, (VOID *)&id, 2);
            }
            Body.ChunkId[0] = 'B';
            Body.ChunkId[1] = 'O';
            Read(filePtr, (VOID *)&Body.ChunkId[2], 2);
            Read(filePtr, (VOID *)&Body.Size, 4);
            
            counter = 0;
                
            /*if bitmap has been initialise already, i dont want to do it again
            I 'll check this by checking the depth.  Anything above 6 should be..not initialised*/
            //we might be using a bitmap that is being used as a backbuffer or...something

            if(mybm->Depth > 6 || mybm->Depth == 0)
            {
                InitBitMap(mybm, chunk.BitMapHeader.BitPlanes, chunk.BitMapHeader.Width, chunk.BitMapHeader.Height);
                
                for(plane = 0; plane < mybm->Depth; plane++)
                {
                    if((mybm->Planes[plane] = (PLANEPTR)AllocRaster(mybm->BytesPerRow * 8, mybm->Rows)) == NULL)
                    {
                        Write(Output(),(APTR)"Cannot allocate memory for bitmap.\n", strlen("Cannot allocate memory for bitmap.\n"));
                        Exit(1000);
                    }
                    BltClear((UBYTE*)mybm->Planes[plane], RASSIZE(mybm->BytesPerRow * 8, mybm->Rows), 0);
                }
            }

            for(y = 0; y < mybm->Rows; y++)
            {
                for(x = 0; x < mybm->Depth; x++)
                {
                    buffer = &mybm->Planes[x][y * mybm->BytesPerRow];
                    counter = 0;
                    while(counter < mybm->BytesPerRow)
                    {
                        Read(filePtr, (unsigned char *)&code, 1);
            
                        if(code < 128)
                        {
                            Read(filePtr, (unsigned char *)&buffer[counter], (code + 1));
                            counter = (counter + code + 1);
                        }
                        else if(code > 128)
                        {
                            Read(filePtr, (unsigned char *)&temp, 1);
                            for(i = counter; i < (counter + 257 - code); i++)
                            {
                                buffer[i] = temp;
                            }
                            counter = counter + 257-code;
                        }
                        else
                        {
                        }	
                    }
                }
            }
            
            Close(filePtr);
            /*filePtr = Open((CONST_STRPTR)"dh0:aus.bpl", MODE_NEWFILE);
            Write(filePtr, &mybm->Planes[0][0], ((320/8*256)));
            Write(filePtr, &mybm->Planes[1][0], ((320/8*256)));
            Write(filePtr, &mybm->Planes[2][0], ((320/8*256)));
            Write(filePtr, &mybm->Planes[3][0], ((320/8*256)));
            Write(filePtr, &mybm->Planes[4][0], ((320/8*256)));
            Close(filePtr);*/

        }
     };

    class SpriteImage
    {
    private:
        UWORD Colors[3];
        UWORD height;
        

    public:
        UWORD *imageData;
            
    };
}