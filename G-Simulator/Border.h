#pragma once

#ifndef BORDER
#define BORDER

#endif // BORDER

struct Border
{
    float X, Y;      //Координаты начала границ
    float XS, YS;    //Размеры границ

    Border(float X = 0, float Y = 0, float XS = 0, float YS = 0)
    {
        SetBorder(X, Y, XS, YS);
    }


    void SetBorder(float X, float Y, float XS, float YS)
    {
        this->X = X;
        this->Y = Y;

        this->XS = XS;
        this->YS = YS;
    }

    float GetXRight()
    {
        return X + XS;
    }

    float GetYDown()
    {
        return Y - YS;
    }

};
