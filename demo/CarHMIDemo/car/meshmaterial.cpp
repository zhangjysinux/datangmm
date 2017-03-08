#include "meshmaterial.h"

MeshMaterial::MeshMaterial()
{
}

void MeshMaterial::SetMaterialColor(QColor color)
{
    mMaterialColor.Set(color);
}

void MeshMaterial::SetSpecularColor(QColor color)
{
    mSpecularColor.Set(color);
}

void MeshMaterial::SetEmissionColor(QColor color)
{
    mEmissionColor.Set(color);
}

void MeshMaterial::SetShininess(GLint s)
{
    if (s>128)
    {
        s=128;
    }
    else if (s<0)
    {
        s=0;
    }
    mShininess = s;
}
/*
void MeshMaterial::render()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mMaterialColor.mRGBA);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mSpecularColor.mRGBA);
    glMaterialfv(GL_FRONT, GL_EMISSION, mEmissionColor.mRGBA);
    glMateriali(GL_FRONT, GL_SHININESS, mShininess);
}
*/
