#ifndef SCENE_OBJECT_WEAPON_MINIGUN_INCLUDED
#define SCENE_OBJECT_WEAPON_MINIGUN_INCLUDED


#include "AWeapon.hpp"
#include "resource/AudioSample.hpp"

class Material;
struct GLUquadric;


/// Caution - Laser
class Minigun : public AWeapon
{
public:
	Minigun( World * world );
	~Minigun();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void draw2Self();

	virtual void triggerPressed();
	virtual void triggerReleased();

private:
	GLUquadric * mQuadric;
	bool mFired;
	float mCoolDown;
	float mRange;
	float mTrailRadius;
	float mTrailLength;
	float mTrailAlpha;
	float mDamage;
	float mRotation;
	QVector3D mTrailStart;
	QVector3D mTrailDirection;
	QVector3D mTrailEnd;
	Material * mMaterial;
	AudioSample * mFireSound;
};


#endif
