#ifndef CARMATERIAL_H
#define CARMATERIAL_H

#include <QtOpenGL>
#include "carcolor.h"

class MeshMaterial
{
public:
    MeshMaterial();

    void SetMaterialColor(QColor color);
    void SetSpecularColor(QColor color);
    void SetEmissionColor(QColor color);
    void SetShininess(GLint s);

//    void render();

public:
    CarColor mMaterialColor;
    CarColor mSpecularColor;
    CarColor mEmissionColor;
    GLint mShininess;
};

#endif // CARMATERIAL_H
