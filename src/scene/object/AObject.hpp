#ifndef SCENE_OBJECT_AOBJECT_INCLUDED
#define SCENE_OBJECT_AOBJECT_INCLUDED


#include <GLWidget.hpp>

#include <QQuaternion>
#include <QVector4D>
#include <QMatrix4x4>
#include <QLinkedList>
#include <QSharedPointer>

#include <utility/FrustumTest.hpp>
#include "ALightSource.hpp"


class Scene;


/// Abstract object in a scene
/**
 * Base class of every object in a scene.
 * Each object has it's orientation stored as a quaternion and it's position stored as a Vector.\n
 * Other objects can be subordinated and are transformed relative to their parents.\n
 * - This is the usual rendering sequence:\n
 *   - Update:
 *      1. updateSelf( const double & delta )
 *      2. (update subnodes)
 *      3. updateSelfPost( const double & delta )
 *   - Update (second pass):
 *      4. update2Self( const double & delta )
 *      5. (update subnodes - second pass)
 *      6. update2SelfPost( const double & delta )
 *   - Draw:
 *      7. drawSelf()
 *      8. (draw subnodes)
 *      9. drawSelfPost()
 *   - Draw (second pass):
 *     10. draw2Self()
 *     11. (draw subnodes - second pass)
 *     12. draw2SelfPost()
 * Each object tracks its own model transformation matrix and synchronizes it with
 */
class AObject
{
public:
	/// Creates an object for the given scene
	AObject( Scene * scene, float boundingSphereRadius=0.0f );
	/// Copies an object
	AObject( const AObject & other );
	/// Frees this object
	virtual ~AObject() = 0;
	/// Copies an object
	AObject & operator=( const AObject & other );

	/// Transform a vector from local object space to world space
	const QVector4D toWorld( const QVector4D & v ) { validateMatrix(); return mMatrix * v; }
	/// Transform a point vectorfrom local object space to world space
	const QVector3D pointToWorld( const QVector3D & v ) { validateMatrix(); return (mMatrix * QVector4D(v,1)).toVector3D(); }
	/// Transform a direction vector from local object space to world space
	const QVector3D directionToWorld( const QVector3D & v ) { validateMatrix(); return (mMatrix * QVector4D(v,0)).toVector3D(); }

	/// Returns the transformation matrix to world space
	const QMatrix4x4 & matrix() const { validateMatrix(); return mMatrix; }
	/// The object's position in world space
	const QVector3D worldPosition() const { validateMatrix(); return mMatrix.column(3).toVector3D(); }
	/// Returns the vector in world space pointing along the positive local X axis
	const QVector3D worldLeft() const { validateMatrix(); return mMatrix.column(0).toVector3D(); }
	/// Returns the vector in world space pointing along the positive local Y axis
	const QVector3D worldUp() const { validateMatrix(); return mMatrix.column(1).toVector3D(); }
	/// Returns the vector in world space pointing along the positive local Z axis
	const QVector3D worldDirection() const { validateMatrix(); return mMatrix.column(2).toVector3D(); }

	/// Returns the vector pointing along the positive local X axis
	const QVector3D left() const { return mRotation.rotatedVector(QVector3D(1,0,0)); }
	/// Returns the vector pointing along the positive local Y axis
	const QVector3D up() const { return mRotation.rotatedVector(QVector3D(0,1,0)); }
	/// Returns the vector pointing along the positive local Z axis
	const QVector3D direction() const { return mRotation.rotatedVector(QVector3D(0,0,1)); }

	/// Adds a vector to the local position
	void move( const QVector3D & distance ) { mPosition += distance; mMatrixNeedsUpdate = true; }
	/// Adds a scalar to the local position on the X axis
	void moveX( const qreal & x ) { mPosition.setX(mPosition.x()+x); mMatrixNeedsUpdate = true; }
	/// Adds a scalar to the local position on the Y axis
	void moveY( const qreal & y ) { mPosition.setY(mPosition.y()+y); mMatrixNeedsUpdate = true; }
	/// Adds a scalar to the local position on the Z axis
	void moveZ( const qreal & z ) { mPosition.setZ(mPosition.z()+z); mMatrixNeedsUpdate = true; }
	/// Sets the local position
	void setPosition( const QVector3D & position ) { mPosition = position; mMatrixNeedsUpdate = true; }
	/// Sets the local position on the X axis
	void setPositionX( const qreal & x ) { mPosition.setX(x); mMatrixNeedsUpdate = true; }
	/// Sets the local position on the Y axis
	void setPositionY( const qreal & y ) { mPosition.setY(y); mMatrixNeedsUpdate = true; }
	/// Sets the local position on the Z axis
	void setPositionZ( const qreal & z ) { mPosition.setZ(z); mMatrixNeedsUpdate = true; }
	/// Sets the local rotation
	void setRotation( const QQuaternion & rotation ) { mRotation = rotation; mMatrixNeedsUpdate = true; }

	/// The object's local position
	const QVector3D & position() const { return mPosition; }
	/// The object's local rotation
	const QQuaternion & rotation() const { return mRotation; }

	/// Add a child to this object
	void add( QSharedPointer<AObject> other );
	/// Remove a child from this object
	void remove( QSharedPointer<AObject> other );
	/// Returns a pointer to this object's parent
	AObject * parent() { return mParent; }
	/// Returns a pointer to this object's scene
	Scene * scene() { return mScene; }
	/// Returns all child objects
	QList< QSharedPointer<AObject> > & subNodes() { return mSubNodes; }

	/// Updates this object and all of it's sub-objects
	void update( const double & delta );
	/// Abstract method for updating this object
	virtual void updateSelf( const double & delta ) {}
	/// Executed after all sub-objects are updated
	virtual void updateSelfPost( const double & delta ) {}

	/// Updates this object and all of it's sub-objects (second pass)
	void update2( const double & delta );
	/// Abstract method for updating this object (second pass)
	virtual void update2Self( const double & delta ) {}
	/// Executed after all sub-objects are updated (second pass)
	virtual void update2SelfPost( const double & delta ) {}

	/// Draws this object and all of it's sub-objects
	void draw();
	/// Abstract method for drawing this object
	virtual void drawSelf() {}
	/// Executed after all sub-objects are drawn
	virtual void drawSelfPost() {}

	/// Draws this object and all of it's sub-objects (second pass)
	void draw2();
	/// Abstract method for drawing this object (second pass)
	virtual void draw2Self() {}
	/// Executed after all sub-objects are drawn (second pass)
	virtual void draw2SelfPost() {}

	/// Returns the bounding sphere
	const float & boundingSphereRadius() const { return mBoundingSphereRadius; }

	/// Causes all objects to draw the bounding sphere
	static void setGlobalDebugBoundingSpheres( bool enable ) { sDebugBoundingSpheres = enable; }

protected:
	/// Set bounding sphere radius for frustum culling
	void setBoundingSphere( const float & radius ) { mBoundingSphereRadius = radius; }
	/// Draws the bounding sphere as wireframe (for debugging)
	void drawBoundingShpere();

private:
	static bool sDebugBoundingSpheres;
	Scene * mScene;
	AObject * mParent;
	QVector3D mPosition;
	QQuaternion mRotation;
	float mBoundingSphereRadius;
	QList< QSharedPointer<AObject> > mSubNodes;
	FrustumTest mFrustumTest;

	mutable bool mMatrixNeedsUpdate;

	/// Precalculated model matrix
	mutable QMatrix4x4 mMatrix;
	/// Synchronizes matrix to current position and rotation
	void syncMatrix() const;
	/// Synchronizes matrix to current position and rotation if necessary
	void validateMatrix() const { if( mMatrixNeedsUpdate ) { syncMatrix(); mMatrixNeedsUpdate = false; } }

	void setParent( AObject * parent ) { mParent = parent; }
};

#endif
