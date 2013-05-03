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
	float nearPlane() const { return mNearPlane; }
	float farPlane() const { return mFarPlane; }

	void setFOV( float fov ) { mFOV = fov; }
	void setNearPlane( float nearPlane ) { mNearPlane = nearPlane; }
	void setFarPlane( float farPlane ) { mFarPlane = farPlane; }

protected:
	void updateMatrix() const;

private:
	float mFOV;
	float mNearPlane;
	float mFarPlane;
};


#endif
