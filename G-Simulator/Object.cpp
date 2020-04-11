#include "Object.h"



Object::Object(float x, float y, float radius, float mass, float x_speed, float y_speed)
{
    this->Obj.setPosition(x, -y);

    this->Obj.setRadius(radius);
    this->Obj.setOrigin(radius, radius);
    this->mass = mass;
    this->x_speed = x_speed;
    this->y_speed = y_speed;

    this->Obj.setFillColor(Color::Blue);

    this->x_acceleration = 0;
    this->y_acceleration = 0;
    this->Name = "No Name";

}

void Object::setObject(CircleShape new_object)
{
    this->Obj = new_object;
}

void Object::SetPosition(float xval, float yval)
{
    Obj.setPosition(xval, -yval);
}

CircleShape Object::getObject()
{
    return(this->Obj);
}

void Object::ChangeSpeed(float xch, float ych)
{
    this->x_speed += xch;
    this->y_speed += ych;
}

void Object::SetSpeed(float xval, float yval)
{
    this->x_speed = xval;
    this->y_speed = yval;
}

void Object::SetAcceleration(float xval, float yval)
{
    this->x_acceleration = xval;
    this->y_acceleration = yval;
}


float Object::GetXPos()
{
    return(Obj.getPosition().x);
}


float Object::GetYPos()
{
    return(-Obj.getPosition().y);
}

float Object::GetXSpeed()
{
    return this->x_speed;
}

void Object::SetRadius(float val)
{
    this->Obj.setRadius(val);
    this->Obj.setOrigin(Obj.getRadius(), Obj.getRadius());
}

float Object::GetRadius()
{
    return this->Obj.getRadius();
}

float Object::GetYSpeed()
{
    return this->y_speed;
}

float Object::GetXAcceleration()
{
    return this->x_acceleration;
}

float Object::GetYAcceleration()
{
    return this->y_acceleration;
}

void Object::ChangeMass(float ch)
{
    this->mass += ch;
}

void Object::SetMass(float val)
{
    this->mass = val;
}


float Object::GetMass()
{
    return this->mass;
}


void Object::SetName(string new_name)
{
    this->Name = new_name;
}


string Object::GetName()
{
    return this->Name;
}


Object::~Object()
{
}

