#include "caranimator.h"
#include "carorientation.h"

CarAnimator::CarAnimator(QObject *parent)
    : QObject(parent),
      mPosition(QVector<GLfloat>(3)),
      mOrientation(QVector<GLfloat>(3))
{
    mOrientation[0] = 0.0;
    mOrientation[1] = 0.0;
    mOrientation[2] = 0.0;

    mPosition[0] = 0.0;
    mPosition[1] = 0.0;
    mPosition[2] = 0.0;

    mPositionTo = mPosition;
    mPositionFrom = mPosition;
    mOrientationTo = mOrientation;
    mOrientationFrom = mOrientation;
    setAnimationSteps(1);
}

bool CarAnimator::updateAnimation()
{
    mCurrentStep++;
    if (mCurrentStep <= mSteps) {
        // Interpolate orientation and position of model
        mPosition[0] = mPositionFrom[0] + (mPositionTo[0] - mPositionFrom[0]) * mCurrentStep / mSteps;
        mPosition[1] = mPositionFrom[1] + (mPositionTo[1] - mPositionFrom[1]) * mCurrentStep / mSteps;
        mPosition[2] = mPositionFrom[2] + (mPositionTo[2] - mPositionFrom[2]) * mCurrentStep / mSteps;

        mOrientation[0] = mOrientationFrom[0] + (mOrientationTo[0] - mOrientationFrom[0]) * mCurrentStep / mSteps;
        mOrientation[1] = mOrientationFrom[1] + (mOrientationTo[1] - mOrientationFrom[1]) * mCurrentStep / mSteps;
        mOrientation[2] = mOrientationFrom[2] + (mOrientationTo[2] - mOrientationFrom[2]) * mCurrentStep / mSteps;
        return true;
    } else {
        mPosition = mPositionTo;
        mOrientation = mOrientationTo;
    }
    return false;
}

void CarAnimator::setOrientationTo(CarOrientation& orientationTo)
{
    mOrientationTo = orientationTo.mOrientation;
    mPositionTo = orientationTo.mPosition;

    // select shortest route for rotation and scale to 0..360 range
    for (int i=0; i<3; ++i) {
        if (mOrientationTo[i] > 360) {
            mOrientationTo[i] -=360;
        } else if (mOrientationTo[i] <= 0 ) {
            mOrientationTo[i] += 360;
        }
        if (mOrientationTo[i] - mOrientation[i] > 180) {
            mOrientation[i] += 360;
        } else if (mOrientationTo[i] - mOrientation[i] < -180) {
            mOrientation[i] -= 360;
        }
    }

    mOrientationFrom = mOrientation;
    mPositionFrom = mPosition;
    mCurrentStep = 0;
}

void CarAnimator::setOrientation(CarOrientation& orientation)
{
    mOrientation = orientation.mOrientation;
    mPosition = orientation.mPosition;
    setOrientationTo(orientation);  // This is set so that animator wont try to rotate model
}

void CarAnimator::setAnimationSteps(int steps)
{
    mSteps = steps;
    if (mSteps < 1) {   // use at least 1 step for animation
        mSteps = 1;
    }
    mCurrentStep = 0;
}

QVector<GLfloat> CarAnimator::position()
{
    return mPosition;
}

QVector<GLfloat> CarAnimator::orientation()
{
    return mOrientation;
}
