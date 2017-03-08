/*
* 3d mesh container
* Has per mesh position, rotation, scale etc information.
*/
#ifndef MESH3D_H
#define MESH3D_H

#include <QtOpenGL>
#include <QGLBuffer>

//#include "3rdparty/lib3ds/types.h"
#include <lib3ds/types.h>
#include "meshmaterial.h"

// Vertex data structure
struct CombinedVertex
{
  float x, y, z;
  float nx, ny, nz;
};


class Mesh3d
{
public:
    Mesh3d(int faces = 0, int points = 0);
    void setName(QString name);
    void setTransformation(QVector<GLfloat> pos, QVector<GLfloat> orientation);
    void setStartIndex(int index);
    void setMaterial(MeshMaterial mMaterial);

    // OpenGL ES2 stuff
    void generateBuffers(Lib3dsVector *vertices, Lib3dsVector *normals);
    void generateBuffers(Lib3dsVector *vertices, Lib3dsVector *normals, Lib3dsWord *indices);
    void drawMeshEs2(GLenum drawmode, QMatrix4x4 perspective);
    void createTransformationMatrixES2(const QVector<GLfloat> pos, const QVector<GLfloat> orientation);
    void generateShader();
    void setColor(QVector3D color);
    void generateCombinedVBO();
    void bindBuffers();
    void releaseBuffers();

private:

    // OpenGL ES2 stuff
    QVector<CombinedVertex> mCombinedVertices;
    QVector<GLushort> mIndexBuffer;
    QGLShaderProgram mShaderProgram;
    QMatrix4x4 mTransformation;
    QMatrix4x4 mFrustum;
    QVector3D mColor;

    int mVertexAttr;    // Attribute array locations
    int mNormalAttr;
    int mMatrixUniform;
    int mColorUniform;

    QGLBuffer mCombinedBuffer;  // Vertices and normals in one buffer.
    QGLBuffer mIndexedBuffer; // Indexed vertice data buffer (should use less memory)

    // Each mesh can be moved and rotated independently
    QVector<GLfloat> mPosition;     // Mesh is in this position
    QVector<GLfloat> mOrientation;  // Orientaion of mesh.

    QString mName;

    MeshMaterial mMaterial;

    bool mUsingIndexBuffer;

    int mStartIndex;    // index to vertice buffer containing every vertice of model.
    int mFaces;
    int mPoints;
};

#endif // MESH3D_H
