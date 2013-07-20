#ifndef WAVEFRONTOBJECT_HPP
#define WAVEFRONTOBJECT_HPP

#include <scene/Scene.hpp>
#include <resource/WavefrontModel.hpp>

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

class WavefrontObject : public AObject
{
public:
	WavefrontObject( Scene * scene, QString filename, float scale = 1.0f );
	virtual ~WavefrontObject();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

private:
	Scene * mScene;
	float mScale;
	GLuint mIndex;
};

#endif // WAVEFRONTOBJECT_HPP
