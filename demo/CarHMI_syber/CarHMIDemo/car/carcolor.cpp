#include "carcolor.h"

CarColor::CarColor()
{
}

void CarColor::Set(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha)
{
    mRGBA[0] = r;
    mRGBA[1] = g;
    mRGBA[2] = b;
    mRGBA[3] = alpha;
}

void CarColor::Set(QColor color)
{
    mRGBA[0] = color.redF();
    mRGBA[1] = color.greenF();
    mRGBA[2] = color.blueF();
    mRGBA[3] = color.alphaF();
}
