/*
* This class uses lib3ds to load 3ds model and
* generates vertex buffers from it
*/

#include <math.h>

#include <QDebug>
#include <QtOpenGL>

#include "mesh3d.h"
#include "model3ds.h"
#include "meshmaterial.h"
#include "carorientation.h"
#include "caranimator.h"

#include <lib3ds/file.h>
#include <lib3ds/mesh.h>

Model3DS::Model3DS(Lib3dsFile *file)
    : mPosition(QVector<GLfloat> (3))
    , mOrientation(QVector<GLfloat> (3))
    , mAspect(1.0)
{
    mTotalFaces = 0;
    mHighlightMesh = -1;
    mMeshes.clear();
    mAnimators.clear();

    mPosition.fill(0.0);
    mOrientation.fill(0.0);

    CreateMeshes(file);
}

Model3DS::~Model3DS()
{
    qDeleteAll(mAnimators);
    qDeleteAll(mMeshes);
}

void Model3DS::NextMesh()
{
    mHighlightMesh++;
    if (mHighlightMesh >= mMeshCount) {
        mHighlightMesh = 0;
    }
}

void Model3DS::PreviousMesh()
{
    mHighlightMesh--;
    if (mHighlightMesh < 0) {
        mHighlightMesh = mMeshCount-1;
    }
}

void Model3DS::SetHighlightMesh(int index)
{
    if (index >= mMeshCount) {
        index = -1;
    }
    mHighlightMesh = index;
}

void Model3DS::setTransformation(QVector<GLfloat> pos, QVector<GLfloat> orientation)
{
    if ((pos.count() != 3) || (orientation.count() != 3)) {
        return;
    }
    mPosition = pos;
    mOrientation = orientation;
}

void Model3DS::resetAnimators()
{
    if (mAnimators.count() == 0) {
        for (int i=0; i<mMeshes.count(); i++) {
            mAnimators.append(new CarAnimator);
        }
    }

    QVector<GLfloat> posFrom(3);
    QVector<GLfloat> orFrom(3);
    QVector<GLfloat> posTo(3);
    QVector<GLfloat> orTo(3);
    posTo.fill(0.0);
    orTo.fill(0.0);
    posFrom.fill(0.0);
    orFrom.fill(0.0);

    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    int min = -5;
    int max = 5;
    int minRot = -180;
    int maxRot = 180;
    int minStep = 50;
    int maxStep = 75;

    for (int i=0; i<mMeshes.count(); i++){
        GLfloat rnd;
        rnd = ( qrand() / (RAND_MAX + 1.0) * (max + 1 - min) + min );
        posFrom[0] = rnd;
        rnd = ( qrand() / (RAND_MAX + 1.0) * (max + 1 - min) + min );
        posFrom[1] = rnd;
        rnd = ( qrand() / (RAND_MAX + 1.0) * (max + 1 - min) + min );
        posFrom[2] = rnd;

        rnd = ( qrand() / (RAND_MAX + 1.0) * (maxRot + 1 - minRot) + minRot );
        orFrom[0] = rnd;
        rnd = ( qrand() / (RAND_MAX + 1.0) * (maxRot + 1 - minRot) + minRot );
        orFrom[1] = rnd;
        rnd = ( qrand() / (RAND_MAX + 1.0) * (maxRot + 1 - minRot) + minRot );
        orFrom[2] = rnd;

        int steps = ( qrand() / (RAND_MAX + 1.0) * (maxStep + 1 - minStep) + minStep );

        CarOrientation orientation;
        orientation.mPosition = posFrom;
        orientation.mOrientation = orFrom;
        mAnimators.at(i)->setOrientation(orientation);

        orientation.mPosition = posTo;
        orientation.mOrientation = orTo;
        mAnimators.at(i)->setOrientationTo(orientation);

        mAnimators.at(i)->setAnimationSteps(steps);
        mMeshes.at(i)->setTransformation(posFrom,orFrom);
    }
}

bool Model3DS::animate()
{

    bool changed = false;

    for (int i = 0; i < mMeshCount; ++i)
    {
        changed |= mAnimators.at(i)->updateAnimation();
        mMeshes.at(i)->setTransformation(mAnimators.at(i)->position(),mAnimators.at(i)->orientation());
    }

    return changed;
}

void Model3DS::setAspectRatio(int width, int height)
{
    mAspect = 1.0;
    if (height != 0)
    {
        float w = width;
        float h = height;
        mAspect = w / h;
    }
    // TODO: magic numbers (these could/should be settable by widget?)
    float fov = 60.0;
    float nearPlane = 0.5;
    float farPlane = -0.2;

    // Calculate perspective
    mPerspective.setToIdentity();
    mPerspective.perspective(fov,mAspect,nearPlane,farPlane);
}

void Model3DS::generateShaders()
{
    // Each mesh has own shader
    for (int i=0; i<mMeshCount; i++) {
        mMeshes.at(i)->generateShader();
    }
}

Model3DS *Model3DS::create(const QString &filename)
{
    Model3DS *result = 0;
    Lib3dsFile *file = 0;

    file = lib3ds_file_load(filename.toLocal8Bit().constData());
    if (file) {
        result = new Model3DS(file);
        lib3ds_file_free(file);
    }

    return result;
}

void Model3DS::CountFaces(Lib3dsFile *file)
{
    mTotalFaces = 0;
    mTotalPoints = 0;
    mMeshCount = 0;
    Lib3dsMesh *mesh;

    // default material for mesh
    MeshMaterial material;
    material.SetMaterialColor(QColor::fromRgb(23,165,189));
    material.SetSpecularColor(QColor::fromRgb(23,165,189));
    material.SetEmissionColor(QColor::fromRgbF(0,0,0));
    material.SetShininess(80);

    // Loop through every mesh
    for(mesh = file->meshes;mesh != NULL;mesh = mesh->next)
    {
        mMeshes.append(new Mesh3d(mesh->faces, mesh->points));
        mMeshes.at(mMeshCount)->setName(QString(mesh->name));
        mMeshes.at(mMeshCount)->setMaterial(material);
        mTotalFaces += mesh->faces;
        mTotalPoints += mesh->points;
        mMeshCount++;
    }

//    qDebug() << "Total faces: " << mTotalFaces;
//    qDebug() << "Total points: " << mTotalPoints;
//    qDebug() << "Total meshes: " << mMeshCount;

}

void Model3DS::CreateMeshes(Lib3dsFile *file)
{

    // Calculate the number of faces we have in total
    CountFaces(file);

    // Allocate memory for our vertices and normals
    Lib3dsVector *vertices = new Lib3dsVector[mTotalFaces * 3];
    Lib3dsVector *normals = new Lib3dsVector[mTotalFaces * 3];
    Lib3dsWord *indices = new Lib3dsWord[mTotalFaces * 3];  // there are as many indices as faces

    Lib3dsMesh *mesh;
    unsigned int finishedFaces = 0;
    unsigned int finishedMeshes = 0;

    Lib3dsVector max;
    Lib3dsVector min;
    max[0] = 0.0;
    max[1] = 0.0;
    max[2] = 0.0;
    min[0] = 0.0;
    min[1] = 0.0;
    min[2] = 0.0;

    // Loop through all the meshes
    for(mesh = file->meshes;mesh != NULL;mesh = mesh->next) {
        unsigned int facesInMesh = 0;
        lib3ds_mesh_calculate_normals(mesh, &normals[finishedFaces*3]);

        mMeshes.at(finishedMeshes)->setStartIndex(finishedFaces);

        // Loop through every face
        for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++) {
            Lib3dsFace * face = &mesh->faceL[cur_face];
            for(unsigned int i = 0;i < 3;i++) {
                memcpy(&vertices[finishedFaces*3 + i], mesh->pointL[face->points[i]].pos, sizeof(Lib3dsVector));
                memcpy(&indices[finishedFaces*3 + i], &(face->points[i]), sizeof(Lib3dsWord));

                // Find out maximum values for coordinates
                if (vertices[finishedFaces*3][i] > max[i]) {
                    max[i] = vertices[finishedFaces*3][i];
                }
                if (vertices[finishedFaces*3][i] < min[i]) {
                    min[i] = vertices[finishedFaces*3][i];
                }
            }
            finishedFaces++;
            facesInMesh++;
        }
        //        qDebug() << "Faces in mesh" << finishedMeshes << ":" << facesInMesh;
        //        qDebug() << "First indices:" << indices[finishedFaces+0] << indices[finishedFaces+1] << indices[finishedFaces+2] << indices[finishedFaces+3];
        finishedMeshes++;
    }

    // Scale model to -1 to 1 coordinate range
    float prescale = max[0];
    for (unsigned int i = 0; i<3; ++i) {
        min[i] = abs(min[i]);
        if (max[i] > prescale) {
            prescale = max[i];
        }
        if (min[i] > prescale) {
            prescale = min[i];
        }
    }

    //    qDebug() << "Maximum absolute coordinate: " << prescale;

    for ( unsigned int i = 0; i < mTotalFaces * 3; ++i) {
        vertices[i][0] = vertices[i][0] / prescale;
        vertices[i][1] = vertices[i][1] / prescale;
        vertices[i][2] = vertices[i][2] / prescale;
    }

    for ( unsigned int i = 0; i < mTotalFaces * 3; ++i) {
        // normalize normals, so they don't need to be normalized per frame
        float x = normals[i][0] / prescale;
        float y = normals[i][1] / prescale;
        float z = normals[i][2] / prescale;
        float len = sqrt(x*x + y*y + z*z);

        normals[i][0] = x / len;
        normals[i][1] = y / len;
        normals[i][2] = z / len;
    }

    for ( int i = 0; i < mMeshes.count(); i++ ) {
#if defined (QT_OPENGL_ES_2)
        mMeshes.at(i)->generateBuffers(vertices,normals,indices); // Should be used on panda (but messes smoothing)
#else
        mMeshes.at(i)->generateBuffers(vertices,normals); // ES2
#endif
    }

    delete [] indices;
    delete [] vertices;
    delete [] normals;

    resetAnimators();
}

void Model3DS::Draw() const
{
    if (mTotalFaces > 0)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        GLenum drawmode(GL_LINES);

        for (int i=0; i<mMeshCount; i++) {
            Mesh3d *mesh = mMeshes.at(i);
            if (i == mHighlightMesh) {
                mesh->setColor(QVector3D(1.0, 0.0, 0.0));
                drawmode = GL_TRIANGLES;
            } else {
                mesh->setColor(QVector3D(0.09, 0.65, 0.74));
                drawmode = GL_LINES;
            }
            mesh->createTransformationMatrixES2(mPosition,mOrientation);
            mesh->drawMeshEs2(drawmode,mPerspective);
        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }
}
