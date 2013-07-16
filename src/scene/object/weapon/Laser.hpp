#ifndef SCENE_OBJECT_WEAPON_LASER_INCLUDED
#define SCENE_OBJECT_WEAPON_LASER_INCLUDED


#include "AWeapon.hpp"


struct GLUquadric;
class Scene;
class World;


class Laser : public AWeapon
{
public:
	Laser( Scene * scene, World * world );
	~Laser();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void drawAfterSelf();
	
	virtual void triggerPressed();
	virtual void triggerReleased();

private:
	World * mWorld;
	GLUquadric * mQuadric;
	bool mFired;
	float mCoolDown;
	float mRange;
	float mTrailRadius;
	float mTrailLength;
	float mTrailAlpha;
	QVector3D mTrailStart;
	QVector3D mTrailDirection;
	QVector3D mTrailEnd;
};


#endif
