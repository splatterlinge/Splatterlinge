#include "World.hpp"

#include "../Scene.hpp"
#include "../Sky.hpp"
#include "Landscape.hpp"
#include "Teapot.hpp"

#include <QPainter>
#include <GL/glu.h>


World::World( Scene * scene ) :
	AObject( scene )
{
	add( QSharedPointer<Teapot>( new Teapot( scene ) ) );
	mLandscape = QSharedPointer<Landscape>( new Landscape( scene, "test" ) );
	add( mLandscape );

	mTimeLapse = false;
	mTimeOfDay = 0.0f;
	mSky = new Sky( scene->glWidget(), "earth", &mTimeOfDay );

	scene->addKeyListener( this );
}


World::~World()
{
	scene()->removeKeyListener( this );
	delete mSky;
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
		mTimeOfDay += 0.0001f;
	if( mTimeOfDay > 1.0f )
		mTimeOfDay -= 1.0f;

	mSky->update( delta );

	float landscapeHeight = mLandscape->terrain()->getHeight( scene()->eye()->position() );
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

}
