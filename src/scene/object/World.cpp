#include "World.hpp"

#include "../Scene.hpp"
#include "Landscape.hpp"
#include "Teapot.hpp"
#include "WavefrontObject.hpp"
#include "Sky.hpp"

#include <QPainter>
#include <QSettings>


World::World( Scene * scene, QString name ) :
	AObject( scene )
{
	QSettings s( "./data/world/"+name+".ini", QSettings::IniFormat );
	s.beginGroup( "World" );
		QString skyName = s.value( "skyName", "earth" ).toString();
		QString landscapeName = s.value( "landscapeName", "earth" ).toString();
	s.endGroup();

	mTimeLapse = false;
	mTimeOfDay = 0.0f;

	mLandscape = QSharedPointer<Landscape>( new Landscape( scene, landscapeName ) );
	add( mLandscape );

	mTeapot = QSharedPointer<Teapot>( new Teapot( scene, 4 ) );
	mTeapot->setPositionY( mLandscape->terrain()->getHeight( QVector3D(0,0,0) ) + 3 );
	add( mTeapot );
	mDragTeapot = false;

	mTable = QSharedPointer<WavefrontObject>( new WavefrontObject( scene, "data/object/table01/table01.obj" ) );
	mTable->setPositionY( mLandscape->terrain()->getHeight( QVector3D(0,0,0) ) + 3 );
	add( mTable );

	mTree = QSharedPointer<WavefrontObject>( new WavefrontObject( scene, "data/object/tree/tree.obj" ) );
	mTree->setPositionX( 100 );
	mTree->setPositionY( mLandscape->terrain()->getHeight( QVector3D(100,0,0) ) - 1 );
	add( mTree );

	mSky = QSharedPointer<Sky>( new Sky( scene, skyName, &mTimeOfDay ) );
	add( mSky );

	scene->addKeyListener( this );
	scene->addMouseListener( this );
}


World::~World()
{
	scene()->removeKeyListener( this );
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
		float length = 300.0f;
		if( getLineIntersection( scene()->eye()->position(), -scene()->eye()->direction(), &length ) )
		{
			mTeapot->setPosition( scene()->eye()->position() - scene()->eye()->direction() * length );
			mTeapot->moveY( 3 );
		}
	}
}


void World::drawSelf()
{
	glLight( GL_LIGHT0, GL_POSITION, mSky->sunDirection() );
	glLight( GL_LIGHT0, GL_AMBIENT, mSky->ambient() );
	glLight( GL_LIGHT0, GL_DIFFUSE, mSky->diffuse() );
	glLight( GL_LIGHT0, GL_SPECULAR, mSky->specular() );

	static float cycle = 0.0f;
	QColor color = QColor::fromHsvF( cycle, 0.5f, 0.5f, 1.0f );
	cycle+=0.0005f;
	QVector4D colorVec( color.redF(), color.greenF(), color.blueF(), 1.0f );
	if( cycle>=1.0f )
		cycle-=1.0f;

	glLight( GL_LIGHT1, GL_POSITION, QVector4D(	mTeapot->position(),	1	) );
	glLight( GL_LIGHT1, GL_AMBIENT, QVector4D(	0,	0,	0,	1	) );
	glLight( GL_LIGHT1, GL_DIFFUSE, QVector4D(	colorVec	) );
	glLight( GL_LIGHT1, GL_SPECULAR, QVector4D(	colorVec	) );
	glLight( GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.0f );
	glLight( GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.02f );
	glLight( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f );

	glFog( GL_FOG_COLOR, mSky->fogColor() );
	glFog( GL_FOG_START, scene()->eye()->farPlane()*0.9f );
	glFog( GL_FOG_END, scene()->eye()->farPlane()*1.1f );
}


void World::drawSelfPost()
{
	glDisable( GL_LIGHTING );
	float length = 300.0f;
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
			glColor3f( 1,0,0 );
			glVertex3f( intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.z() );
			glVertex3f( intersectionPoint.x()+1, intersectionPoint.y()+1, intersectionPoint.z()+1 );
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
