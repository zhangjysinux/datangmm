#ifndef MODEL3DS_H
#define MODEL3DS_H

#include <QString>
#include <QtOpenGL>

class CarAnimator;
class Lib3dsFile;
class Mesh3d;

class Model3DS
{
public:
    virtual ~Model3DS();

    void Draw() const;

    void NextMesh();
    void PreviousMesh();
    void SetHighlightMesh(int index);
    void setTransformation(QVector<GLfloat> pos, QVector<GLfloat> orientation);
    void resetAnimators();
    bool animate(); // animation event. Animates individual meshes

    // ES2.0
    void setAspectRatio(int width, int height);
    void generateShaders();

    static Model3DS *create(const QString &filename);

private:
    Model3DS(Lib3dsFile *file);

    void CreateMeshes(Lib3dsFile *file);
    void CountFaces(Lib3dsFile *file);

    unsigned int mTotalFaces;
    unsigned int mTotalPoints;
    int mMeshCount;

    QVector<GLfloat> mPosition;         // position & orientation of this model in 3d world.
    QVector<GLfloat> mOrientation;

    QList<Mesh3d*> mMeshes;  // Mesh parameters
    QList<CarAnimator*> mAnimators;     // Animator for each mesh

    int mHighlightMesh;

    float mAspect;  // Aspect ratio to use in matrix transformations
    QMatrix4x4 mPerspective;
};

#endif // MODEL3DS_H
