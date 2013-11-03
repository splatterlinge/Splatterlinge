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

#ifndef SCENE_OBJECT_CREATURE_SPLATTERBUG_INCLUDED
#define SCENE_OBJECT_CREATURE_SPLATTERBUG_INCLUDED

#include "ACreature.hpp"
#include "resource/AudioSample.hpp"
#include <GLWidget.hpp>
#include <QGLBuffer>

#include <resource/Material.hpp>
#include <effect/SplatterSystem.hpp>
#include <utility/RandomNumber.hpp>
#include <utility/Intersection.hpp>
#include <utility/Quaternion.hpp>
#include <utility/Sphere.hpp>

#include <math.h>
#include <float.h>
#include <QDebug>

struct GLUquadric;
class Material;

class StaticModel;

class Splatterbug: public ACreature
{
public:

    static const int DetectionDistanceDay = 50;
	static const int DetectionDistanceNight = 15;
	static const int DetectionDistanceOfTorch = 80;

    static const float SplatterbugBoundingSphereSize = 25.0f;

	//Enums
    enum
	{
        TARGET_BODY = 0,
        eNumOfTagets
    }eTarget;

	// Public attributes
    float mDamageOnBodyPart[eNumOfTagets];
    float damageMultiplicationFactor[eNumOfTagets];

	// De-/Constructor
    Splatterbug( World * world, float damage );
	~Splatterbug();


	//Public methods
	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual const AObject * intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction,
		float & length, QVector3D * normal = NULL ) const;

    virtual void receiveDamage( int damage, const QVector3D * position = NULL, const QVector3D * direction = NULL );
protected:

private:
	AudioSample *mBugSound;    
    AudioSample *mBugBiteSound;

	Material * mMaterial;
	StaticModel * mModel;
	QVector3D mTarget;

	GLuint vboId;
	QVector3D destinationPoint;

	float mVelocityY;
	float mHeightAboveGround;
	float mSplatterlingSizeFactor;
	float mCoolDown;
	float mAttackCoolDown;

	int mHitDamage;
	int mActDetectionDistance;

	bool playerDetected;
	bool mNightActive;
	bool mTorchDetected;
	bool mHeadDisintegrated;
	bool mBodyHitted;

	void setRandomDestination(void);
	bool isPlayerDetected(float & distToPlayer );
	bool isTorchDetected( float & distToTorch );
    void setNewPosition(const QVector3D arg_Vec, const double & arg_delta, const QQuaternion & arg_world_rot);



};

#endif // SCENE_OBJECT_CREATURE_SPLATTERBUG_INCLUDED

