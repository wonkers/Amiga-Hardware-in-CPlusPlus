#pragma once

#include "System.h"
#include "Input.h"
#include "Graphics.h"
#include "Copper.h"
#include "Vectors.h"
#include "Sprites.h"
#include "Entities.h"
#include "Blitter.h"

using namespace Hardware::GamePort;
using namespace Graphics;

enum GameState
{
    STARTING,
    TITLE,
    PLAYING,
    FINISHED,
};

class Game
{

private:
    GameState gameState;
    Joy Joystick;
    
    volatile struct Custom *custom = (struct Custom*)0xdff000;

    short MouseLeft(){return !((*(volatile UBYTE*)0xbfe001)&64);}	
    short MouseRight(){return !((*(volatile UWORD*)0xdff016)&(1<<10));}
    
public:

    Game()
    {

    }
    ~Game(){}
    void initialize()
    {
        Hardware::Initialize();
        Hardware::SetInterruptHandler((APTR)Hardware::interruptHandler);

        //interrupt only on vertical blank
        custom->intena  = (1 << INTB_SETCLR) | (1 << INTB_INTEN) | (1 << INTB_VERTB);
        custom->intreq  = 1 << INTB_VERTB;
    }
    void LoadContent()
    {
       
    }
    void UnloadContent()
    {
        
    }

    void Update()
    {
        if(Joystick.GetState(Joystick1).Up == Pressed)
        {
           
        }
        if(Joystick.GetState(Joystick1).Down == Pressed)
        {
           
        }
        if(Joystick.GetState(Joystick1).Left == Pressed)
        {
           
        }
        if(Joystick.GetState(Joystick1).Right == Pressed)
        {
           
        }
        if(Joystick.GetState(Joystick1).Fire == Pressed)
        {
            
        }
    }


    void Run()
    {
        initialize();
        LoadContent();
        gameState = STARTING;

        //Game loops starts here
        while(gameState != FINISHED)
        {
            //wait vertical blank
            Hardware::WaitVbl();
            
            //time to do shit here before the next frame
            Update();

            if(MouseLeft())
                gameState = FINISHED;
        }

        Hardware::FreeSystem();
    }
};
