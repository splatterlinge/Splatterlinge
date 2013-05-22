#ifndef WAVEFRONTOBJECT_HPP
#define WAVEFRONTOBJECT_HPP

#include "../Scene.hpp"
#include "resource/Material.hpp"

#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QList>
#include <QVector3D>
#include <QVector4D>
#include <QRgb>
#include <QMap>

struct FacePoint {
	const QVector3D * vertex;
	const QVector3D * texture;
	const QVector3D * normal;
};

class WavefrontObject : public AObject
{
public:
	WavefrontObject( Scene * scene, const float & size, QString filename );
	virtual ~WavefrontObject();

	virtual void updateSelf( const float & delta );
	virtual void drawSelf();

	bool parseObj( QString filename );
	bool parseMtl( QString filename );
	void draw();

private:
	Scene * mScene;
	float mSize;
	float mScale;
	QString mMtllib;
	QList<QVector3D> * mVertices;
	QList<QVector3D> * mTextureVertices;
	QList<QVector3D> * mNormals;
	QList<QList<FacePoint> > * mFaces;
	QMap<QString, Material> * mMaterials;
};

#endif // WAVEFRONTOBJECT_HPP
