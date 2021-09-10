#pragma once

namespace Hardware{
    namespace GamePort
    {
        enum ButtonState
        {
            Released,
            Pressed
        };
        enum KeyState
        {
            KeyUp,
            KeyDown
        };
        enum Joystick
        {
            Joystick0,
            Joystick1
        };

        struct Buttons
        {
            ButtonState Up;
            ButtonState Down;
            ButtonState Left;
            ButtonState Right;
            ButtonState Fire;
        };
        struct Keys
        {
            KeyState Return;
            KeyState Space;
            KeyState Escape;
            KeyState Tab;
            KeyState RightCtrl;
            KeyState LeftCtrl;
            KeyState RightShift;
            KeyState LeftShft;
            KeyState LeftAlt;
            KeyState RightAlt;
        };

        class Joy
        {
            volatile struct Custom *custom = (struct Custom*)0xdff000;
            short Fire(){return !((*(volatile UBYTE*)0xbfe001)&128);}	
        public:
            Joy(){}

            ~Joy(){}
            Buttons GetState(Joystick portnumber) 
            {
                Buttons buttons;
                UWORD up = 0x0000;
                UWORD down = 0x0000;
                UWORD left = 0x0000;
                UWORD right = 0x0000;
                UWORD tst;
                switch(portnumber)
                {
                    case Joystick0:
                    tst =*(volatile UWORD*)&custom->joy0dat;
                    break;
                    case Joystick1:
                    tst =*(volatile UWORD*)&custom->joy1dat;
                }
                up = ((tst & 0x0200) >> 8) ^ ((tst & 0x0100) >> 7);
                down = ((tst & 0x0001)) ^ ((tst & 0x0002) >> 1);
                left = (tst & 0x0200) >> 8;
                right = (tst & 0x0020) >> 1;

                if(up)
                    buttons.Up = Pressed;
                else
                    buttons.Up = Released;
                if(down)
                    buttons.Down = Pressed;
                else
                    buttons.Down = Released;
                if(left)
                    buttons.Left = Pressed;
                else
                    buttons.Left = Released;
                if(right)
                    buttons.Right = Pressed;
                else
                    buttons.Right = Released;

                if(Fire())
                    buttons.Fire = Pressed;
                else
                    buttons.Fire = Released;

                return buttons;
            }


        };

        class Keyboard
        {
        public:
            Keyboard(){}
            ~Keyboard(){}
            KeyState GetState()
            {
                KeyState state;
                return state;
            }

        };
    }
}