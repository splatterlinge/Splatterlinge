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
};

class WavefrontModel
{
public:
	WavefrontModel( GLWidget * widget, QString filename );
	QSizeF getSize() { return mSize; }
	GLuint getIndex() { return mIndex; }

	bool parse();
	bool render();

private:
	GLWidget * mGLWidget;
	QString mFilename;
    QVector<Face> * mFaces;
	QSizeF mSize;
	GLuint mIndex;
    QGLBuffer mVertexBuffer;
    QGLBuffer mIndexBuffer;
};

#endif // WAVEFRONTMODEL_HPP
