#include "World.hpp"

#include "../Scene.hpp"
#include "Landscape.hpp"
#include "Teapot.hpp"
#include "geometry/Sky.hpp"

#include <QPainter>
#include <GL/glu.h>


World::World( Scene * scene ) :
	AObject( scene )
{
	mLandscape = QSharedPointer<Landscape>( new Landscape( scene, "test" ) );
	add( mLandscape );

	mTeapot = QSharedPointer<Teapot>( new Teapot( scene, 4 ) );
	mTeapot->setPositionY( mLandscape->terrain()->getHeight(QVector3D(0,0,0)) + 3 );
	add( mTeapot );
	mDragTeapot = false;

	mTimeLapse = false;
	mTimeOfDay = 0.0f;
	mSky = new Sky( scene->glWidget(), "earth", &mTimeOfDay );

	scene->addKeyListener( this );
	scene->addMouseListener( this );
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


void World::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{

}


void World::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	if( event->button() == Qt::RightButton )
	{
		mDragTeapot = true;
	}
}


void World::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	if( event->button() == Qt::RightButton )
	{
		mDragTeapot = false;
	}
}


void World::wheelEvent( QGraphicsSceneWheelEvent * event )
{

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
	
	if( mDragTeapot )
	{
		float length = 100.0f;
		if( getLineIntersection( scene()->eye()->position(), -scene()->eye()->direction(), &length ) )
		{
			mTeapot->setPosition( scene()->eye()->position() - scene()->eye()->direction() * length );
			mTeapot->moveY( 3 );
		}
	}
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
	glDisable( GL_LIGHTING );
	float length = 100.0f;
	if( getLineIntersection( scene()->eye()->position(), -scene()->eye()->direction(), &length ) )
	{
		QVector3D intersectionPoint = scene()->eye()->position() - scene()->eye()->direction() * length;
		glBegin( GL_LINES );
			glColor3f( 0,1,1 );
			glVertex3f( intersectionPoint.x(), intersectionPoint.y()+1, intersectionPoint.z() );
			glVertex3f( intersectionPoint.x(), intersectionPoint.y()-1, intersectionPoint.z() );
			glColor3f( 1,1,0 );
			glVertex3f( intersectionPoint.x()+1, intersectionPoint.y(), intersectionPoint.z() );
			glVertex3f( intersectionPoint.x()-1, intersectionPoint.y(), intersectionPoint.z() );
			glColor3f( 1,0,1 );
			glVertex3f( intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.z()+1 );
			glVertex3f( intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.z()-1 );
		glEnd();
	}
}


QSharedPointer<AObject> World::getLineIntersection( const QVector3D & origin, const QVector3D & direction, float * length )
{
	QSharedPointer<AObject> target;

	if( mLandscape->terrain()->getLineIntersection( origin, direction, length ) )
		target = mLandscape;

	// Add checks to other objects here, if they are closer set target accordingly

	return target;
}
