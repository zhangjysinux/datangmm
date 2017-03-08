#include "carorientation.h"

CarOrientation::CarOrientation(QObject *parent) :
    QObject(parent),
    mOrientation(3),
    mPosition(3)
{
    mOrientation.fill(0.0);
    mPosition.fill(0.0);
    mOrientation[0] = 1.0;
}

void CarOrientation::setOrientation(GLfloat i, GLfloat j, GLfloat k)
{
    mOrientation[0] = i;
    mOrientation[1] = j;
    mOrientation[2] = k;
}

void CarOrientation::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    mPosition[0] = x;
    mPosition[1] = y;
    mPosition[2] = z;
}

void CarOrientation::setOrientation(CarOrientation& orientation)
{
    mOrientation = orientation.mOrientation;
    mPosition = orientation.mPosition;
}
