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

	float getFov() const { return mFOV; }
	float getNear() const { return mNear; }
	float getFar() const { return mFar; }

	void setFOV( float _fov ) { mFOV = _fov; }
	void setNear( float _near ) { mNear = _near; }
	void setFar( float _far ) { mFar = _far; }

protected:
	void updateMatrix() const;

private:
	float mFOV;
	float mNear;
	float mFar;
};


#endif
