#include "Knife.hpp"


#include <resource/StaticModel.hpp>


Knife::Knife( World * world ) :
	AWeapon( world, -1, -1, -1 )
{
	mModel = new StaticModel( world->scene()->glWidget(), "knife" );
	setPositionY(20);
}


Knife::~Knife()
{
	delete mModel;
}

void Knife::triggerPressed()
{
}


void Knife::triggerReleased()
{
}


void Knife::holster()
{
}


void Knife::pull()
{
}


bool Knife::isPulled()
{
	return true;
}


void Knife::updateSelf( const double &delta )
{
}


void Knife::draw2Self()
{
	glPushMatrix();

	mModel->draw();

	glPopMatrix();
}
