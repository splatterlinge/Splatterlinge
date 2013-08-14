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




Splatterling::Splatterling( World * world ) : ACreature( world )
{
	mQuadric = gluNewQuadric();
	gluQuadricTexture( mQuadric, GL_TRUE );

	mHeightAboveGround = 6.0f;
	mVelocityY = 0.0f;
    mMaterial = new Material( scene()->glWidget(), "KirksEntry" );
    glGenBuffers(2, this->BufferName);

    wingUpMovement = false;

    for(unsigned int i = 0; i < PositionSize/sizeof(GLfloat); i++){
        PositionData[i] = GlobalPositionData[i];
    }
}


Splatterling::~Splatterling()
{
	gluDeleteQuadric( mQuadric );
    delete mMaterial;
}


static QVector3D randomPointOnWorld( World * world )
{
    QVector3D pos( RandomNumber::minMax(-500,500), 0, RandomNumber::minMax(-500,500) );
	pos.setY( world->landscape()->terrain()->getHeight( pos ) );
	return pos;
}

void Splatterling::randomDestinationPoint(){
    QVector3D pos( RandomNumber::minMax(this->position().x()-100,this->position().x()+100), 0, RandomNumber::minMax(this->position().z()-100,this->position().z()+100) );
    pos.setY( this->world()->landscape()->terrain()->getHeight( pos ) + 20);
    destinationPoint = pos;
}


void Splatterling::updateSelf( const double & delta )
{
	switch( state() )
	{
		case SPAWNING:
		{
            setPosition( randomPointOnWorld( world() ) + QVector3D(0,20,0) );
			setState( ALIVE );
            setLife( 100 );
			mHeightAboveGround = 6.0f;

            randomDestinationPoint();

			break;
		}

		case ALIVE:
		{
            GLfloat dist = (world()->player()->worldPosition() - worldPosition()).length();

            if(dist < 12){
                //Player in front of player
                //Player near get him
                mTarget = world()->player()->worldPosition();
                QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
                QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D(0,1,0) );
                setRotation( QQuaternion::slerp( rotation(), targetRotation, 0.05 ) );
                world()->player()->setLife(world()->player()->life()-1);
                qDebug() << world()->player()->life();
            }else if(dist < 200){
                //Player near get him
                mTarget = world()->player()->worldPosition();
                QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
                QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D(0,1,0) );
                setRotation( QQuaternion::slerp( rotation(), targetRotation, 0.05 ) );
                setPosition( position() + direction()*delta*25.0 );
            } else{
                //player not in near, just move somehow

                dist = (destinationPoint - worldPosition()).length();
                if(dist > 5){
                    mTarget = destinationPoint;
                    QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
                    QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D(0,1,0) );
                    setRotation( QQuaternion::slerp( rotation(), targetRotation, 0.05 ) );
                    setPosition( position() + direction()*delta*10.0 );
                }else{
                    randomDestinationPoint();
                }
            }

            recalculateWingPosition();
			if( life() <= 0 )
				setState( DYING );
			break;
		}

        case DYING:
			mHeightAboveGround = 3.0f;

            mVelocityY += -6.0f * delta;	// apply some gravity
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
            }

            if(!(position().y() > 0.0)){
                setState( DEAD );
            }
			break;

		case DEAD:
			break;
	}
}


void Splatterling::drawSelf()
{
    glDisable(GL_LIGHTING);
    glBindBuffer(GL_ARRAY_BUFFER, this->BufferName[COLOR_OBJECT]);
    glBufferData(GL_ARRAY_BUFFER, Splatterling::ColorSize, ColorData, GL_STREAM_DRAW);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, this->BufferName[POSITION_OBJECT]);
    glBufferData(GL_ARRAY_BUFFER, Splatterling::PositionSize, PositionData, GL_STREAM_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glDrawArrays(GL_TRIANGLE_FAN, 0, Splatterling::BodyVertexCount);
    glDrawArrays(GL_TRIANGLE_STRIP, 6, Splatterling::HeadVertexCount);

    glDrawArrays(GL_TRIANGLES, Splatterling::BodyVertexCount+Splatterling::HeadVertexCount, 3);
    glDrawArrays(GL_TRIANGLES, Splatterling::BodyVertexCount+Splatterling::HeadVertexCount+3, 3);

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
        PositionData[Splatterling::WingOneYPos]   += 0.02f;
        PositionData[Splatterling::WingOneYPos+3] += 0.02f;
        PositionData[Splatterling::WingTwoYPos]   += 0.02f;
        PositionData[Splatterling::WingTwoYPos+3] += 0.02f;
        if(PositionData[Splatterling::WingOneYPos] >= 4.0f){
            wingUpMovement = false;
        }
    }else{
        PositionData[Splatterling::WingOneYPos]   -= 0.02f;
        PositionData[Splatterling::WingOneYPos+3] -= 0.02f;
        PositionData[Splatterling::WingTwoYPos]   -= 0.02f;
        PositionData[Splatterling::WingTwoYPos+3] -= 0.02f;
        if(PositionData[Splatterling::WingOneYPos] <= 0.0f){
            wingUpMovement = true;
        }
    }
}
