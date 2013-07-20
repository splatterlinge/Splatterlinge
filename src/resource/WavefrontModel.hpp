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

static QMap<QString, GLuint> mMap = QMap<QString, GLuint>();

class WavefrontModel
{
public:
	WavefrontModel( GLWidget * widget, QString filename );

	int load();
	bool parse();
	bool render();

private:
	GLWidget * mGLWidget;
	QString mFilename;
	QList<Face> * mFaces;
};

#endif // WAVEFRONTMODEL_HPP
