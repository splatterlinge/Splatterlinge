#include "AWeapon.hpp"

#include <scene/Scene.hpp>
#include <GLWidget.hpp>


AWeapon::AWeapon( Scene * scene, float boundingSphereRadius ) :
	AObject::AObject( scene, boundingSphereRadius )
{
}


AWeapon::~AWeapon()
{
}
