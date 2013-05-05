#ifndef TEAPOT_INCLUDED
#define TEAPOT_INCLUDED

#include "AObject.hpp"


class Material;


class Teapot : public AObject
{
public:
	Teapot( Scene * scene );
	virtual ~Teapot();
	
	virtual void updateSelf( const float & delta );
	virtual void drawSelf();

private:
	Material * mMaterial;
};


#endif
