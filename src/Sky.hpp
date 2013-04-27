#ifndef SKY_INCLUDED
#define SKY_INCLUDED


#include <QGLFramebufferObject>
#include <QString>
#include <QSize>

#include "Shader.hpp"


class GLWidget;


class Sky
{
public:
	Sky( GLWidget * glWidget, const QString & skyMapPath, const float * timeOfDay );
	~Sky();

	void update( const float & delta );
	void draw( const QVector3D & eye );
	const QVector4D & ambient() const { return mAmbient; }
	const QVector4D & diffuse() const { return mDiffuse; }
	const QVector4D & specular() const { return mSpecular; }
	const QVector4D & sunDirection() const { return mSunDirection; }
	const QVector3D & sunAxis() const { return mSunAxis; }
	void setSunAxis( const QVector3D & axis ) { mSunAxis = axis.normalized(); }

	GLWidget * getGLWidget() { return mGLWidget; }

private:
	GLWidget * mGLWidget;
	const float * mTimeOfDay;
	QImage mSkyDomeImage;
	Shader * mSkyDomeShader;
	int mSkyDomeShader_sunDir;
	int mSkyDomeShader_timeOfDay;
	int mSkyDomeShader_sunBrightness;
	float mSunBrightness;
	QGLBuffer mCubeIndexBuffer;
	QGLBuffer mCubeVertexBuffer;
	GLuint mSkyDomeMap;
	QVector4D mAmbient;
	QVector4D mDiffuse;
	QVector4D mSpecular;
	QVector4D mSunDirection;
	QVector3D mSunAxis;
};


#endif
