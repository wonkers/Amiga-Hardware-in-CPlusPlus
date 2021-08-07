#include "System.h"

//short frameCounter = 0;

enum GameState
{
    STARTING,
    TITLE,
    PLAYING,
    FINISHED,
};
static __attribute__((interrupt)) void interruptHandler() {
    volatile struct Custom *custom = (struct Custom*)0xdff000;
	custom->intreq=(1<<INTB_VERTB); 
	custom->intreq=(1<<INTB_VERTB); //reset vbl req. twice for a4000 bug.
	// DEMO - increment frameCounter
	//frameCounter++;
}

class Game
{

private:
    GameState gameState;
    ThisSystem system;
    volatile struct Custom *custom = (struct Custom*)0xdff000;

    short MouseLeft(){return !((*(volatile UBYTE*)0xbfe001)&64);}	
    short MouseRight(){return !((*(volatile UWORD*)0xdff016)&(1<<10));}
    
public:
    Game();
    ~Game();

    void  initialize();
    void  LoadContent();
    void  Update();
    void  UpdateDraw();

    void Run();
};
