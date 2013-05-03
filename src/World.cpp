#include "World.hpp"

#include <QPainter>
#include <GL/glu.h>

#include "GLScene.hpp"
#include "Landscape.hpp"
#include "Sky.hpp"
#include "Teapot.hpp"
#include "TextureRenderer.hpp"


World::World( GLScene * scene, const Object3D * parent ) :
	Object3D( scene, parent )
{
	addNode( QSharedPointer<Object3D>( new Teapot(scene,this) ) );

	mTimeLapse = false;
	mTimeOfDay = 0.0f;
	mSky = new Sky( scene->glWidget(), "earth", &mTimeOfDay );
	mLandscape = new Landscape( scene->glWidget(), "test" );

//	mWaterShader = new Shader( scene->glWidget(), "water" );
//	mTexRenderer = new TextureRenderer( scene->glWidget(), QSize(256,256), true );
	
	scene->addKeyListener( this );
}


World::~World()
{
	scene()->removeKeyListener( this );
	delete mSky;
	delete mLandscape;
}


void World::keyPressEvent( QKeyEvent * event )
{
	switch( event->key() )
	{
	case Qt::Key_Shift:
		mTimeLapse = true;
		break;
	default:
		break;
	}
}


void World::keyReleaseEvent( QKeyEvent * event )
{
	switch( event->key() )
	{
	case Qt::Key_Shift:
		mTimeLapse = false;
		break;
	default:
		break;
	}
}


void World::updateSelf( const float & delta )
{
	if( mTimeLapse )
		mTimeOfDay += 0.002f;
	else
		mTimeOfDay += 0.0002f;
	if( mTimeOfDay > 1.0f )
		mTimeOfDay -= 1.0f;
	mSky->update( delta );

	float landscapeHeight = mLandscape->getTerrain()->getHeight( scene()->eye()->position() );
	if( scene()->eye()->position().y() < landscapeHeight + 2 )
		scene()->eye()->setPositionY( landscapeHeight + 2 );
}


void World::drawSelf()
{
	glLightfv( GL_LIGHT0, GL_POSITION, reinterpret_cast<const GLfloat*>(&mSky->sunDirection()) );
	glLightfv( GL_LIGHT0, GL_AMBIENT, reinterpret_cast<const GLfloat*>(&mSky->ambient()) );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, reinterpret_cast<const GLfloat*>(&mSky->diffuse()) );
	glLightfv( GL_LIGHT0, GL_SPECULAR, reinterpret_cast<const GLfloat*>(&mSky->specular()) );
	glFogfv( GL_FOG_COLOR, reinterpret_cast<const GLfloat*>(&mSky->fogColor()) );
	glFogf( GL_FOG_START, scene()->eye()->farPlane()*0.66f );
	glFogf( GL_FOG_END, scene()->eye()->farPlane()*1.1 );
	mSky->draw( scene()->eye()->position() );
}


void World::drawSelfPost()
{
	mLandscape->drawPatch( QRectF( scene()->eye()->position().x()-scene()->eye()->farPlane(), scene()->eye()->position().z()-scene()->eye()->farPlane(), scene()->eye()->farPlane()*2, scene()->eye()->farPlane()*2 ) );
/*
	glPushAttrib( GL_VIEWPORT_BIT );
	mTexRenderer->bind();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport( 0, 0, mTexRenderer->size().width(), mTexRenderer->size().width() );
	mSky->draw( scene()->eye()->position() );
	mLandscape->drawPatch( QRectF( scene()->eye()->position().x()-scene()->eye()->farPlane(), scene()->eye()->position().z()-scene()->eye()->farPlane(), scene()->eye()->farPlane()*2, scene()->eye()->farPlane()*2 ) );
	mTexRenderer->release();
	glPopAttrib();

	mWaterShader->bind();
	mWaterShader->program()->setUniformValue( "refractionMap", 0 );
	glActiveTexture( GL_TEXTURE0 );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, mTexRenderer->texID() );
	glBegin( GL_QUADS );
		glVertex3f(-100, 5, 100 );
		glVertex3f( 100, 5, 100 );
		glVertex3f( 100, 5,-100 );
		glVertex3f(-100, 5,-100 );
	glEnd();
	mWaterShader->release();
*/
}
