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

#include "Sky.hpp"
#include "World.hpp"

#include <utility/Interpolation.hpp>
#include <resource/Shader.hpp>
#include <scene/TextureRenderer.hpp>

#include <QGLShaderProgram>
#include <QSettings>
#include <QString>
#include <QImage>
#include <GL/glu.h>
#include <math.h>
#include <QDebug>


static void TexImage( QGLWidget * glWidget, GLenum target, QString path )
{
	QImage image( path );
	if( image.isNull() )
	{
		qFatal( "\"%s\" not found!", path.toLocal8Bit().constData() );
	}
	QImage glImage = QGLWidget::convertToGLFormat( image );
	glTexImage2D( target, 0, GL_RGBA8, glImage.width(), glImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits() );
}


const GLfloat Sky::sCubeVertices[] =
{	// positions		texcoords
	-1.0,  1.0,  1.0,	0.0, 1.0,
	-1.0, -1.0,  1.0,	0.0, 0.0,
	 1.0, -1.0,  1.0,	1.0, 0.0,
	 1.0,  1.0,  1.0,	1.0, 1.0,
	-1.0,  1.0, -1.0,	0.0, 1.0,
	-1.0, -1.0, -1.0,	0.0, 0.0,
	 1.0, -1.0, -1.0,	1.0, 0.0,
	 1.0,  1.0, -1.0,	1.0, 1.0
};

QGLBuffer Sky::sCubeVertexBuffer;


const GLushort Sky::sCubeIndices[] =
{
	0, 1, 2, 3,
	3, 2, 6, 7,
	7, 6, 5, 4,
	4, 5, 1, 0,
	0, 3, 7, 4,
	5, 6, 2, 1
};

QGLBuffer Sky::sCubeIndexBuffer;


void Sky::drawCube( bool texCoords )
{
	sCubeVertexBuffer.bind();
	sCubeIndexBuffer.bind();
	glEnableClientState( GL_INDEX_ARRAY );
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 5*sizeof(GL_FLOAT), 0 );
	if( texCoords )
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer( 2, GL_FLOAT, 5*sizeof(GL_FLOAT), (const GLvoid*)(3*sizeof(GL_FLOAT)) );
	}

	glDrawElements( GL_QUADS, sizeof(sCubeIndices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0 );

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	sCubeVertexBuffer.release();
	sCubeIndexBuffer.release();
}


void Sky::drawQuad( bool texCoords )
{
	sCubeVertexBuffer.bind();
	sCubeIndexBuffer.bind();
	glEnableClientState( GL_INDEX_ARRAY );
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 5*sizeof(GL_FLOAT), 0 );
	if( texCoords )
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer( 2, GL_FLOAT, 5*sizeof(GL_FLOAT), (const GLvoid*)(3*sizeof(GL_FLOAT)) );
	}

	glDrawElements( GL_QUADS, 4, GL_UNSIGNED_SHORT, 0 );

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	sCubeVertexBuffer.release();
	sCubeIndexBuffer.release();
}


Sky::Sky( World * world, QString name ) :
	AWorldObject( world ),
	mTimeOfDay( 0 )
{
	QSettings s( "./data/sky/"+name+"/sky.ini", QSettings::IniFormat );

	s.beginGroup( "Sky" );
		QString skyDomePath = "./data/sky/"+name+'/'+s.value( "domeMapPath", "dome.png" ).toString();
		QString sunFlarePath = "./data/sky/"+name+'/'+s.value( "sunFlarePath", "flare.png" ).toString();
		mSunFlareSize = s.value( "sunFlareSize", 0.6 ).toFloat();
		mAxis = QVector3D(
			s.value( "axisX", 1.0f ).toFloat(),
			s.value( "axisY", 0.0f ).toFloat(),
			s.value( "axisZ", 1.0f ).toFloat()
		).normalized();
		mSunInitialDir = QVector3D(
			s.value( "sunInitialX", 0.0f ).toFloat(),
			s.value( "sunInitialY", 0.0f ).toFloat(),
			s.value( "sunInitialZ", -1.0f ).toFloat()
		).normalized();
		mSunSpotPower = s.value( "sunSpotPower", 10.0f ).toFloat();
		mDiffuseFactorDay = s.value( "diffuseFactorDay", 3.0f ).toFloat();
		mDiffuseFactorNight = s.value( "diffuseFactorNight", 0.2f ).toFloat();
		mDiffuseFactorMax = s.value( "diffuseFactorMax", 2.0f ).toFloat();
		mSpecularFactorDay = s.value( "specularFactorDay", 2.0f ).toFloat();
		mSpecularFactorNight = s.value( "specularFactorNight", 0.3f ).toFloat();
		mSpecularFactorMax = s.value( "specularFactorMax", 2.0f ).toFloat();
		mAmbientFactorDay = s.value( "ambientFactorDay", 0.3f ).toFloat();
		mAmbientFactorNight = s.value( "ambientFactorNight", 0.1f ).toFloat();
		mAmbientFactorMax = s.value( "ambientFactorMax", 0.2f ).toFloat();
	s.endGroup();

	s.beginGroup( "StarMap" );
		QString starMapPathPX = "./data/sky/"+name+'/'+s.value( "positiveX", "star.px.png" ).toString();
		QString starMapPathNX = "./data/sky/"+name+'/'+s.value( "negativeX", "star.nx.png" ).toString();
		QString starMapPathPY = "./data/sky/"+name+'/'+s.value( "positiveY", "star.py.png" ).toString();
		QString starMapPathNY = "./data/sky/"+name+'/'+s.value( "negativeY", "star.ny.png" ).toString();
		QString starMapPathPZ = "./data/sky/"+name+'/'+s.value( "positiveZ", "star.pz.png" ).toString();
		QString starMapPathNZ = "./data/sky/"+name+'/'+s.value( "negativeZ", "star.nz.png" ).toString();
	s.endGroup();

	s.beginGroup( "CloudPlane" );
		QString cloudMapPath = "./data/sky/"+name+'/'+s.value( "map", "clouds.png" ).toString();
		float cloudScale = s.value( "scale", 10.0f ).toFloat();
		float cloudCurvature = s.value( "curvature", 1.5f ).toFloat();
		float cloudOffset = s.value( "offset", 1.0f ).toFloat();
		mCloudCloudiness = s.value( "cloudiness", 0.1f ).toFloat();
		mCloudSmoothness = s.value( "smoothness", 0.6f ).toFloat();
		mCloudHorizonFade = s.value( "horizonFade", 8.0f ).toFloat();
	s.endGroup();

	QImage cloudImage( cloudMapPath );
	if( cloudImage.isNull() )
	{
		qFatal( "\"%s\" not found!", cloudMapPath.toLocal8Bit().constData() );
	}
	mCloudMap = scene()->glWidget()->bindTexture( cloudImage );

	mCloudPlaneRes = 10;
	mCloudPlaneVertices.resize( mCloudPlaneRes * mCloudPlaneRes );
	for( int z = 0; z < mCloudPlaneRes; ++z )
	{
		for( int x = 0; x < mCloudPlaneRes; ++x )
		{
			float divX = ((float)x/(float)(mCloudPlaneRes-1))*2.0f-1.0f;
			float divZ = ((float)z/(float)(mCloudPlaneRes-1))*2.0f-1.0f;
			float a = divX*((float)M_PI/2.0f);
			float b = divZ*((float)M_PI/2.0f);
			mCloudPlaneVertices[x+z*mCloudPlaneRes].position = QVector3D(
				cloudScale * divX,
				cloudOffset - (2.0f-(cosf(a)+cosf(b)))*cloudCurvature,
				cloudScale * divZ
			);
			mCloudPlaneVertices[x+z*mCloudPlaneRes].texCoord = QVector2D(
				divX/2.0f,
				divZ/2.0f
			);
		}
	}
	mCloudPlaneVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
	mCloudPlaneVertexBuffer.create();
	mCloudPlaneVertexBuffer.bind();
	mCloudPlaneVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mCloudPlaneVertexBuffer.allocate( mCloudPlaneVertices.constData(), mCloudPlaneVertices.size()*sizeof(VertexP3fT2f) );
	mCloudPlaneVertexBuffer.release();

	mCloudPlaneIndex.resize( 2 * (mCloudPlaneRes-1) * mCloudPlaneRes );
	for( int z = 0; z < mCloudPlaneRes-1; ++z )
	{
		for( int x=0; x<mCloudPlaneRes; ++x )
		{
			mCloudPlaneIndex[2*(x+mCloudPlaneRes*z)] = x + z*mCloudPlaneRes;
			mCloudPlaneIndex[2*(x+mCloudPlaneRes*z)+1] = x + (z+1)*mCloudPlaneRes;
		}
	}
	mCloudPlaneIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
	mCloudPlaneIndexBuffer.create();
	mCloudPlaneIndexBuffer.bind();
	mCloudPlaneIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mCloudPlaneIndexBuffer.allocate( mCloudPlaneIndex.constData(), mCloudPlaneIndex.size()*sizeof(unsigned short) );
	mCloudPlaneIndexBuffer.release();

	mCloudShader = new Shader( scene()->glWidget(), "cloud" );
	mCloudShader_cloudMap = mCloudShader->program()->uniformLocation( "cloudMap" );
	mCloudShader_cloudiness = mCloudShader->program()->uniformLocation( "cloudiness" );
	mCloudShader_smoothness = mCloudShader->program()->uniformLocation( "smoothness" );
	mCloudShader_horizonFade = mCloudShader->program()->uniformLocation( "horizonFade" );

	if( !sCubeVertexBuffer.isCreated() )
	{
		sCubeVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
		sCubeVertexBuffer.create();
		sCubeVertexBuffer.bind();
		sCubeVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
		sCubeVertexBuffer.allocate( sCubeVertices, sizeof(sCubeVertices) );
		sCubeVertexBuffer.release();
	}

	if( !sCubeIndexBuffer.isCreated() )
	{
		sCubeIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
		sCubeIndexBuffer.create();
		sCubeIndexBuffer.bind();
		sCubeIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
		sCubeIndexBuffer.allocate( sCubeIndices, sizeof(sCubeIndices) );
		sCubeIndexBuffer.release();
	}

	mDomeShader = new Shader( scene()->glWidget(), "skydome" );
	mDomeShader_sunDir = mDomeShader->program()->uniformLocation( "sunDir" );
	mDomeShader_timeOfDay = mDomeShader->program()->uniformLocation( "timeOfDay" );
	mDomeShader_sunSpotPower = mDomeShader->program()->uniformLocation( "sunSpotPower" );
	mDomeShader_diffuseMap = mDomeShader->program()->uniformLocation( "diffuseMap" );

	mDomeImage = QImage( skyDomePath );
	if( mDomeImage.isNull() )
	{
		qFatal( "\"%s\" not found!", skyDomePath.toLocal8Bit().constData() );
	}

	mDomeMap = scene()->glWidget()->bindTexture( mDomeImage );
	if( mDomeMap >= 0 )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mDomeMap );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}

	mStarCubeShader = new Shader( scene()->glWidget(), "cube" );
	mStarCubeShader_cubeMap = mDomeShader->program()->uniformLocation( "cubeMap" );

	glGenTextures( 1, &mStarCubeMap );
	glBindTexture( GL_TEXTURE_CUBE_MAP, mStarCubeMap );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	TexImage( scene()->glWidget(), GL_TEXTURE_CUBE_MAP_POSITIVE_X, starMapPathPX );
	TexImage( scene()->glWidget(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X, starMapPathNX );
	TexImage( scene()->glWidget(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y, starMapPathPY );
	TexImage( scene()->glWidget(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, starMapPathNY );
	TexImage( scene()->glWidget(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z, starMapPathPZ );
	TexImage( scene()->glWidget(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, starMapPathNZ );

	QImage sunFlareImage( sunFlarePath );
	if( sunFlareImage.isNull() )
	{
		qFatal( "\"%s\" not found!", cloudMapPath.toLocal8Bit().constData() );
	}
	mSunFlareMap = scene()->glWidget()->bindTexture( sunFlareImage );
}


Sky::~Sky()
{
	scene()->glWidget()->deleteTexture( mDomeMap );
	scene()->glWidget()->deleteTexture( mStarCubeMap );
	scene()->glWidget()->deleteTexture( mSunFlareMap );
	delete mDomeShader;
	delete mStarCubeShader;
	delete mCloudShader;
	mCloudPlaneIndexBuffer.destroy();
	mCloudPlaneVertexBuffer.destroy();

//	sCubeIndexBuffer.destroy();
//	sCubeVertexBuffer.destroy();
}


void Sky::setTimeOfDay( const float & timeOfDay )
{
	mTimeOfDay = timeOfDay;
	if( mTimeOfDay > 1.0f )
		mTimeOfDay -= floorf( mTimeOfDay );
	else if( mTimeOfDay < 0.0f )
		mTimeOfDay -= floorf( mTimeOfDay );
}


void Sky::updateSelf( const double & delta )
{
	float angle = mTimeOfDay*(M_PI*2.0f);
	QMatrix4x4 m;
	m.setToIdentity();
	m.rotate( angle*(180.0/M_PI), mAxis );
	mSunDirection = m.map( mSunInitialDir );

	float skyMapTime = mTimeOfDay * (float)mDomeImage.width() - 0.5f;

	int xA = (int)floorf( skyMapTime );
	if( xA < 0 )
		xA += mDomeImage.width();
	QRgb colorA = mDomeImage.pixel( xA, mDomeImage.height()-1 );

	int xB = xA + 1;
	if( xB >= mDomeImage.width() )
		xB -= mDomeImage.width();
	QRgb colorB = mDomeImage.pixel( xB, mDomeImage.height()-1 );

	QVector4D colorAF( qRed(colorA), qGreen(colorA), qBlue(colorA), qAlpha(colorA) );
	colorAF /= 255.0f;
	QVector4D colorBF( qRed(colorB), qGreen(colorB), qBlue(colorB), qAlpha(colorB) );
	colorBF /= 255.0f;

	mBaseColor = Interpolation::linear( colorAF, colorBF, skyMapTime-floorf( skyMapTime ) );

	float diffuseFactor = Interpolation::linear( mDiffuseFactorDay, mDiffuseFactorNight, (float)((sunDirection().y()+1.0f)*0.5f) );
	if( diffuseFactor > mDiffuseFactorMax )
		diffuseFactor = mDiffuseFactorMax;

	float specularFactor = Interpolation::linear( mSpecularFactorDay, mSpecularFactorNight, (float)((sunDirection().y()+1.0f)*0.5f) );
	if( diffuseFactor > mSpecularFactorMax )
		diffuseFactor = mSpecularFactorMax;

	float ambientFactor = Interpolation::linear( mAmbientFactorDay, mAmbientFactorNight, (float)((sunDirection().y()+1.0f)*0.5f) );
	if( ambientFactor > mAmbientFactorMax )
		ambientFactor = mAmbientFactorMax;

	mDiffuse = mBaseColor.toVector3D() * diffuseFactor;
	mSpecular = mBaseColor.toVector3D() * specularFactor;
	mAmbient = mBaseColor.toVector3D().normalized() * ambientFactor;
}


void Sky::drawSelf()
{
	scene()->eye()->disableClippingPlanes();
	glPushAttrib( GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();

	QMatrix4x4 eyeRotMat = scene()->eye()->viewMatrix();
	eyeRotMat.setColumn( 3, QVector4D(0,0,0,1) );
	glLoadMatrix( eyeRotMat );

	glDepthMask( GL_FALSE );
	glDepthFunc( GL_EQUAL );
	glDisable( GL_CULL_FACE );
	glDepthRange( 1.0, 1.0 );

	drawStarCube();
	drawSky();
	drawCloudPlane();

	glPopMatrix();
	glPopAttrib();
	scene()->eye()->enableClippingPlanes();
}


void Sky::draw2Self()
{
	scene()->eye()->disableClippingPlanes();
	glPushAttrib( GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();

	QMatrix4x4 eyeRotMat = scene()->eye()->viewMatrix();
	eyeRotMat.setColumn( 3, QVector4D(0,0,0,1) );
	glLoadMatrix( eyeRotMat );

	glDepthMask( GL_FALSE );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	drawSunFlare();

	glPopMatrix();
	glPopAttrib();
	scene()->eye()->enableClippingPlanes();
}


void Sky::drawSunFlare()
{
	if( world()->landscape()->drawingReflection() || world()->landscape()->drawingRefraction() )
		return;

	QVector3D sunPoint( mSunDirection * scene()->eye()->farPlane() );

	const unsigned char samplingPoints = 32;
	unsigned char visiblePoints = 0;
	float occlusionRadius = 0.04f * scene()->eye()->farPlane();
	glPushMatrix();
		glTranslate( sunPoint - mSunDirection.toVector3D()*occlusionRadius );
		glScale( occlusionRadius );
		visiblePoints = mOcclusionTest.randomPointsInUnitSphereVisible( samplingPoints );
	glPopMatrix();
	if( !visiblePoints )
		return;

	glColor( ((float)visiblePoints/(float)samplingPoints) * mDiffuse/2.0f );
	glPushMatrix();
	glRotate( mTimeOfDay*360.0f, mAxis );
	glScalef( mSunFlareSize, mSunFlareSize, 1.0f );
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, mSunFlareMap );
	glPushMatrix();
		glRotate( mTimeOfDay*360.0f*2.0f, 0,0,1 );
		drawQuad( true );
	glPopMatrix();
	glPushMatrix();
		glRotate( -mTimeOfDay*360.0f*3.0f, 0,0,1 );
		drawQuad( true );
	glPopMatrix();
	glDisable( GL_BLEND );
	glPopMatrix();
}


void Sky::drawStarCube()
{
	glPushMatrix();
	glRotate( mTimeOfDay*360.0f, mAxis );
	glBindTexture( GL_TEXTURE_CUBE_MAP, mStarCubeMap );
	mStarCubeShader->bind();
	mStarCubeShader->program()->setUniformValue( mStarCubeShader_cubeMap, 0 );
	drawCube( false );
	mStarCubeShader->release();
	glPopMatrix();
}


void Sky::drawSky()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	mDomeShader->bind();
	mDomeShader->program()->setUniformValue( mDomeShader_sunDir, mSunDirection.toVector3D() );
	mDomeShader->program()->setUniformValue( mDomeShader_timeOfDay, mTimeOfDay );
	mDomeShader->program()->setUniformValue( mDomeShader_sunSpotPower, mSunSpotPower );
	mDomeShader->program()->setUniformValue( mDomeShader_diffuseMap, 0 );
	glBindTexture( GL_TEXTURE_2D, mDomeMap );
	drawCube( false );
	mDomeShader->release();
	glDisable( GL_BLEND );
}


void Sky::drawCloudPlane()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

	glBindTexture( GL_TEXTURE_2D, mCloudMap );

	glMatrixMode( GL_TEXTURE );

	glPushMatrix();
	glTranslatef( mTimeOfDay, 0.0f, 0.0f );

	glActiveTexture( GL_TEXTURE1 );	glPushMatrix();
	glTranslatef( mTimeOfDay, mTimeOfDay, 0.0f );
	glScalef( 0.5, 0.5, 1.0f );

	mCloudShader->bind();
	mCloudShader->program()->setUniformValue( mCloudShader_cloudMap, 0 );
	mCloudShader->program()->setUniformValue( mCloudShader_cloudiness, mCloudCloudiness );
	mCloudShader->program()->setUniformValue( mCloudShader_smoothness, mCloudSmoothness );
	mCloudShader->program()->setUniformValue( mCloudShader_horizonFade, mCloudHorizonFade );
	mCloudPlaneVertexBuffer.bind();
	mCloudPlaneIndexBuffer.bind();
	glEnableClientState( GL_INDEX_ARRAY );
	VertexP3fT2f::glEnableClientState();
	VertexP3fT2f::glPointerVBO();
	for( int slice=0; slice<mCloudPlaneRes-1; slice++ )
	{
		glDrawElements(
			GL_TRIANGLE_STRIP,
			mCloudPlaneRes*2,
			GL_UNSIGNED_SHORT,
			(void*)((size_t)(2*sizeof(unsigned short)*(	// convert index to pointer
				mCloudPlaneRes*slice			// index to start
			) ) )
		);
	}
	glDisableClientState( GL_INDEX_ARRAY );
	VertexP3fT2f::glDisableClientState();
	mCloudPlaneIndexBuffer.release();
	mCloudPlaneVertexBuffer.release();
	mCloudShader->release();

	glActiveTexture( GL_TEXTURE1 );	glPopMatrix();
	glActiveTexture( GL_TEXTURE0 );	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );

	glDisable( GL_BLEND );
}
