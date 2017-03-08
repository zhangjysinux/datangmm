/*
* Class to control animation of the car
*/

#ifndef CARANIMATOR_H
#define CARANIMATOR_H

#include <QObject>
#include <QtOpenGL>
class CarOrientation;

class CarAnimator : public QObject
{
    Q_OBJECT

public:
    CarAnimator(QObject *parent=0);
    bool updateAnimation();

    void setOrientationTo(CarOrientation& orientation);
    void setOrientation(CarOrientation& orientation);

    void setAnimationSteps(int steps = 1);

    QVector<GLfloat> position();
    QVector<GLfloat> orientation();

private:

    QVector<GLfloat> mPosition;     // Current position of model.
    QVector<GLfloat> mOrientation;  // Current orientation of model.

    QVector<GLfloat> mPositionFrom;     // Animator tries to move model from this position.
    QVector<GLfloat> mOrientationFrom;  // Animator tries to rotate model from this orientation.

    QVector<GLfloat> mPositionTo;       // Animator tries to move model to this position
    QVector<GLfloat> mOrientationTo;    // Animator tries to rotate model to this orientation

    int mSteps;         // use this many frames for animation
    int mCurrentStep;   // current step of animation
};

#endif // CARANIMATOR_H
