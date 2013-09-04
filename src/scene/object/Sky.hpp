/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCENE_OBJECT_SKY_INCLUDED
#define SCENE_OBJECT_SKY_INCLUDED

#include "AWorldObject.hpp"

#include <GLWidget.hpp>

#include <QGLBuffer>
#include <QString>
#include <QSize>

#include <resource/Shader.hpp>
#include <scene/object/AObject.hpp>
#include <utility/OcclusionTest.hpp>
#include <geometry/Vertex.hpp>


class Scene;


/// Sky renderer
/**
 * Simulates and renders a sky.
 */
class Sky : public AWorldObject
{
public:
	Sky( World * world, QString name );
	~Sky();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void draw2Self();

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

	Shader * mCloudShader;
	int mCloudShader_cloudMap;
	int mCloudShader_cloudiness;
	int mCloudShader_smoothness;
	int mCloudShader_horizonFade;
	float mCloudSmoothness;
	float mCloudCloudiness;
	float mCloudHorizonFade;
	QVector<unsigned short> mCloudPlaneIndex;
	QVector<VertexP3fT2f> mCloudPlaneVertices;
	QGLBuffer mCloudPlaneIndexBuffer;
	QGLBuffer mCloudPlaneVertexBuffer;
	int mCloudPlaneRes;
	GLuint mCloudMap;

	QVector4D mBaseColor;
	QVector4D mAmbient;
	QVector4D mDiffuse;
	QVector4D mSpecular;
	QVector4D mSunDirection;
	QVector3D mSunInitialDir;
	QVector3D mAxis;

	Shader * mStarCubeShader;
	int mStarCubeShader_cubeMap;
	GLuint mStarCubeMap;

	Shader * mDomeShader;
	int mDomeShader_sunDir;
	int mDomeShader_timeOfDay;
	int mDomeShader_sunSpotPower;
	int mDomeShader_diffuseMap;
	GLuint mDomeMap;
	QImage mDomeImage;

	OcclusionTest mOcclusionTest;
	GLuint mSunFlareMap;
	float mSunFlareSize;

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
	void drawCloudPlane();
	void drawSunFlare();
	static void drawCube( bool texCoords );
	static void drawQuad( bool texCoords );
};


#endif
