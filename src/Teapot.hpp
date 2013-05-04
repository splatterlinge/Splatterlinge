#ifndef TEAPOT_INCLUDED
#define TEAPOT_INCLUDED


#include "Object3D.hpp"


class Material;


class Teapot : public Object3D
{
public:
	Teapot( GLScene * scene );
	virtual ~Teapot();
	
	virtual void updateSelf( const float & delta );
	virtual void drawSelf();

private:
	Material * mMaterial;
};


#endif
