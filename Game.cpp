#include "Game.h"


Game::Game()
{
    
}
Game::~Game()
{
    
}

void Game::initialize()
{
    
}
void Game::LoadContent()
{

}
void  Game::Update()
{

}
void  Game::UpdateDraw()
{

}


void Game::Run()
{
    // DEMO
    
	system.SetInterruptHandler((APTR)interruptHandler);
	custom->intena=(1<<INTB_SETCLR)|(1<<INTB_INTEN)|(1<<INTB_VERTB);
	custom->intreq=1<<INTB_VERTB;//reset vbl req

    initialize();
    LoadContent();
    gameState = STARTING;
    while(gameState != FINISHED)
    {
        system.WaitVbl();

        //used for debugging
        //int f = frameCounter & 255;

        Update();
        UpdateDraw();

        if(MouseLeft())
            gameState = FINISHED;
    }

    system.FreeSystem();
}