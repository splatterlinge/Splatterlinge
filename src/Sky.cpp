#include "Sky.hpp"

#include <QString>
#include <QImage>
#include <GL/glu.h>
#include <math.h>
#include <QDebug>

#include "GLWidget.hpp"
#include "Shader.hpp"
#include "util.h"


static GLfloat cubeVertices[] =
{
	-1.0,  1.0,  1.0,
	-1.0, -1.0,  1.0,
	 1.0, -1.0,  1.0,
	 1.0,  1.0,  1.0,
	-1.0,  1.0, -1.0,
	-1.0, -1.0, -1.0,
	 1.0, -1.0, -1.0,
	 1.0,  1.0, -1.0
};


static GLushort cubeIndices[] =
{
	0, 1, 2, 3,
	3, 2, 6, 7,
	7, 6, 5, 4,
	4, 5, 1, 0,
	0, 3, 7, 4,
	5, 6, 2, 1
};


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


Sky::Sky( GLWidget * glWidget, QString name, const float * timeOfDay = 0 ) :
	mGLWidget(glWidget),
	mTimeOfDay(timeOfDay)
{
	QSettings s( "./data/sky/"+name+"/sky.ini", QSettings::IniFormat );

	s.beginGroup( "Sky" );
		QString skyDomePath = "./data/sky/"+name+"/"+s.value( "domeMapPath", "dome.png").toString();
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
		QString starMapPathPX = "./data/sky/"+name+"/"+s.value( "starMapPathPX", "star.px.png").toString();
		QString starMapPathNX = "./data/sky/"+name+"/"+s.value( "starMapPathNY", "star.nx.png").toString();
		QString starMapPathPY = "./data/sky/"+name+"/"+s.value( "starMapPathPZ", "star.py.png").toString();
		QString starMapPathNY = "./data/sky/"+name+"/"+s.value( "starMapPathNX", "star.ny.png").toString();
		QString starMapPathPZ = "./data/sky/"+name+"/"+s.value( "starMapPathPY", "star.pz.png").toString();
		QString starMapPathNZ = "./data/sky/"+name+"/"+s.value( "starMapPathNZ", "star.nz.png").toString();
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

	mCubeVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
	mCubeVertexBuffer.create();
	mCubeVertexBuffer.bind();
	mCubeVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mCubeVertexBuffer.allocate( cubeVertices, sizeof(cubeVertices) );

	mCubeIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
	mCubeIndexBuffer.create();
	mCubeIndexBuffer.bind();
	mCubeIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mCubeIndexBuffer.allocate( cubeIndices, sizeof(cubeIndices) );
	
	mDomeShader = new Shader( glWidget, "skydome" );
	mDomeShader_sunDir = mDomeShader->program()->uniformLocation( "sunDir" );
	mDomeShader_timeOfDay = mDomeShader->program()->uniformLocation( "timeOfDay" );
	mDomeShader_sunSpotPower = mDomeShader->program()->uniformLocation( "sunSpotPower" );

	mSkyDomeImage = QImage( skyDomePath );
	if( mSkyDomeImage.isNull() )
	{
		qFatal( "\"%s\" not found!", skyDomePath.toLocal8Bit().constData() );
	}

	mDomeMap =  mGLWidget->bindTexture( mSkyDomeImage, GL_TEXTURE_2D, GL_RGBA );
	if( mDomeMap >= 0 && mDomeShader->hasDiffuseMap() )
	{
		mGLWidget->glActiveTexture( GL_TEXTURE0+(mDomeShader->texUnit_diffuseMap()) );
		glBindTexture( GL_TEXTURE_2D, mDomeMap );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}

	mStarCubeShader = new Shader( glWidget, "cube" );

	glGenTextures( 1, &mStarCubeMap );
	glBindTexture( GL_TEXTURE_CUBE_MAP, mStarCubeMap );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	TexImage( glWidget, GL_TEXTURE_CUBE_MAP_POSITIVE_X, starMapPathPX );
	TexImage( glWidget, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, starMapPathNX );
	TexImage( glWidget, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, starMapPathPY );
	TexImage( glWidget, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, starMapPathNY );
	TexImage( glWidget, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, starMapPathPZ );
	TexImage( glWidget, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, starMapPathNZ );
}


Sky::~Sky()
{
	delete mDomeShader;
	delete mStarCubeShader;
	mCubeVertexBuffer.destroy();
	mCubeIndexBuffer.destroy();
}


void Sky::update( const float & delta )
{
	float angle = (*mTimeOfDay)*(M_PI*2.0f);
	QMatrix4x4 m;
	m.setToIdentity();
	m.rotate( angle*(180.0/M_PI), mAxis );
	mSunDirection = m.map( mSunInitialDir );
	
	float xA=(*mTimeOfDay)*(float)mSkyDomeImage.width()-0.5f;
	if( xA < 0.0f )
		xA = (float)mSkyDomeImage.width()+xA;
	QRgb colorA = mSkyDomeImage.pixel( xA, mSkyDomeImage.height()-1 );

	float xB = xA + 1.0f;
	if( xB >= (float)mSkyDomeImage.width() )
		xB -= (float)mSkyDomeImage.width();
	QRgb colorB = mSkyDomeImage.pixel( xB, mSkyDomeImage.height()-1 );

	QVector4D colorAF( qRed(colorA), qGreen(colorA), qBlue(colorA), qAlpha(colorA) );
	colorAF /= 255.0f;
	QVector4D colorBF( qRed(colorB), qGreen(colorB), qBlue(colorB), qAlpha(colorB) );
	colorBF /= 255.0f;

	QVector4D baseColor = colorAF + (colorBF-colorAF) * (xA-floorf(xA));
	mFogColor = baseColor;

	float diffuseFactor = interpolateLinear( mDiffuseFactorDay, mDiffuseFactorNight, (float)((sunDirection().y()+1.0f)*0.5f) );
	if( diffuseFactor > mDiffuseFactorMax )
		diffuseFactor = mDiffuseFactorMax;

	float specularFactor = interpolateLinear( mSpecularFactorDay, mSpecularFactorNight, (float)((sunDirection().y()+1.0f)*0.5f) );
	if( diffuseFactor > mSpecularFactorMax )
		diffuseFactor = mSpecularFactorMax;

	float ambientFactor = interpolateLinear( mAmbientFactorDay, mAmbientFactorNight, (float)((sunDirection().y()+1.0f)*0.5f) );
	if( ambientFactor > mAmbientFactorMax )
		ambientFactor = mAmbientFactorMax;

	mDiffuse = baseColor.toVector3D() * diffuseFactor;
	mSpecular = baseColor.toVector3D() * specularFactor;
	mAmbient = baseColor.toVector3D().normalized() * ambientFactor;
}


void Sky::draw( const QVector3D & eye )
{
	glDepthMask( GL_FALSE );
	glDisable( GL_CULL_FACE );

	glMatrixMode( GL_MODELVIEW );	glPushMatrix();
	glTranslatef( eye.x(), eye.y(), eye.z() );

	glPushMatrix();
	float angle = (*mTimeOfDay)*(360.0f);
	glRotatef( angle, mAxis.x(), mAxis.y(), mAxis.z() );
	mStarCubeShader->bind();
	mCubeVertexBuffer.bind();
	mCubeIndexBuffer.bind();
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_INDEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, 0 );
	glDrawElements( GL_QUADS, sizeof(cubeIndices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0 );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	mCubeVertexBuffer.release();
	mCubeIndexBuffer.release();
	mStarCubeShader->release();
	glPopMatrix();

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	mDomeShader->bind();
	mDomeShader->program()->setUniformValue( mDomeShader_sunDir, mSunDirection.toVector3D() );
	mDomeShader->program()->setUniformValue( mDomeShader_timeOfDay, *mTimeOfDay );
	mDomeShader->program()->setUniformValue( mDomeShader_sunSpotPower, mSunSpotPower );
	mGLWidget->glActiveTexture( GL_TEXTURE0+(mDomeShader->texUnit_diffuseMap()) );	glBindTexture( GL_TEXTURE_2D, mDomeMap );
	mCubeVertexBuffer.bind();
	mCubeIndexBuffer.bind();
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_INDEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, 0 );
	glDrawElements( GL_QUADS, sizeof(cubeIndices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0 );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	mCubeVertexBuffer.release();
	mCubeIndexBuffer.release();
	mDomeShader->release();
	glDisable( GL_BLEND );

	glMatrixMode( GL_MODELVIEW );	glPopMatrix();
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
}
