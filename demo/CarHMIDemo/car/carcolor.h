/*
* Helper to convert from QColor
*/
#ifndef COLOR_H
#define COLOR_H

#include <QtOpenGL>

class CarColor
{
public:
    CarColor();
    void Set(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha = 1.0);
    void Set(QColor color);

    GLfloat mRGBA[4]; // Red = 0, Green = 1, Blue = 2, Alpha = 3;
};

#endif // COLOR_H
