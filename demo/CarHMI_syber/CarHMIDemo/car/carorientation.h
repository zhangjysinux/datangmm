#ifndef CARORIENTATION_H
#define CARORIENTATION_H

#include <QObject>
#include <QtOpenGL>

class CarOrientation : public QObject
{
    Q_OBJECT
public:
    explicit CarOrientation(QObject *parent = 0);
    void setOrientation(GLfloat i, GLfloat j, GLfloat k);
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setOrientation(CarOrientation& orientation);

signals:

public slots:

public:
    QVector<GLfloat> mOrientation;
    QVector<GLfloat> mPosition;
};

#endif // CARORIENTATION_H
