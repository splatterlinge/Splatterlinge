#ifndef WAVEFRONTMODEL_HPP
#define WAVEFRONTMODEL_HPP

#include "Material.hpp"

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
	QString material;
};

class WavefrontModel
{
public:
	WavefrontModel(QString filename);

	int load();
	bool parse();
	bool render();

private:
	QMap<QString, GLuint> mMap;
	QString mFilename;
	QList<Face> * mFaces;
};

#endif // WAVEFRONTMODEL_HPP
