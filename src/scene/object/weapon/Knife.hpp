#ifndef SCENE_OBJECT_WEAPON_KNIFE_HPP
#define SCENE_OBJECT_WEAPON_KNIFE_HPP

#include "AWeapon.hpp"


class Material;
class StaticModel;


class Knife : public AWeapon
{
public:
	Knife( World * world );
	~Knife();

	virtual void updateSelf( const double & delta );
	virtual void draw2Self();

	virtual void triggerPressed();
	virtual void triggerReleased();
	virtual void holster();
	virtual void pull();
	virtual bool isPulled();

private:
	StaticModel * mModel;
};

#endif // SCENE_OBJECT_WEAPON_KNIFE_HPP
