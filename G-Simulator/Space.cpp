#include "Space.h"
#include <iostream>



Space::Space(float G)
{
    this->G = G;
}


void Space::AddObject(Object obj)
{
    this->Objects.push_back(obj);
}

void Space::ClearObjects()
{
    this->Objects.clear();
}


void Space::DeleteObject(int index)
{
    this->Objects.erase(begin(Objects) + index);
}

float Space::GetDistance(float x1, float y1, float x2, float y2)
{
    return(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}

void AdditionalForceCalc(int beg_index, int end_index, Space &sp)
{
    float Force = 0;
    float angle;
    float new_accel_x;
    float new_accel_y;

    for (int i = beg_index; i < end_index; i++)
    {
        new_accel_x = 0;
        new_accel_y = 0;

        for (int j = 0; j < sp.Objects.size(); j++)
        {
            if (j != i)
            {
                Force = (sp.G * sp.Objects[j].GetMass() * sp.Objects[i].GetMass()) / pow(sp.GetDistance(sp.Objects[i].GetXPos(), -sp.Objects[i].GetYPos(), sp.Objects[j].GetXPos(), -sp.Objects[j].GetYPos()), 2);

                angle = atan2(-sp.Objects[j].GetYPos() + sp.Objects[i].GetYPos(), sp.Objects[j].GetXPos() - sp.Objects[i].GetXPos());

                new_accel_x += Force * cos(angle) / sp.Objects[i].GetMass();
                new_accel_y += Force * sin(angle) / sp.Objects[i].GetMass();

            }
        }

        sp.Objects[i].SetAcceleration(new_accel_x, new_accel_y);

        sp.Objects[i].SetSpeed(sp.Objects[i].GetXSpeed() + sp.Objects[i].GetXAcceleration(), sp.Objects[i].GetYSpeed() + sp.Objects[i].GetYAcceleration());
    }

}

void Space::ForceCalc()
{
    int beg_index = floor(Objects.size() / 4) + Objects.size() % 4;
    int step = (Objects.size() - beg_index) / 3;


    thread dob_thread1([beg_index, this, step]()
    {
        AdditionalForceCalc(beg_index, beg_index + step, ref(*this));
    });

    thread dob_thread2([beg_index, this, step]()
    {
        AdditionalForceCalc(beg_index + step, beg_index + step * 2, ref(*this));
    });

    thread dob_thread3([beg_index, this, step]()
    {
        AdditionalForceCalc(beg_index + step * 2, beg_index + step * 3, ref(*this));
    });

    float Force = 0;
    float angle;
    float new_accel_x;
    float new_accel_y;

    for (int i = 0; i < beg_index; i++)
    {
        new_accel_x = 0;
        new_accel_y = 0;

        for (int j = 0; j < Objects.size(); j++)
        {
            if (j != i)
            {
                Force = (G * Objects[j].GetMass() * Objects[i].GetMass()) / pow(GetDistance(Objects[i].GetXPos(), -Objects[i].GetYPos(), Objects[j].GetXPos(), -Objects[j].GetYPos()), 2);

                angle = atan2(-Objects[j].GetYPos() + Objects[i].GetYPos(), Objects[j].GetXPos() - Objects[i].GetXPos());

                new_accel_x += Force * cos(angle) / Objects[i].GetMass();
                new_accel_y += Force * sin(angle) / Objects[i].GetMass();

            }
        }

        Objects[i].SetAcceleration(new_accel_x, new_accel_y);

        Objects[i].SetSpeed(Objects[i].GetXSpeed() + Objects[i].GetXAcceleration(), Objects[i].GetYSpeed() + Objects[i].GetYAcceleration());
    }

    dob_thread1.join();
    dob_thread2.join();
    dob_thread3.join();




        for (int i = 0; i < Objects.size(); i++)
        {
            for (int j = 0; j < Objects.size(); j++)
            {
                if (j != i)
                {
                    if (i == Objects.size())
                        break;

                    if (GetDistance(Objects[i].GetXPos(), -Objects[i].GetYPos(), Objects[j].GetXPos(), -Objects[j].GetYPos()) <= (Objects[i].GetRadius() + Objects[j].GetRadius()))
                    {
                        float speed_x, speed_y;

                        if (Objects[i].GetRadius() >= Objects[j].GetRadius())
                        {
                            speed_x = (Objects[i].GetMass() * Objects[i].GetXSpeed() + Objects[j].GetMass() * Objects[j].GetXSpeed()) / (Objects[j].GetMass() + Objects[i].GetMass());
                            speed_y = (Objects[i].GetMass() * Objects[i].GetYSpeed() + Objects[j].GetMass() * Objects[j].GetYSpeed()) / (Objects[j].GetMass() + Objects[i].GetMass());

                            Objects[i].ChangeMass(Objects[j].GetMass());
                            Objects[i].SetRadius((sqrt(pow(Objects[i].GetRadius(), 2) + pow(Objects[j].GetRadius(), 2))));
                            Objects[i].SetSpeed(speed_x, speed_y);
                            Objects.erase(begin(Objects) + j);
                        }

                        else
                        {
                            speed_x = (Objects[i].GetMass() * Objects[i].GetXSpeed() + Objects[j].GetMass() * Objects[j].GetXSpeed()) / (Objects[j].GetMass() + Objects[i].GetMass());
                            speed_y = (Objects[i].GetMass() * Objects[i].GetYSpeed() + Objects[j].GetMass() * Objects[j].GetYSpeed()) / (Objects[j].GetMass() + Objects[i].GetMass());

                            Objects[j].ChangeMass(Objects[i].GetMass());
                            Objects[i].SetRadius((sqrt(pow(Objects[i].GetRadius(), 2) + pow(Objects[j].GetRadius(), 2))));
                            Objects[j].SetSpeed(speed_x, speed_y);
                            Objects.erase(begin(Objects) + i);
                        }

                    }
                }
            }
        }

}



Space::~Space()
{
}

