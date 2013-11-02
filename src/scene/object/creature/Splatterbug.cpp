/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */
#include "Splatterbug.hpp"
#include <scene/object/World.hpp>
#include <scene/TextureRenderer.hpp>
//#include <scene/Scene.hpp>
#include <resource/Material.hpp>
#include <effect/SplatterSystem.hpp>
#include <utility/RandomNumber.hpp>
#include <utility/Quaternion.hpp>
#include <utility/Sphere.hpp>
#include <utility/Capsule.hpp>
#include <math.h>
#include <float.h>

Splatterbug::Splatterbug( World * world, float damage ) : ACreature( world )
{
    mModel = new StaticModel( scene()->glWidget(), "Splatterbug");
    mMaterial = new Material( scene()->glWidget(), "Splatterbug_Splatterbug" );
    mBugSound = new AudioSample("bug_walk2");
    mBugBiteSound = new AudioSample("neck_snap");
    mBugBiteSound->setLooping( false );
    mBugSound->setLooping( true );
    mBugSound->setRolloffFactor( 0.01f );

    mBugSound->play();
    mBugBiteSound->setRolloffFactor( 0.01f );
    damageMultiplicationFactor[TARGET_BODY] = 2.0f;
    mDamageOnBodyPart[TARGET_BODY] = 0.0f;
    mBodyHitted = false;
    mVelocityY = 0.0f;
    mCoolDown = 0.0f;
    this->mAttackCoolDown = 5;
    this->mHitDamage = damage;
    mActDetectionDistance = Splatterbug::DetectionDistanceDay;
    setBoundingSphere( (Splatterbug::SplatterbugBoundingSphereSize*0.01f * this->mHitDamage) );
}

Splatterbug::~Splatterbug()
{
    delete mMaterial;
    delete mBugSound;
    delete mBugBiteSound;
}

void Splatterbug::drawSelf()
{
    glColor4f(1,1,1,1);
    glNormal3f(0,1,0);
    mMaterial->bind();
    glPushMatrix();
    glScalef(0.20 * (this->mHitDamage*0.22),0.20 * (this->mHitDamage*0.22),0.20 * (this->mHitDamage*0.22));
    //glScalef(0.2 ,0.2, 0.2);
    mModel->draw();
    glPopMatrix();
    mMaterial->release();
}

const AObject * Splatterbug::intersectLine( const AObject * exclude, const QVector3D & origin,
                                      const QVector3D & direction, float & length,
                                      QVector3D * normal) const
{
    const AObject * nearestTarget = AObject::intersectLine( exclude, origin, direction, length, normal );
    float rayLength;
    if( Sphere::intersectCulledRay( worldPosition(), boundingSphereRadius(), origin, direction, &rayLength ) )
    {
        if( rayLength < length )
        {
            // intersection closer than previous intersections?
            length = rayLength;
            if( normal )        // interested in normal?
                *normal = origin - worldPosition();
            nearestTarget = this;
        }
    }
    return nearestTarget;
}

void Splatterbug::receiveDamage( int damage, const QVector3D * position,
                                 const QVector3D * direction)
{
    int targetBodyPart = TARGET_BODY;
    targetBodyPart = TARGET_BODY;
    damage *= damageMultiplicationFactor[targetBodyPart];
    ACreature::receiveDamage( damage, direction );
    mDamageOnBodyPart[targetBodyPart] += damage;
    if( targetBodyPart == TARGET_BODY )
    {
        if( 5.0f <= mDamageOnBodyPart[TARGET_BODY] )
        {
            mBodyHitted = true;
            if(state() != DEAD){
                setLife( 0 );
                world()->player()->receivePoints( 50 );
                setState( DYING );
            }
        }
    }
    QVector3D splatterSource;
    if( position )
        splatterSource = *position;
    else
        splatterSource = worldPosition();
    if( state() != DEAD )
        world()->splatterSystem()->spray( splatterSource, damage );
    else
        world()->splatterSystem()->spray( splatterSource, damage / 2.0f );
}

static QVector3D randomPointOnWorld( World * world )
{
    QVector3D pos( RandomNumber::minMax(2,3), 0, RandomNumber::minMax(2,3) );
    pos.setY( world->landscape()->terrain()->getHeight( pos ) );
    return pos;
}

void Splatterbug::setRandomDestination()
{
    QVector2D twoDimPosXZ;
    QVector3D pos;
    bool destinationPointValid = false;
    do{
        pos = QVector3D( RandomNumber::minMax( this->position().x() - 100, this->position().x() + 100 ), 0, RandomNumber::minMax( this->position().z() - 100, this->position().z() + 100 ) );
        twoDimPosXZ = QVector2D(pos.x(), (pos.z()/(world()->landscape()->terrain()->size().z()/2.0f))*(world()->landscape()->terrain()->size().x()/2.0f) );
        float dist = (twoDimPosXZ - QVector2D(0,0)).length();

        if( dist < (world()->landscape()->terrain()->size().x()/2.0f) ){
            destinationPointValid = true;
        }
    }while (!destinationPointValid);

    pos.setY( this->world()->landscape()->terrain()->getHeight( pos ) + 0.5 );
    destinationPoint = pos;
}

bool Splatterbug::isPlayerDetected(float & distToPlayer ){
    QVector3D vectPlayerSplatter = world()->player()->worldPosition() - worldPosition();
    distToPlayer = ( vectPlayerSplatter ).length();
    vectPlayerSplatter = vectPlayerSplatter.normalized();

    double anglePlayerInSight = acos( QVector3D::dotProduct( vectPlayerSplatter, worldDirection() ) ) * 360 / M_PI;

    if( anglePlayerInSight < 100 && anglePlayerInSight > -100 && distToPlayer < 2.0f * mActDetectionDistance )
    {
        return true;
    }
    else if( distToPlayer < mActDetectionDistance )
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Splatterbug::isTorchDetected( float & distToTorch ){
    if(mNightActive){
        if(world()->player()->getTorch()->parent() != NULL){
            QVector3D vectTorchSplatter = world()->player()->getTorch().data()->worldPosition() - worldPosition();
            distToTorch = ( vectTorchSplatter ).length();
            vectTorchSplatter = vectTorchSplatter.normalized();
            double angleTorchInSight = acos( QVector3D::dotProduct( vectTorchSplatter, worldDirection() ) ) * 360 / M_PI;
            if( angleTorchInSight < 100 && angleTorchInSight > -100 && distToTorch < 2.0f * Splatterling::DetectionDistanceOfTorch )
            {
                return true;
            }
            else if( distToTorch < Splatterling::DetectionDistanceOfTorch )
            {
                return true;
            }
        }
    }
    distToTorch = 10000.0;
    return false;
}
void Splatterbug::updateSelf( const double & delta )
{
    switch( state() )
    {
        case SPAWNING:
        {
            setPosition( randomPointOnWorld( world() ));
            setState( ALIVE );
            setLife( 50 + this->mHitDamage);
            setRandomDestination();
            break;
        }
        case ALIVE:
        {
            QQuaternion worldRot = QQuaternion::fromAxisAndAngle(0,0,1,0);
            World * world2 = dynamic_cast<World*>(scene()->root());

            if( world2 )
            {
               worldRot = QQuaternion::nlerp( rotation(), world2->landscape()->terrain()->getNormalRotation(position()), 33.0*delta);
            }
            if(worldRot.x() < 0.0){
                worldRot.setX(worldRot.x() - 1);
            }
            else{
                worldRot.setX(worldRot.x() + 1);
            }

            mNightActive = world()->sky().data()->timeOfDay() > 0.6f && world()->sky().data()->timeOfDay() < 0.9f;
            if(mNightActive)
            {
                mActDetectionDistance = DetectionDistanceNight;
            }else{
                mActDetectionDistance = DetectionDistanceDay;
            }
            mBugSound->setPositionAutoVelocity( this->worldPosition(), delta );
            GLfloat dist;
            playerDetected = isPlayerDetected(dist);
            GLfloat distToTorch;
            mTorchDetected = isTorchDetected(distToTorch);
            if( dist < 3.5 )
            {
                //Player in front of player
                QVector2D playerPosFlat(world()->player()->worldPosition().x(), world()->player()->worldPosition().z());
                QVector2D splatterPosFlat(worldPosition().x(), worldPosition().z());
                dist = ( playerPosFlat - splatterPosFlat).length();
                if( dist < 3.0 )
                {
                    if( mCoolDown == 0.0f )
                    {
                        if(!mBugBiteSound->isPlaying()){
                            mBugBiteSound->play();
                        }
                        world()->player()->receiveDamage( this->mHitDamage );
                        mCoolDown = this->mAttackCoolDown;
                    }
                }

            }
            else if( playerDetected )
            {
                //Player near get him
                mTarget = world()->player()->worldPosition();
                QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
                directionToTarget.setY(0);
                directionToTarget.normalize();
                QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D( 0, 1, 0 ) );
                setRotation( targetRotation + worldRot * delta);

                //Geschwindigkeit des Bugs
                QVector3D newPos = position() + direction() * delta * 10.0;
                float worldPos =world()->landscape()->terrain()->getHeight(newPos) + 2.0* delta;

                newPos.setY(worldPos);
                setPosition( newPos );

            }
            else if( mTorchDetected )
            {
                //torch detected
                if(distToTorch > 8.0f){
                    mTarget = world()->player()->getTorch().data()->worldPosition();
                    mTarget += QVector3D( 0, 1, 0 );
                    QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
                    QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D( 0, 1, 0 ) );
                    setRotation( QQuaternion::slerp( rotation(), targetRotation, 5 * delta ) );


                    QVector3D newPos = position() + direction()*delta * 12.0;
                    if(world()->landscape()->terrain()->getHeight(newPos)+2.0 > worldPosition().y()){
                        newPos.setY(world()->landscape()->terrain()->getHeight(newPos)+2.0);
                    }
                    setPosition( newPos );
                }else{
                }
            }
            else
            {
                //player not in near, just move somehow
                dist = ( destinationPoint - worldPosition() ).length();
                if( dist > 5 )
                {
                    mTarget = destinationPoint;
                    QVector3D directionToTarget = ( mTarget - worldPosition() ).normalized();
                    directionToTarget.setY(0);
                    directionToTarget.normalize();
                    QQuaternion targetRotation = Quaternion::lookAt( directionToTarget, QVector3D( 0, 1, 0 ) );
                    setRotation( targetRotation + worldRot * delta);
                    setPosition( position() + direction()*delta * 10.0 );
                }
                else
                {
                    setRandomDestination();
                }
            }
            if( life() <= 0 )
            {
                world()->player()->receivePoints( 100 );
                setState( DYING );
            }
            break;
        }
        case DYING:
        {
            mBugSound->stop();
            mBugBiteSound->stop();
            mVelocityY += -30.0f * delta;       // apply some gravity
            setPositionY( position().y() + mVelocityY *delta );
            float landscapeHeight;
            if( world()->landscape()->terrain()->getHeight( worldPosition(), landscapeHeight ) )
            {
                if( landscapeHeight + mHeightAboveGround > worldPosition().y() )
                {
                    setPositionY( landscapeHeight + mHeightAboveGround);
                }
            }
                    world()->splatterSystem()->spray( worldPosition(), mVelocityY*(-8.0f) );
                    world()->player()->receivePoints( mVelocityY*(-8.0f) );
                    setState( DEAD );
            break;
        }
        case DEAD:
        setPositionY(position().y() - 1.1f * delta);
            break;
    }
    if( mCoolDown > delta )
        mCoolDown -= delta;
    else
        mCoolDown = 0.0f;
}
