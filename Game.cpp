// //#include "Game.h"

// Game::Game()
// {
// }
// Game::~Game()
// {
    
// }

// void Game::initialize()
// {
    
// }
// void Game::LoadContent()
// {
    
//     //BPTR filePtr = OpenFile("dh0:file.txt");
//     //CloseFile(filePtr);
//    // filePtr = Open("dh0:file.txt", MODE_OLDFILE);
//    // Close(filePtr);

//     //SpriteImage sprImg;
//     //sprImg.imageData = sprImg.LoadBitmap(imgs[0]);
//     //BitMap bitmap;
//     //InitBitMap(&bitmap, 4, 640, 256);
//     //for(short plane = 0; plane < bitmap.Depth; plane++)
//     //{
//     //    bitmap.Planes[plane] = (PLANEPTR)AllocRaster(bitmap.BytesPerRow * 8, bitmap.Rows);
//         //BltClear(bitmap.Planes[plane], RASSIZE(bitmap.BytesPerRow * 8, bitmap.Rows),0);
//     //}
//     //BitplaneImage bpl;
//     //bpl.LoadBitmap(imgs[0], &bitmap);

//     //hardware.planes[1][10000] = 0xff;
// }

// void Game::UnloadContent()
// {

// }

// void  Game::Update()
// {
//     if(Joystick.GetState(Joystick1).Up == Pressed)
//     {
//          KPrintF("Pressed Up\n");
//     }
//     if(Joystick.GetState(Joystick1).Down == Pressed)
//     {
//         KPrintF("Pressed Down\n");
//     }
//     if(Joystick.GetState(Joystick1).Left == Pressed)
//     {
//         KPrintF("Pressed left\n");
//     }
//     if(Joystick.GetState(Joystick1).Right == Pressed)
//     {
//         KPrintF("Pressed Right\n");
//     }
//     if(Joystick.GetState(Joystick1).Fire == Pressed)
//     {
//         KPrintF("Pressed Fire\n");
//     }
// }
// void  Game::UpdateCopper()
// {

// }


// void Game::Run()
// {
// 	hardware.SetInterruptHandler((APTR)interruptHandler);
// 	custom->intena=(1<<INTB_SETCLR)|(1<<INTB_INTEN)|(1<<INTB_VERTB);
// 	custom->intreq=1<<INTB_VERTB;//reset vbl req

//     initialize();
//     LoadContent();
//     gameState = STARTING;
//     while(gameState != FINISHED)
//     {
//         hardware.WaitVbl();

//         //used for debugging
//         //int f = frameCounter & 255;

//         Update();
//         UpdateCopper();

//         if(MouseLeft())
//             gameState = FINISHED;
//     }

//     hardware.FreeSystem();
// }

