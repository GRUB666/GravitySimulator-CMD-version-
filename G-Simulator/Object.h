#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "SFML/Audio.hpp"

using namespace std;
using namespace sf;

class Object
{
private:

    float x_speed, y_speed;
    float x_acceleration, y_acceleration;

    float mass;

    CircleShape Obj;

    string Name;

public:

    Object(float x = 0, float y = 0, float radius = 1, float mass = 1, float x_speed = 0, float y_speed = 0);

    void setObject(CircleShape new_object);
    CircleShape getObject();

    void ChangeSpeed(float xch, float ych);
    void SetSpeed(float xch, float ych);

    void SetPosition(float xval, float yval);


    void SetColor(Color col)
    {
        Obj.setFillColor(col);
    }

    int GetColor()
    {
        Obj.getFillColor() == Color::Green;

        if (Obj.getFillColor() == Color::Red)
        {
            return 0;
        }

        if (Obj.getFillColor() == Color::Green)
        {
            return 1;
        }

        if (Obj.getFillColor() == Color::Blue)
        {
            return 2;
        }

        if (Obj.getFillColor() == Color::White)
        {
            return 3;
        }

        if (Obj.getFillColor() == Color::Yellow)
        {
            return 4;
        }

        if (Obj.getFillColor() == Color::Cyan)
        {
            return 5;
        }

        if (Obj.getFillColor() == Color::Magenta)
        {
            return 6;
        }

    }

    void SetColor2(int color)
    {
        switch (color)
        {
        case 0:
            Obj.setFillColor(Color::Red);
            break;

        case 1:
            Obj.setFillColor(Color::Green);
            break;

        case 2:
            Obj.setFillColor(Color::Blue);
            break;

        case 3:
            Obj.setFillColor(Color::White);
            break;

        case 4:
            Obj.setFillColor(Color::Yellow);
            break;

        case 5:
            Obj.setFillColor(Color::Cyan);
            break;

        case 6:
            Obj.setFillColor(Color::Magenta);
            break;
        }
    }

    void SetAcceleration(float xch, float ych);

    float GetXSpeed();
    float GetYSpeed();

    float GetXPos();
    float GetYPos();

    void SetRadius(float val);
    float GetRadius();

    float GetXAcceleration();
    float GetYAcceleration();

    void ChangeMass(float ch);
    void SetMass(float val);
    float GetMass();

    void SetName(string new_name);
    string GetName();



    ~Object();
};
