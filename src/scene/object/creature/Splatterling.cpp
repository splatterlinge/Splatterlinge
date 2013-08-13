#include "Splatterling.hpp"

#include <scene/object/World.hpp>
#include <resource/Material.hpp>
#include <effect/SplatterSystem.hpp>
#include <utility/RandomNumber.hpp>
#include <utility/Quaternion.hpp>
#include <utility/Sphere.hpp>

#include <math.h>
#include <float.h>
#include <QDebug>




static const GLsizeiptr PositionSize = 12 * 3 * sizeof(GLfloat);
static GLfloat PositionData[] =
{
    0.0f,0.0f,0.0f,
    8.0f,1.0f,-1.0f,
    8.0f,1.0f,1.0f,
    8.0f,-1.0f,1.0f,
    8.0f,-1.0f,-1.0f,
    8.0f,1.0f,-1.0f,
    //fluegel

    4.0f,0.5f,0.0f,
    0.0f,4.0f,8.0f,
    8.0f,4.0f,8.0f,

    4.0f,0.5f,0.0f,
    0.0f,4.0f,-8.0f,
    8.0f,4.0f,-8.0f,
};

static const GLsizeiptr ColorSize = 12 * 3 * sizeof(GLubyte);
static const GLubyte ColorData[] =
{
    255,255,0,
    255,255,0,
    255,255,0,
    255,255,0,
    255,255,0,

    0,0,255,
    0,0,255,
    0,0,255,
    0,0,255,
    0,0,255,
    0,0,255,
};
static const int BufferSize = 2;
static GLuint BufferName[BufferSize];

static const GLsizei BodyVertexCount = 6;
static const int WingOneYPos = (BodyVertexCount*3)+4;
static const int WingTwoYPos = (BodyVertexCount*3)+(3*3)+4;

enum
{
    POSITION_OBJECT = 0,
    COLOR_OBJECT = 1
};





Splatterling::Splatterling( World * world ) : ACreature( world )
{
	mQuadric = gluNewQuadric();
	gluQuadricTexture( mQuadric, GL_TRUE );

	mHeightAboveGround = 6.0f;
	mVelocityY = 0.0f;
    mMaterial = new Material( scene()->glWidget(), "KirksEntry" );
    glGenBuffers(2, BufferName);
    wingUpMovement = false;
}


Splatterling::~Splatterling()
{
	gluDeleteQuadric( mQuadric );
    delete mMaterial;
}


static QVector3D randomPointOnWorld( World * world )
{
	QVector3D pos( RandomNumber::minMax(-100,100), 0, RandomNumber::minMax(-100,100) );
	pos.setY( world->landscape()->terrain()->getHeight( pos ) );
	return pos;
}


void Splatterling::updateSelf( const double & delta )
{
	switch( state() )
	{
		case SPAWNING:
		{
			setPosition( randomPointOnWorld( world() ) + QVector3D(0,10,0) );
			setState( ALIVE );
			setLife( 100 );
			mHeightAboveGround = 6.0f;
			break;
		}

		case ALIVE:
		{
/*			mTarget = world()->teapot()->worldPosition();
			QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
			QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D(0,1,0) );
			setRotation( QQuaternion::slerp( rotation(), targetRotation, 0.05 ) );
            setPosition( position() + direction()*delta*10.0 );
*/
            recalculateWingPosition();
			if( life() <= 0 )
				setState( DYING );
			break;
		}

		case DYING:
			setState( DEAD );
			mHeightAboveGround = 3.0f;
			break;

		case DEAD:
			break;
	}

/*
	mVelocityY += -3.0f * delta;	// apply some gravity
	setPositionY( position().y() + mVelocityY * delta );

	float landscapeHeight;
	if( world()->landscape()->terrain()->getHeight( position(), landscapeHeight ) )
	{
		if( landscapeHeight + mHeightAboveGround > position().y() )
		{
			setPositionY( landscapeHeight + mHeightAboveGround );
			if( mVelocityY < 0.0f )
				mVelocityY = 0.0f;
		}
    }*/
}


void Splatterling::drawSelf()
{
    glDisable(GL_LIGHTING);
    glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_OBJECT]);
    glBufferData(GL_ARRAY_BUFFER, ColorSize, ColorData, GL_STREAM_DRAW);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, BufferName[POSITION_OBJECT]);
    glBufferData(GL_ARRAY_BUFFER, PositionSize, PositionData, GL_STREAM_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawArrays(GL_TRIANGLE_FAN, 0, BodyVertexCount);

    glDrawArrays(GL_TRIANGLES, 6, 3);
    glDrawArrays(GL_TRIANGLES, 9, 3);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glEnable(GL_LIGHTING);
}


AObject * Splatterling::intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction, float & length, QVector3D * normal )
{
	AObject * nearestTarget = AObject::intersectLine( exclude, origin, direction, length, normal );

	float rayLength;
	if( Sphere::intersectRay( worldPosition(), 4, origin, direction, &rayLength ) )
	{
		if( rayLength < length )
		{	// intersection closer than previous intersections?
			length = rayLength;
			if( normal )	// interested in normal?
				*normal = origin - worldPosition();
			nearestTarget = this;
		}
	}

	return nearestTarget;
}


void Splatterling::receiveDamage( int damage, const QVector3D * position, const QVector3D * direction )
{
	ACreature::receiveDamage( damage, direction );
	QVector3D splatterSource;
	if( position )
		splatterSource = *position;
	else
		splatterSource = worldPosition();

	if( state() != DEAD )
		world()->splatterSystem()->spray( splatterSource, damage );
	else
		world()->splatterSystem()->spray( splatterSource, damage/2.0f );
}

void Splatterling::recalculateWingPosition(){
    if(wingUpMovement){
        PositionData[WingOneYPos]   += 0.02f;
        PositionData[WingOneYPos+3] += 0.02f;
        PositionData[WingTwoYPos]   += 0.02f;
        PositionData[WingTwoYPos+3] += 0.02f;
        if(PositionData[WingOneYPos] >= 4.0f){
            wingUpMovement = false;
        }
    }else{
        PositionData[WingOneYPos]   -= 0.02f;
        PositionData[WingOneYPos+3] -= 0.02f;
        PositionData[WingTwoYPos]   -= 0.02f;
        PositionData[WingTwoYPos+3] -= 0.02f;
        if(PositionData[WingOneYPos] <= 0.0f){
            wingUpMovement = true;
        }
    }
}
