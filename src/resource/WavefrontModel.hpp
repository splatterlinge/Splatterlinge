#ifndef WAVEFRONTMODEL_HPP
#define WAVEFRONTMODEL_HPP

#include "Material.hpp"
#include "scene/Scene.hpp"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QSizeF>
#include <QGLBuffer>

class Vertex
{
public:
    Vertex() {}

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;

    bool operator == ( const Vertex & other )
    {
        return position == other.position
                && texCoord == other.texCoord
                && normal == other.normal;
    }
};

class Face
{
public:
    Face() {
        points = new QVector<Vertex>();
    }

    QVector<Vertex> * points;
    Material * material;

    bool operator == ( const Face & other )
    {
        return points == other.points
                && material == other.material;
    }
};

class Part
{
public:
    unsigned int start;
    unsigned int count;
    Material * material;
};

class WavefrontModel
{
public:
    WavefrontModel( GLWidget * widget, QString filename );
    QSizeF getSize() { return mSize; }

    bool parse();
    bool render();

private:
    GLWidget * mGLWidget;
    QString mFilename;
    QSizeF mSize;
    QVector<Part> mParts;
    QVector<Vertex> mVertices;
    QVector<unsigned int> mIndices;
    QGLBuffer mVertexBuffer;
    QGLBuffer mIndexBuffer;
};

#endif // WAVEFRONTMODEL_HPP
