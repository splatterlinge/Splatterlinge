/*
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

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

	/// Updates position and rotation.
	void update( const double & delta );
	/// Applies position/velocity/orientation to OpenAL.
	void applyAL();
	/// Applies OpenGL projection/modelview matrices and clipping planes.
	void applyGL();

	/// Attach this eye to an object.
	void attach( QWeakPointer<AObject> object );
	/// Detaches this eye from an object.
	void detach();

	/// Field of view in degrees.
	float fov() const { return mFOV; }
	/// Set field of view in degrees.
	void setFOV( const float & fov ) { mFOV = fov; }

	/// Aspect ratio
	float aspect() const { return mAspect; }
	/// Set aspect ratio
	void setAspect( const float & aspect ) { mAspect = aspect; }

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

	/// Sets the scale
	void setScale( const QVector3D & scale ) { mScale = scale; }

	void setViewOffset( const QVector3D & offset ) { mViewOffset = offset; }

	/// The object's position
	const QVector3D & position() const { return mPosition; }
	/// The object's rotation
	const QQuaternion & rotation() const { return mRotation; }

	const QVector3D & scale() const { return mScale; }
	const QMatrix4x4 & viewMatrix() const { return mViewMatrix; }
	const QMatrix4x4 & viewMatrixInverse() const { return mViewMatrixInverse; }
	const QMatrix4x4 & projectionMatrix() const { return mProjectionMatrix; }

protected:

private:
	Scene * mScene;
	QVector3D mPosition;
	QVector3D mLastPosition;
	QVector3D mVelocity;
	QQuaternion mRotation;
	QVector3D mScale;
	QWeakPointer<AObject> mAttached;
	QMap<int,QVector4D> mClippingPlanes;
	void applyClippingPlanes();
	float mFOV;
	float mAspect;
	float mNearPlane;
	float mFarPlane;
	QMatrix4x4 mViewMatrix;
	QMatrix4x4 mViewMatrixInverse;
	QMatrix4x4 mProjectionMatrix;
	QVector3D mViewOffset;
};


#endif
