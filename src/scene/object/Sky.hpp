#ifndef SCENE_OBJECT_SKY_INCLUDED
#define SCENE_OBJECT_SKY_INCLUDED


#include <GLWidget.hpp>

#include <QGLFramebufferObject>
#include <QGLBuffer>
#include <QString>
#include <QSize>

#include <resource/Shader.hpp>
#include <scene/object/AObject.hpp>
#include <scene/Scene.hpp>


class Scene;


/// Sky renderer
/**
 * 
 */
class Sky : public AObject
{
public:
	Sky( Scene * scene, QString name, const float * timeOfDay );
	~Sky();

	virtual void updateSelf( const float & delta );
	virtual void drawSelf();
	const QVector4D & fogColor() const { return mFogColor; }
	const QVector4D & ambient() const { return mAmbient; }
	const QVector4D & diffuse() const { return mDiffuse; }
	const QVector4D & specular() const { return mSpecular; }
	const QVector4D & sunDirection() const { return mSunDirection; }
	const QVector3D & axis() const { return mAxis; }

private:
	const float * mTimeOfDay;
	QImage mSkyDomeImage;
	Shader * mDomeShader;
	Shader * mStarCubeShader;
	int mDomeShader_sunDir;
	int mDomeShader_timeOfDay;
	int mDomeShader_sunSpotPower;
	int mDomeShader_diffuseMap;
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
