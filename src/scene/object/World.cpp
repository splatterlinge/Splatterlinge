#include "World.hpp"

#include <scene/Scene.hpp>
#include <geometry/ParticleSystem.hpp>
#include <geometry/SplatterSystem.hpp>
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
	
	mSplatterSystem = new SplatterSystem( scene->glWidget(), mLandscape->terrain() );
	mTarget = QVector3D(0,0,0);
}


World::~World()
{
	scene()->removeKeyListener( this );
	delete mSplatterSystem;
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
	mSplatterSystem->spray( mTarget+QVector3D(0,2,0), 20.0f );
}


void World::updateSelf( const double & delta )
{
	if( mTimeLapse )
		mTimeOfDay += 0.2f * delta;
	else
		mTimeOfDay += 0.01f * delta;
	if( mTimeOfDay > 1.0f )
		mTimeOfDay -= 1.0f;

	mSky->update( delta );

	float landscapeHeight = mLandscape->terrain()->getHeight( scene()->eye()->position() );
	if( scene()->eye()->position().y() < landscapeHeight + 2 )
		scene()->eye()->setPositionY( landscapeHeight + 2 );

	float length = 300.0f;
	if( getLineIntersection( scene()->eye()->position(), -scene()->eye()->direction(), &length ) )
		mTarget = scene()->eye()->position() - scene()->eye()->direction() * length;

	if( mDragTeapot )
	{
		mTeapot->setPosition( mTarget );
		mTeapot->moveY( 3 );
	}

	mSplatterSystem->update( delta );
}


void World::drawSelf()
{
	glLight( GL_LIGHT0, GL_POSITION, mSky->sunDirection() );
	glLight( GL_LIGHT0, GL_AMBIENT, mSky->ambient() );
	glLight( GL_LIGHT0, GL_DIFFUSE, mSky->diffuse() );
	glLight( GL_LIGHT0, GL_SPECULAR, mSky->specular() );
	glEnable( GL_LIGHT0 );

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
	glEnable( GL_LIGHT1 );

	glFog( GL_FOG_COLOR, mSky->fogColor() );
	glFog( GL_FOG_START, scene()->eye()->farPlane()*0.9f );
	glFog( GL_FOG_END, scene()->eye()->farPlane()*1.1f );
}


void World::drawSelfPost()
{
	mSplatterSystem->draw();
	glDisable( GL_LIGHTING );
	glBegin( GL_LINES );
		glColor3f( 0,1,1 );
		glVertex3f( mTarget.x(), mTarget.y()+1, mTarget.z() );
		glVertex3f( mTarget.x(), mTarget.y()-1, mTarget.z() );
		glColor3f( 1,1,0 );
		glVertex3f( mTarget.x()+1, mTarget.y(), mTarget.z() );
		glVertex3f( mTarget.x()-1, mTarget.y(), mTarget.z() );
		glColor3f( 1,0,1 );
		glVertex3f( mTarget.x(), mTarget.y(), mTarget.z()+1 );
		glVertex3f( mTarget.x(), mTarget.y(), mTarget.z()-1 );
		glColor3f( 1,0,0 );
		glVertex3f( mTarget.x(), mTarget.y(), mTarget.z() );
		glVertex3f( mTarget.x()+1, mTarget.y()+1, mTarget.z()+1 );
	glEnd();
}


QSharedPointer<AObject> World::getLineIntersection( const QVector3D & origin, const QVector3D & direction, float * length )
{
	QSharedPointer<AObject> target;

	if( mLandscape->terrain()->getLineIntersection( origin, direction, length ) )
		target = mLandscape;

	// Add checks to other objects here, if they are closer set target accordingly

	return target;
}
