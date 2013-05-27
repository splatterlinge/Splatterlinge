#ifndef SCENE_OBJECT_EYE_INCLUDED
#define SCENE_OBJECT_EYE_INCLUDED

#include "AObject.hpp"

#include <GLWidget.hpp>


class Scene;


/// Special scene object representing the "eye" for observing a scene
/**
 * 
 */
class Eye : public AObject
{
public:
	Eye( Scene * scene );
	Eye( Eye & other );
	virtual ~Eye();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void drawSelfPost();

	float fov() const { return mFOV; }
	float nearPlane() const { return mNearPlane; }
	float farPlane() const { return mFarPlane; }

	void setFOV( float fov ) { mFOV = fov; }
	void setNearPlane( float nearPlane ) { mNearPlane = nearPlane; }
	void setFarPlane( float farPlane ) { mFarPlane = farPlane; }

	void setClippingPlane( int n, QVector4D plane = QVector4D(0,0,0,0) );
	void enableClippingPlanes();
	void disableClippingPlanes();

	bool isPointInFrustum( QVector3D point ) const ;
	bool isSphereInFrustum( QVector3D center, float radius ) const;

protected:
	void updateMatrix() const;

private:
	QMap<int,QVector4D> mClippingPlanes;
	float mFOV;
	float mNearPlane;
	float mFarPlane;
	QVector4D mFrustum[6];
	void prepareFrustum();
};


#endif
