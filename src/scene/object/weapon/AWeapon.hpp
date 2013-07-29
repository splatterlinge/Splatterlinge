#ifndef SCENE_OBJECT_WEAPON_AWEAPON_INCLUDED
#define SCENE_OBJECT_WEAPON_AWEAPON_INCLUDED


#include "../AWorldObject.hpp"


/// Abstract baseclass for a weapon
/**
 *
 */
class AWeapon : public AWorldObject
{
public:
	/// Creates a weapon for the given world
	AWeapon( World * world );
	/// Frees this object
	virtual ~AWeapon();

	virtual void triggerPressed() = 0;
	virtual void triggerReleased() = 0;
	virtual void holster() = 0;
	virtual void pull() = 0;
};


#endif
