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

	static const int BufferSize = 4;
    static const GLsizeiptr PositionSize = 932 * 3 * sizeof( GLfloat );
    static const GLsizeiptr TexSize = 479 * 2 * sizeof( GLfloat );
    static const GLsizeiptr NormalSize = 932 * 3 * sizeof( GLfloat );

    static const int DetectionDistanceDay = 60;
    static const int DetectionDistanceNight = 15;
    static const int DetectionDistanceOfTorch = 80;

    static const float SplatterbugLength = 8.8f;
    static const float SplatterbugBoundingSphereSize = 18.0f;

	//Enums
		enum
	{
		POSITION_OBJECT = 0,
		COLOR_OBJECT = 1,
		TEXTURE_OBJECT = 2,
		NORMAL_OBJECT = 3
	};
	enum
	{
		TARGET_BODY = 0,
        TARGET_HEAD = 1,
        TARGET_WING_RIGHT = 2,
        TARGET_WING_LEFT = 3,
        TARGET_NOTHING = 4
    };
	enum
	{
		DEAD_NORMAL =0,
        DEAD_HEADSHOT= 1,
        DEAD_WINGSHOT = 2
	};

	// Public attributes
	GLuint BufferName[BufferSize];
	float mDamageOnBodyPart[2];
	float damageMultiplicationFactor[2];

	// De-/Constructor
    Splatterbug( World * world );
	~Splatterbug();


	//Public methods
	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	virtual const AObject * intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction,
		float & length, QVector3D * normal = NULL ) const;

	virtual void receiveDamage( int damage, const QVector3D * position = NULL, const QVector3D * direction = NULL );
protected:
	;

private:
	AudioSample *mBugSound;    
    AudioSample *mBugBiteSound;

	GLUquadric * mQuadric;
	Material * mMaterial;
	StaticModel * mModel;
	QVector3D mTarget;

	GLuint vboId;
	GLfloat PositionData[PositionSize/sizeof( GLfloat )];
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



};

#endif // SCENE_OBJECT_CREATURE_SPLATTERBUG_INCLUDED

