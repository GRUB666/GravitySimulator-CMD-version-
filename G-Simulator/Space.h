#pragma once
#include "Object.h"
#include <vector>
#include <iterator>
#include <cmath>
#include <thread>

class Space
{

public:

    vector<Object> Objects;

    float G;

public:
    Space(float G = 1);

    void AddObject(Object obj);

    void ClearObjects();

    void DeleteObject(int index);

    Object& operator [] (int index)
    {
        return Objects[index];
    }

    static float GetDistance(float x1, float y1, float x2, float y2);



    void ForceCalc();


    ~Space();
};
