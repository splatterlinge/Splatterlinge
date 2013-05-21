#ifndef WAVEFRONTOBJECT_HPP
#define WAVEFRONTOBJECT_HPP

#include "../Scene.hpp"

#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QVector3D>
#include <QVector4D>



struct FacePoint {
    int vertex;
    int texture;
    int normal;
};

class WavefrontObject : public AObject
{
public:
    WavefrontObject( Scene * scene, const float & size, QString filename );
    virtual ~WavefrontObject();

    virtual void updateSelf( const float & delta );
    virtual void drawSelf();

    bool load( QString filename );
    void draw();

private:
    float mSize;
    QString mMtllib;
    QList<QVector4D> * mVertices;
    QList<QVector3D> * mTextureVertices;
    QList<QVector3D> * mNormals;
    QList<QList<FacePoint> > * mFaces;
};

#endif // WAVEFRONTOBJECT_HPP
