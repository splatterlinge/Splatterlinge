#ifndef EYE_INCLUDED
#define EYE_INCLUDED

#include "AObject.hpp"

#include <GLWidget.hpp>


class Scene;


class Eye : public AObject
{
public:
	Eye( Scene * scene );
	Eye( Eye & other );
	virtual ~Eye();

	virtual void updateSelf( const float & delta );
	virtual void drawSelf();
	virtual void drawSelfPost();

	float fov() const { return mFOV; }
	float nearPlane() const { return mNearPlane; }
	float farPlane() const { return mFarPlane; }

	void setFOV( float fov ) { mFOV = fov; }
	void setNearPlane( float nearPlane ) { mNearPlane = nearPlane; }
	void setFarPlane( float farPlane ) { mFarPlane = farPlane; }

	void setClippingPlane( int n, QVector4D plane = QVector4D(0,0,0,0) );

protected:
	void updateMatrix() const;

private:
	QMap<int,QVector4D> mClippingPlanes;
	float mFOV;
	float mNearPlane;
	float mFarPlane;
};


#endif
