#include "Sky.hpp"

#include <QtOpenGL>
#include <GL/glu.h>
#include <math.h>

#include "GLWidget.hpp"


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


Sky::Sky( GLWidget * glWidget, const QString & skyMapPath, const float * timeOfDay = 0 ) :
	mGLWidget(glWidget),
	mTimeOfDay(timeOfDay)
{
	mSunBrightness = 10;
	setSunAxis( QVector3D(1,0,1) );

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
	
	mSkyDomeShader = new Shader( glWidget, "skydome" );
	mSkyDomeShader_sunDir = mSkyDomeShader->program()->uniformLocation( "sunDir" );
	mSkyDomeShader_timeOfDay = mSkyDomeShader->program()->uniformLocation( "timeOfDay" );
	mSkyDomeShader_sunBrightness = mSkyDomeShader->program()->uniformLocation( "sunBrightness" );

	mSkyDomeImage = QImage( skyMapPath );
	if( mSkyDomeImage.isNull() )
	{
		qWarning() << skyMapPath << "not found!";
	}

	mSkyDomeMap =  mGLWidget->bindTexture( mSkyDomeImage, GL_TEXTURE_2D, GL_RGBA );
	if( mSkyDomeMap >= 0 && mSkyDomeShader->hasDiffuseMap() )
	{
		glActiveTexture( GL_TEXTURE0+(mSkyDomeShader->texUnit_diffuseMap()) );	glBindTexture( GL_TEXTURE_2D, mSkyDomeMap );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}
}


void Sky::update( const float & delta )
{
	float angle = (*mTimeOfDay)*(M_PI*2.0f);
	QQuaternion q = QQuaternion::fromAxisAndAngle ( mSunAxis, angle*(180.0/M_PI) );
	mSunDirection = q.rotatedVector( QVector3D(0,0,-1) );

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

	float g = mSunDirection.y()*5.0f + 0.5f;
	if( g < 0.0f )
		g = 0.0f;
	if( g > 1.0f )
		g = 1.0f;
	mDiffuse = baseColor.toVector3D() * g;
	mSpecular = baseColor.toVector3D() * g;
	mAmbient = baseColor.toVector3D().normalized() * (-g);
}


void Sky::draw( const QVector3D & eye )
{
	glDepthMask( GL_FALSE );
	glDisable( GL_CULL_FACE );

	glMatrixMode( GL_MODELVIEW );	glPushMatrix();
	glTranslatef( eye.x(), eye.y(), eye.z() );

	mSkyDomeShader->bind();
	mSkyDomeShader->program()->setUniformValue( mSkyDomeShader_sunDir, mSunDirection.toVector3D() );
	mSkyDomeShader->program()->setUniformValue( mSkyDomeShader_timeOfDay, *mTimeOfDay );
	mSkyDomeShader->program()->setUniformValue( mSkyDomeShader_sunBrightness, mSunBrightness );
	glActiveTexture( GL_TEXTURE0+(mSkyDomeShader->texUnit_diffuseMap()) );	glBindTexture( GL_TEXTURE_2D, mSkyDomeMap );
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
	mSkyDomeShader->release();

	glMatrixMode( GL_MODELVIEW );	glPopMatrix();
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
}


Sky::~Sky()
{
	mCubeVertexBuffer.destroy();
	mCubeIndexBuffer.destroy();
}
