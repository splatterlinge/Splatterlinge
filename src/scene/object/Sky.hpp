#ifndef SCENE_OBJECT_SKY_INCLUDED
#define SCENE_OBJECT_SKY_INCLUDED


#include <GLWidget.hpp>

#include <QGLFramebufferObject>
#include <QGLBuffer>
#include <QString>
#include <QSize>

#include <resource/Shader.hpp>
#include <scene/object/AObject.hpp>


class Scene;


/// Sky renderer
/**
 * Simulates and renders a sky.
 */
class Sky : public AObject
{
public:
	Sky( Scene * scene, QString name );
	~Sky();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	/// Sets the time of the day.
	/**
	 * The time of the day is stored as a float value.
	 * Only the fractional part is used in the sky simulation.
	 * A value of 0.0 represents sunrise, on 0.25 the sunis on it's zenith and 0.5 represents sunset.
	 * The night lasts from 0.5 to 1.0(0.0).
	 * This cycle continues periodically
	 * @param timeOfDay The current time of the day.
	 */
	void setTimeOfDay( const float & timeOfDay );

	/// Returns the time of the day.
	const float & timeOfDay() const { return mTimeOfDay; }

	/// The color on the horizon.
	const QVector4D & baseColor() const { return mBaseColor; }
	/// Ambient lighting color.
	const QVector4D & ambient() const { return mAmbient; }
	/// Diffuse lighting color.
	const QVector4D & diffuse() const { return mDiffuse; }
	/// Specular lighting color.
	const QVector4D & specular() const { return mSpecular; }
	/// A vector pointing to the sun.
	const QVector4D & sunDirection() const { return mSunDirection; }
	/// The sky's rotation axis.
	const QVector3D & axis() const { return mAxis; }

private:
	static const GLfloat sCubeVertices[];
	static const GLushort sCubeIndices[];
	static QGLBuffer sCubeIndexBuffer;
	static QGLBuffer sCubeVertexBuffer;

	QGLBuffer sCloudPlaneIndexBuffer;
	QGLBuffer sCloudPlaneVertexBuffer;
	int mCloudPlaneRes;

	QVector4D mBaseColor;
	QVector4D mAmbient;
	QVector4D mDiffuse;
	QVector4D mSpecular;
	QVector4D mSunDirection;
	QVector3D mSunInitialDir;
	QVector3D mAxis;

	Shader * mStarCubeShader;
	GLuint mStarCubeMap;

	Shader * mDomeShader;
	int mDomeShader_sunDir;
	int mDomeShader_timeOfDay;
	int mDomeShader_sunSpotPower;
	int mDomeShader_diffuseMap;
	GLuint mDomeMap;
	QImage mDomeImage;

	float mTimeOfDay;
	float mSunSpotPower;

	float mDiffuseFactorDay;
	float mDiffuseFactorNight;
	float mDiffuseFactorMax;
	float mSpecularFactorDay;
	float mSpecularFactorNight;
	float mSpecularFactorMax;
	float mAmbientFactorDay;
	float mAmbientFactorNight;
	float mAmbientFactorMax;

	void drawStarCube();
	void drawSky();
};


#endif
