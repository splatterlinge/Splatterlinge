#ifndef SCENE_OBJECT_EYE_INCLUDED
#define SCENE_OBJECT_EYE_INCLUDED

#include "AObject.hpp"

#include <GLWidget.hpp>


class Scene;


/// Special scene object representing the "eye" for observing a scene
/**
 * 
 */
class Eye
{
public:
	Eye( Scene * scene );
	Eye( Eye & other );
	~Eye();

	/// Sets OpenAL listener position and orientation.
	void applyAL();
	/// Sets OpenGL projection and modelview matrices and applies clipping planes.
	void applyGL();

	/// Attach this eye to an object.
	void attach( QWeakPointer<AObject> object );
	/// Detaches this eye from an object.
	void detach();

	/// Field of view in degrees.
	float fov() const { return mFOV; }
	/// Set field of view in degrees.
	void setFOV( float fov ) { mFOV = fov; }

	/// Near plane of projection matrix.
	float nearPlane() const { return mNearPlane; }
	/// Set near plane of projection matrix.
	void setNearPlane( float nearPlane ) { mNearPlane = nearPlane; }
	/// Far plane of projection matrix.
	float farPlane() const { return mFarPlane; }
	/// Set far plane of projection matrix.
	void setFarPlane( float farPlane ) { mFarPlane = farPlane; }

	/// Defines a clipping plane.
	/**
	 * @param n The index of the clipping plane.
	 * @param plane The clipping plane - omit or set to null to disable this plane completely.
	 */
	void setClippingPlane( int n, QVector4D plane = QVector4D(0,0,0,0) );
	/// Enables all defined clipping planes.
	void enableClippingPlanes();
	/// Disables all defined clipping planes.
	void disableClippingPlanes();

	/// Sets the position
	void setPosition( const QVector3D & position ) { mPosition = position; }
	/// Sets the position on the X axis
	void setPositionX( const qreal & x ) { mPosition.setX(x); }
	/// Sets the position on the Y axis
	void setPositionY( const qreal & y ) { mPosition.setY(y); }
	/// Sets the position on the Z axis
	void setPositionZ( const qreal & z ) { mPosition.setZ(z); }

	/// Sets the rotation
	void setRotation( const QQuaternion & rotation ) { mRotation = rotation; }

	void setScale( const QVector3D & scale ) { mScale = scale; }

	/// The object's position
	const QVector3D & position() const { return mPosition; }
	/// The object's rotation
	const QQuaternion & rotation() const { return mRotation; }

	const QVector3D & scale() const { return mScale; }
	const QMatrix4x4 & matrix() const { return mMatrix; }
	const QMatrix4x4 & matrixInverse() const { return mMatrixInverse; }

protected:

private:
	Scene * mScene;
	QVector3D mPosition;
	QQuaternion mRotation;
	QVector3D mScale;
	QWeakPointer<AObject> mAttached;
	QMap<int,QVector4D> mClippingPlanes;
	void applyClippingPlanes();
	float mFOV;
	float mNearPlane;
	float mFarPlane;
	QMatrix4x4 mMatrix;
	QMatrix4x4 mMatrixInverse;
};


#endif
