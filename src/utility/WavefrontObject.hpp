#ifndef WAVEFRONTOBJECT_HPP
#define WAVEFRONTOBJECT_HPP

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

class WavefrontObject
{
public:
    WavefrontObject();

    bool load( QString filename );

private:
    QList<QVector4D> * vertices;
    QList<QVector3D> * verticies_textures;
    QList<QVector3D> * normals;
    QList<FacePoint> * faces;
};

#endif // WAVEFRONTOBJECT_HPP
