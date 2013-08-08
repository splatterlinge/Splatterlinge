#ifndef WAVEFRONTOBJECT_HPP
#define WAVEFRONTOBJECT_HPP

#include <scene/Scene.hpp>
#include <resource/StaticModel.hpp>

#include <QString>
#include <QMatrix4x4>

class WavefrontObject : public AObject
{
public:
	WavefrontObject( Scene * scene, QString filename, QVector<QMatrix4x4> instances );
	virtual ~WavefrontObject();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

private:
	StaticModel * mModel;
	Scene * mScene;
	QString mFilename;
};

#endif // WAVEFRONTOBJECT_HPP
