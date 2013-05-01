#ifndef EYE_INCLUDED
#define EYE_INCLUDED


#include "Object3D.hpp"


class Eye : public Object3D
{
public:
	Eye( GLScene * scene, const Object3D * parent = 0 ) : Object3D(scene, parent) {}
	virtual ~Eye() {}

	virtual void updateSelf( const float & delta ) {}
	virtual void drawSelf() {}

protected:
	void updateMatrix() const;
};


#endif
