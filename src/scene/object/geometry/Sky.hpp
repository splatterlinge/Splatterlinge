#ifndef SCENE_OBJECT_GEOMETRY_SKY_INCLUDED
#define SCENE_OBJECT_GEOMETRY_SKY_INCLUDED


#include <QGLFramebufferObject>
#include <QString>
#include <QSize>

#include "GL/glext.h"
#include <resource/Shader.hpp>


class GLWidget;


/// Sky renderer
/**
 * 
 */
class Sky
{
public:
	Sky( GLWidget * glWidget, QString name, const float * timeOfDay );
	~Sky();

	void update( const float & delta );
	void draw( const QVector3D & eye );
	const QVector4D & fogColor() const { return mFogColor; }
	const QVector4D & ambient() const { return mAmbient; }
	const QVector4D & diffuse() const { return mDiffuse; }
	const QVector4D & specular() const { return mSpecular; }
	const QVector4D & sunDirection() const { return mSunDirection; }
	const QVector3D & axis() const { return mAxis; }

	GLWidget * getGLWidget() { return mGLWidget; }

private:
	GLWidget * mGLWidget;
	const float * mTimeOfDay;
	QImage mSkyDomeImage;
	Shader * mDomeShader;
	Shader * mStarCubeShader;
	int mDomeShader_sunDir;
	int mDomeShader_timeOfDay;
	int mDomeShader_sunSpotPower;
	float mSunSpotPower;
	QGLBuffer mCubeIndexBuffer;
	QGLBuffer mCubeVertexBuffer;
	GLuint mDomeMap;
	GLuint mStarCubeMap;
	QVector4D mFogColor;
	QVector4D mAmbient;
	QVector4D mDiffuse;
	QVector4D mSpecular;
	QVector4D mSunDirection;
	QVector3D mSunInitialDir;
	QVector3D mAxis;
	
	float mDiffuseFactorDay;
	float mDiffuseFactorNight;
	float mDiffuseFactorMax;
	float mSpecularFactorDay;
	float mSpecularFactorNight;
	float mSpecularFactorMax;
	float mAmbientFactorDay;
	float mAmbientFactorNight;
	float mAmbientFactorMax;
};


#endif
