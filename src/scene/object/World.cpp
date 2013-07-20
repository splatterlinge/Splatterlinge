#include "World.hpp"

#include <scene/Scene.hpp>
#include <utility/random.hpp>
#include <geometry/ParticleSystem.hpp>
#include <geometry/SplatterSystem.hpp>
#include "Landscape.hpp"
#include "Teapot.hpp"
#include "WavefrontObject.hpp"
#include "Sky.hpp"
#include "Player.hpp"

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
	mTimeReverse = false;
	mTimeOfDay = 0.0f;

	mLandscape = QSharedPointer<Landscape>( new Landscape( scene, landscapeName ) );
	add( mLandscape );

	mPlayer = QSharedPointer<Player>( new Player( scene, this ) );
	add( mPlayer );
	scene->eye()->attach( mPlayer );

	mTable = QSharedPointer<WavefrontObject>( new WavefrontObject( scene, "data/object/table01/table01.obj", 0.4f ) );
	mTable->setPositionY( mLandscape->terrain()->getHeight( QPointF(0,0) ) + 3 );
	add( mTable );

	for(int i=0; i<=40; i+=10)
	{
		mTree = QSharedPointer<WavefrontObject>( new WavefrontObject( scene, "data/object/tree/tree.obj", 0.4f ) );
		mTree->setPositionX( 100 );
		mTree->setPositionZ( -i );
		mTree->setPositionY( mLandscape->terrain()->getHeight( QPointF(100,-i) ) - 1 );
		add( mTree );
	}

	mSky = QSharedPointer<Sky>( new Sky( scene, skyName ) );
	add( mSky );

	scene->addKeyListener( this );

	mTarget = QVector3D(0,0,0);

	mSplatterSystem = new SplatterSystem( scene->glWidget(), mLandscape->terrain() );
	mSplatterInteractor = new SplatterInteractor( *this );
	mSplatterSystem->particleSystem()->setInteractionCallback( mSplatterInteractor );
}


World::~World()
{
	mLightSources.clear();
	scene()->removeKeyListener( this );
	delete mSplatterSystem;
	delete mSplatterInteractor;
}


void World::addLightSource( ALightSource * lightSource )
{
	if( !mLightSources.contains( lightSource ) )
		mLightSources.append( lightSource );
}


void World::removeLightSource( ALightSource * lightSource )
{
	mLightSources.removeOne( lightSource );
}


void World::keyPressEvent( QKeyEvent * event )
{
	switch( event->key() )
	{
	case Qt::Key_PageDown:
		mTimeLapse = true;
		break;
	case Qt::Key_PageUp:
		mTimeReverse = true;
		break;
	default:
		break;
	}
}


void World::keyReleaseEvent( QKeyEvent * event )
{
	switch( event->key() )
	{
	case Qt::Key_PageDown:
		mTimeLapse = false;
		break;
	case Qt::Key_PageUp:
		mTimeReverse = false;
		break;
	default:
		break;
	}
}


void World::updateSelf( const double & delta )
{
	mTimeOfDay += 0.01f * delta;

	if( mTimeLapse )
		mTimeOfDay += 0.2f * delta;
	if( mTimeReverse )
		mTimeOfDay -= 0.2f * delta;

	if( mTimeOfDay > 1.0f )
		mTimeOfDay -= 1.0f;

	mSky->setTimeOfDay( mTimeOfDay );

	mSplatterSystem->update( delta );
}


void World::updateSelfPost( const double & delta )
{

}


void World::drawSelf()
{
	glLight( GL_LIGHT0, GL_POSITION, mSky->sunDirection() );
	glLight( GL_LIGHT0, GL_AMBIENT, mSky->ambient() );
	glLight( GL_LIGHT0, GL_DIFFUSE, mSky->diffuse() );
	glLight( GL_LIGHT0, GL_SPECULAR, mSky->specular() );
	glEnable( GL_LIGHT0 );

	int light = 1;
	QList< ALightSource * >::iterator i;
	for( i = mLightSources.begin(); i != mLightSources.end(); ++i )
	{
		(*i)->updateLightSource( GL_LIGHT0+light );
		light++;
	}

	glFog( GL_FOG_COLOR, mSky->baseColor() );
	glFog( GL_FOG_START, scene()->eye()->farPlane()*0.9f );
	glFog( GL_FOG_END, scene()->eye()->farPlane()*1.1f );
}


void World::drawSelfPost()
{
	mSplatterSystem->draw();
}


QSharedPointer<AObject> World::getLineIntersection( const QVector3D & origin, const QVector3D & direction, float & length, QVector3D & normal )
{
	QSharedPointer<AObject> target;

	if( mLandscape->terrain()->getLineIntersection( origin, direction, length, normal ) )
		target = mLandscape;

	// Add checks to other objects here, if they are closer set target accordingly

	return target;
}


void World::SplatterInteractor::particleInteraction( const double & delta, ParticleSystem::Particle & particle )
{
	bool belowWater = false;
	bool belowGround = false;

	if( particle.position().y() - mWorld.landscape()->waterHeight() < -mWorld.splatterSystem()->particleSystem()->size()/2.0f )
		belowWater = true;
	if( mWorld.landscape()->terrain()->getHeightAboveGround( particle.position() ) < -mWorld.splatterSystem()->particleSystem()->size()/2.0f )
		belowGround = true;

	if( belowWater )
	{
		particle.rVelocity() -= (mWorld.splatterSystem()->particleSystem()->gravity()/1.1) * delta;
	}

	if( belowGround )
	{
		particle.setLife( 0.0f );
		if( !belowWater )
			mWorld.splatterSystem()->splat( particle.position(), mWorld.splatterSystem()->particleSystem()->size() * randomMinMax( 0.5f, 2.0f ) );
	}
}
