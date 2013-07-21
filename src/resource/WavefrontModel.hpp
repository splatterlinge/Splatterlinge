#ifndef WAVEFRONTMODEL_HPP
#define WAVEFRONTMODEL_HPP

#include "Material.hpp"
#include "scene/Scene.hpp"

#include <QString>
#include <QStringList>
#include <QList>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QSizeF>

struct FacePoint
{
	const QVector3D * vertex;
	const QVector2D * texCoord;
	const QVector3D * normal;
};

class Face
{
public:
	Face() {
		points = new QList<FacePoint>();
	}

	QList<FacePoint> * points;
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
	QList<Face> * mFaces;
	QSizeF mSize;
	GLuint mIndex;
};

#endif // WAVEFRONTMODEL_HPP
