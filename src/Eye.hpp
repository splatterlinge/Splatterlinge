#ifndef EYE_INCLUDED
#define EYE_INCLUDED


#include "Object3D.hpp"


class Eye : public Object3D
{
public:
	Eye( GLScene * scene, const Object3D * parent = 0 );
	virtual ~Eye();

	virtual void updateSelf( const float & delta );
	virtual void drawSelf();

	float fov() const { return mFOV; }
	float near() const { return mNear; }
	float far() const { return mFar; }

	void setFOV( float fov ) { mFOV = fov; }
	void setNear( float near ) { mNear = near; }
	void setFar( float far ) { mFar = far; }

protected:
	void updateMatrix() const;

private:
	float mFOV;
	float mNear;
	float mFar;
};


#endif
