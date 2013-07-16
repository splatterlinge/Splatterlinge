#ifndef SCENE_OBJECT_WEAPON_AWEAPON_INCLUDED
#define SCENE_OBJECT_WEAPON_AWEAPON_INCLUDED


#include "../AObject.hpp"


class Scene;


/// Abstract baseclass for a weapon
/**
 * 
 */
class AWeapon : public AObject
{
public:
	/// Creates a weapon for the given scene
	AWeapon( Scene * scene, float boundingSphereRadius=0.0f );
	/// Frees this object
	~AWeapon() = 0;

	virtual void triggerPressed() = 0;
	virtual void triggerReleased() = 0;

protected:

private:

};

#endif

