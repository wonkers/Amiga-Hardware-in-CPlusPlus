#pragma once

class Vector
{
    public:
    int x;  //will change to float
    int y; //will change to float

    Vector(){}

    Vector(int _x, int _y):x(_x), y(_y){}
    void Normalize()
    {
        //to be implemented
    }
    Vector operator =(const Vector &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector operator-=(const Vector& rhs)
    {
        x-=rhs.x;
        y-=rhs.y;
        return *this;
    }
    Vector operator+=(const Vector& rhs)
    {
        x+=rhs.x;
        y+=rhs.y;
        return *this;
    }
    Vector operator*=(const Vector& rhs)
    {
        x*=rhs.x;
        y*=rhs.y;
        return *this;
    }
    Vector operator/=(const Vector& rhs)
    {
        x/=rhs.x;
        y/=rhs.y;
        return *this;
    }
    Vector operator*=(const int value)
    {
        x*=value;
        y*=value;
        return *this;
    }
    Vector operator/=(const int value)
    {
        x/=value;
        y/=value;
        return *this;
    }
    Vector operator/(const int value)
    {
        x/=value;
        y/=value;
        return *this;
    }
    Vector operator+=(const int value)
    {
        x+=value;
        y+=value;
        return *this;
    }
    Vector operator-=(const int value)
    {
        x-=value;
        y-=value;
        return *this;
    }
   // operator override - (const Vector&, Vector rhs)
   // {

   // }
};