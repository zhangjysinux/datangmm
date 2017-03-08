#include <lib3ds/types.h>
#include "mesh3d.h"

#if !defined(QT_OPENGL_ES_2)
# include <QtOpenGL/QGLFunctions>
#endif


Mesh3d::Mesh3d(int faces, int points)
    : mCombinedBuffer(QGLBuffer::VertexBuffer),
      mIndexedBuffer(QGLBuffer::IndexBuffer),
      mPosition(QVector<GLfloat> (3)),
      mOrientation(QVector<GLfloat> (3)),
      mUsingIndexBuffer(false),
      mFaces(faces),
      mPoints(points)

{
    mPosition.fill(0.0);
    mOrientation.fill(0.0);
    mStartIndex = 0;
}

void Mesh3d::setName(QString name)
{
    mName = name;
}

void Mesh3d::setTransformation(QVector<GLfloat> pos, QVector<GLfloat> orientation)
{
    if ((pos.count() != 3) || (orientation.count() != 3)) {
        return;
    }
    mPosition = pos;
    mOrientation = orientation;
}

void Mesh3d::setStartIndex(int index)
{
    mStartIndex = index;
}

void Mesh3d::setMaterial(MeshMaterial material)
{
    mMaterial = material;
    QVector3D col(material.mMaterialColor.mRGBA[0],
                  material.mMaterialColor.mRGBA[1],
                  material.mMaterialColor.mRGBA[2]);
    mColor = col;
}

void Mesh3d::generateBuffers(Lib3dsVector *vertices, Lib3dsVector *normals)
{
    mUsingIndexBuffer = false;
    mCombinedVertices.clear();
    for (int i=0; i<mFaces*3; i++){
        CombinedVertex v;
        v.x = vertices[mStartIndex*3 + i][0];
        v.y = vertices[mStartIndex*3 + i][1];
        v.z = vertices[mStartIndex*3 + i][2];
        v.nx = normals[mStartIndex*3 + i][0];
        v.ny = normals[mStartIndex*3 + i][1];
        v.nz = normals[mStartIndex*3 + i][2];
        mCombinedVertices << v;
    }
    generateCombinedVBO();
}

void Mesh3d::generateBuffers(Lib3dsVector *vertices, Lib3dsVector *normals, Lib3dsWord *indices)
{
    mUsingIndexBuffer = true;
    mCombinedVertices.clear();
    mIndexBuffer.clear();

    CombinedVertex* vertexData = new CombinedVertex[mPoints];
    Lib3dsWord *indexData = new Lib3dsWord[mFaces*3];

    for (int i=0; i<mFaces*3; i++) {
        indexData[i] = indices[i + mStartIndex*3];
        CombinedVertex v;
        v.x = vertices[i+mStartIndex*3][0];
        v.y = vertices[i+mStartIndex*3][1];
        v.z = vertices[i+mStartIndex*3][2];
        v.nx = normals[i+mStartIndex*3][0];
        v.ny = normals[i+mStartIndex*3][1];
        v.nz = normals[i+mStartIndex*3][2];
        if (indexData[i] > mPoints) {
            qDebug() << "warning: index";
        }
        vertexData[indexData[i]] = v;   // Put vertice to indexed location
    }

    for (int i=0; i<mPoints; i++) {
        mCombinedVertices << vertexData[i];
    }

    for (int i=0; i<mFaces*3; i++) {
        GLushort index = indexData[i];  // propably warning for typecast
        mIndexBuffer << index;
    }

//    generateCombinedVBO();
    // Generate indexed VBO

    mCombinedBuffer.create();
    mCombinedBuffer.setUsagePattern(QGLBuffer::StaticDraw);
    mCombinedBuffer.bind();
    mCombinedBuffer.allocate(mCombinedVertices.constData(), sizeof(CombinedVertex) * mPoints);

    mIndexedBuffer.create();
    mIndexedBuffer.setUsagePattern(QGLBuffer::StaticDraw);
    mIndexedBuffer.bind();
//    qDebug() << "count:" << mIndexedVertices.count();
//    qDebug() << "points:" << mPoints;
    mIndexedBuffer.allocate(mIndexBuffer.constData(), sizeof(GLushort) * mFaces * 3);

    delete [] vertexData;
    delete [] indexData;
//    qDebug() << "vbo:" << mName;
//    qDebug() << "index count:" << mIndexBuffer.count();
//    qDebug() << "vertice count:" << mIndexedBuffer.size();

}

void Mesh3d::drawMeshEs2(GLenum drawmode, QMatrix4x4 perspective)
{
    QMatrix4x4 pvm = perspective * mTransformation;

    mShaderProgram.bind();
    mShaderProgram.setUniformValue(mMatrixUniform, pvm);
    mShaderProgram.setUniformValue(mColorUniform, mColor);
    bindBuffers();
    mShaderProgram.enableAttributeArray(mNormalAttr);
    mShaderProgram.enableAttributeArray(mVertexAttr);
    // TODO: resolve glFuncs only once
#if defined(QT_OPENGL_ES_2)
    glVertexAttribPointer(mVertexAttr, 3, GL_FLOAT, GL_FALSE, sizeof(CombinedVertex), (const void *)0);
    glVertexAttribPointer(mNormalAttr, 3, GL_FLOAT, GL_FALSE, sizeof(CombinedVertex), (const void *) (sizeof(float)*3));
#else
    QGLFunctions glFuncs(QGLContext::currentContext());
    glFuncs.glVertexAttribPointer(mVertexAttr, 3, GL_FLOAT, GL_FALSE, sizeof(CombinedVertex), (const void *)0);
    glFuncs.glVertexAttribPointer(mNormalAttr, 3, GL_FLOAT, GL_FALSE, sizeof(CombinedVertex), (const void *) (sizeof(float)*3));
#endif
    if (mUsingIndexBuffer) {
        glDrawElements(drawmode,mFaces*3,GL_UNSIGNED_SHORT,0);
    } else {
        glDrawArrays(drawmode, 0, mFaces*3);
    }
    mShaderProgram.disableAttributeArray(mNormalAttr);
    mShaderProgram.disableAttributeArray(mVertexAttr);
    releaseBuffers();
    mShaderProgram.release();
}

void Mesh3d::createTransformationMatrixES2(const QVector<GLfloat> pos, const QVector<GLfloat> orientation)
{
    mTransformation.setToIdentity();
    mTransformation.translate(pos[0] + mPosition[0], pos[1] + mPosition[1], pos[2] + mPosition[2]);
    mTransformation.rotate(orientation[0] + mOrientation[0], 1.0f, 0.0f, 0.0f);
    mTransformation.rotate(orientation[1] + mOrientation[1], 0.0f, 1.0f, 0.0f);
    mTransformation.rotate(orientation[2] + mOrientation[2], 0.0f, 0.0f, 1.0f);
}

//color = 23,165,189 = 0.09, 0.65, 0.74
// This can be optimized? Using prenormalized light vectors. (Actually do the real light sources later)
// TODO: Better lightning model...
void Mesh3d::generateShader()
{
    QGLShader *vshader1 = new QGLShader(QGLShader::Vertex);
    const char *vsrc1 =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec3 normal;\n"
        "uniform mediump vec3 vcolor;\n"
        "uniform mediump mat4 matrix;\n"
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
        "    float angle = max(dot(normal, toLight), 0.0);\n"
        "    color = vec4(vcolor * 0.2 + vcolor * 0.8 * angle, 1.0);\n"
        "    color = clamp(color, 0.0, 1.0);\n"
        "    gl_Position = matrix * vertex;\n"
        "}\n";
    vshader1->compileSourceCode(vsrc1);
    QGLShader *fshader1 = new QGLShader(QGLShader::Fragment);
    const char *fsrc1 =
        "varying mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "    gl_FragColor = color;\n"
        "}\n";
    fshader1->compileSourceCode(fsrc1);

    mShaderProgram.addShader(vshader1);
    mShaderProgram.addShader(fshader1);
    mShaderProgram.link();

    mVertexAttr = mShaderProgram.attributeLocation("vertex");
    mNormalAttr = mShaderProgram.attributeLocation("normal");
    mMatrixUniform = mShaderProgram.uniformLocation("matrix");
    mColorUniform = mShaderProgram.uniformLocation("vcolor");
}

void Mesh3d::setColor(QVector3D color)
{
    mColor = color;
}

void Mesh3d::generateCombinedVBO()
{
    mCombinedBuffer.create();
    mCombinedBuffer.setUsagePattern(QGLBuffer::StaticDraw);
    mCombinedBuffer.bind();
    mCombinedBuffer.allocate(mCombinedVertices.constData(), sizeof(CombinedVertex) * 3 * mFaces);
}

void Mesh3d::bindBuffers()
{
    if (mUsingIndexBuffer) {
        mIndexedBuffer.bind();
        mCombinedBuffer.bind();
    } else {
        mCombinedBuffer.bind();
    }
}

void Mesh3d::releaseBuffers()
{
    // release binding. This is needed for 2d drawing to work
    if (mUsingIndexBuffer) {
        mIndexedBuffer.release();
        mCombinedBuffer.release();
    } else {
        mCombinedBuffer.release();
    }
}

