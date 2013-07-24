#ifndef SCENE_OBJECT_AOBJECT_INCLUDED
#define SCENE_OBJECT_AOBJECT_INCLUDED


#include <GLWidget.hpp>

#include <QQuaternion>
#include <QVector4D>
#include <QMatrix4x4>
#include <QLinkedList>
#include <QSharedPointer>

#include <utility/FrustumTest.hpp>


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
 * it's parent object on each update( const double & delta ) call.\n
 * Changes in position/orientation are only allowed in an update pass.
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
	const QVector4D toWorld( const QVector4D & v ) { validateMatrix(); return mModelMatrix * v; }
	/// Transform a point vectorfrom local object space to world space
	const QVector3D pointToWorld( const QVector3D & v ) { validateMatrix(); return (mModelMatrix * QVector4D(v,1)).toVector3D(); }
	/// Transform a direction vector from local object space to world space
	const QVector3D directionToWorld( const QVector3D & v ) { validateMatrix(); return (mModelMatrix * QVector4D(v,0)).toVector3D(); }

	/// Returns the transformation matrix to eye space - only valid while drawing
	const QMatrix4x4 & modelViewMatrix() const { return mModelViewMatrix; }
	/// The object's position in eye space - only valid while drawing
	const QVector3D eyePosition() const { return mModelViewMatrix.row(3).toVector3D(); }
	/// Returns the vector in eye space pointing along the positive local X axis - only valid while drawing
	const QVector3D eyeLeft() const { return mModelViewMatrix.row(0).toVector3D(); }
	/// Returns the vector in eye space pointing along the positive local Y axis - only valid while drawing
	const QVector3D eyeUp() const { return mModelViewMatrix.row(1).toVector3D(); }
	/// Returns the vector in eye space pointing along the positive local Z axis - only valid while drawing
	const QVector3D eyeDirection() const { return mModelViewMatrix.row(2).toVector3D(); }

	/// Returns the transformation matrix to world space
	const QMatrix4x4 & modelMatrix() const { validateMatrix(); return mModelMatrix; }
	/// The object's position in world space
	const QVector3D worldPosition() const { validateMatrix(); return mModelMatrix.column(3).toVector3D(); }
	/// Returns the vector in world space pointing along the positive local X axis
	const QVector3D worldLeft() const { validateMatrix(); return mModelMatrix.column(0).toVector3D(); }
	/// Returns the vector in world space pointing along the positive local Y axis
	const QVector3D worldUp() const { validateMatrix(); return mModelMatrix.column(1).toVector3D(); }
	/// Returns the vector in world space pointing along the positive local Z axis
	const QVector3D worldDirection() const { validateMatrix(); return mModelMatrix.column(2).toVector3D(); }

	/// Returns the vector pointing along the positive local X axis
	const QVector3D left() const { return mRotation.rotatedVector(QVector3D(1,0,0)); }
	/// Returns the vector pointing along the positive local Y axis
	const QVector3D up() const { return mRotation.rotatedVector(QVector3D(0,1,0)); }
	/// Returns the vector pointing along the positive local Z axis
	const QVector3D direction() const { return mRotation.rotatedVector(QVector3D(0,0,1)); }

	/// Adds a vector to the local position
	void move( const QVector3D & distance ) { mPosition += distance; mModelMatrixNeedsUpdate = true; }
	/// Adds a scalar to the local position on the X axis
	void moveX( const qreal & x ) { mPosition.setX(mPosition.x()+x); mModelMatrixNeedsUpdate = true; }
	/// Adds a scalar to the local position on the Y axis
	void moveY( const qreal & y ) { mPosition.setY(mPosition.y()+y); mModelMatrixNeedsUpdate = true; }
	/// Adds a scalar to the local position on the Z axis
	void moveZ( const qreal & z ) { mPosition.setZ(mPosition.z()+z); mModelMatrixNeedsUpdate = true; }
	/// Sets the local position
	void setPosition( const QVector3D & position ) { mPosition = position; mModelMatrixNeedsUpdate = true; }
	/// Sets the local position on the X axis
	void setPositionX( const qreal & x ) { mPosition.setX(x); mModelMatrixNeedsUpdate = true; }
	/// Sets the local position on the Y axis
	void setPositionY( const qreal & y ) { mPosition.setY(y); mModelMatrixNeedsUpdate = true; }
	/// Sets the local position on the Z axis
	void setPositionZ( const qreal & z ) { mPosition.setZ(z); mModelMatrixNeedsUpdate = true; }
	/// Sets the local rotation
	void setRotation( const QQuaternion & rotation ) { mRotation = rotation; mModelMatrixNeedsUpdate = true; }

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

	/// Returns the bounding sphere
	const float & boundingSphereRadius() const { return mBoundingSphereRadius; }

	/// Recursively intersect a line with an object and the object's objects
	virtual AObject * intersectLine( const QVector3D & origin, const QVector3D & direction,
		float & length, QVector3D * normal = NULL );

	/// Recursively collision-test a sphere with an object and the object's objects
	virtual bool collideSphere( const float & radius,
		QVector3D & center, QVector3D * normal = NULL );

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
	QMatrix4x4 mModelViewMatrix;

	/// Precalculated model matrix
	mutable QMatrix4x4 mModelMatrix;
	/// Set to true if this object's position/orientation changed
	mutable bool mModelMatrixNeedsUpdate;
	/// Synchronizes matrix to current position and rotation
	void syncMatrix() const;
	/// Synchronizes matrix to current position and rotation if necessary
	void validateMatrix() const { if( mModelMatrixNeedsUpdate ) { syncMatrix(); mModelMatrixNeedsUpdate = false; } }

	void setParent( AObject * parent ) { mParent = parent; }
};


#endif
