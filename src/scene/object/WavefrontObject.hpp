#ifndef WAVEFRONTOBJECT_HPP
#define WAVEFRONTOBJECT_HPP

#include "../Scene.hpp"
#include "resource/Material.hpp"

#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QTextStream>
#include <QVector3D>
#include <QVector4D>
#include <QRgb>
#include <QMap>

struct FacePoint
{
	const QVector3D * vertex;
	const QVector3D * texCoord;
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

class WavefrontObject : public AObject
{
public:
	WavefrontObject( Scene * scene, QString filename );
	virtual ~WavefrontObject();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	bool parseObj( QString filename );

private:
	Scene * mScene;
	float mScale;
	QString mMtllib;
	QList<QVector3D> * mVertices;
	QList<QVector3D> * mTextureVertices;
	QList<QVector3D> * mNormals;
	QList<Face> * mFaces;
	Material * mMaterial;
};

#endif // WAVEFRONTOBJECT_HPP
