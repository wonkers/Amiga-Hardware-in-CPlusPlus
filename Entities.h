#pragma once
#include "Vectors.h"
#include "Sprites.h"


enum EntityState
{
    FACINGRIGHT,
    FACINGLEFT,
    CASTING
};


class Entity
{
    public:
    Vector Position;
    Vector Direction;
    Vector BitMapPosition;
    Vector OldPosition;
    Vector OldBitMapPosition;
};

class Player : public Entity
{
    private:
    public:
    BigMovableAttachedSprite sprite[8];
    EntityState previousState;
    EntityState currentState;

    BigMovableAttachedSprite *currentFrame1;
    BigMovableAttachedSprite *currentFrame2;

    void MoveHorizontal(int value)
    {
        for(int i = 0; i < 8; i++)
        {
            sprite[i].MoveHorizontal(value);
        }
    }
    void MoveVertical(int value)
    {
        for(int i = 0; i < 8; i++)
        {
            sprite[i].MoveVertical(value);
        }
    }

    void SetPosition(UWORD x, UWORD y)
    {
        for(int i = 0; i < 8; i++)
        {
            sprite[i].SetPosition(x, y);
        }
    }
};
