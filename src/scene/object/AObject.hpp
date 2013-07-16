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
 * Each object has it's orientation stored as a quaternion and it's position stored as a Vector.
 * Other objects can be subordinated and are transformed relative to their parents.\n
 * - This is the usual rendering sequence:\n
 *   - Update:
 *     1. updateSelf( const float & delta )
 *     2. (update subnodes)
 *     3. updateSelfPost( const float & delta )
 *   - Draw:
 *     4. drawSelf()
 *     5. (draw subnodes)
 *     6. drawSelfPost()
 *   - PostProc
 *     7. drawSelfPostProc()
 *     8. (draw subnode's post proc)
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

	const QVector4D toWorld( const QVector4D & v ) { validateMatrix(); return mMatrix * v; }
	const QVector3D pointToWorld( const QVector3D & v ) { validateMatrix(); return (mMatrix * QVector4D(v,1)).toVector3D(); }
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
	virtual void updateSelf( const double & delta ) = 0;
	/// Executed after all sub-objects are updated
	virtual void updateSelfPost( const double & delta ) {};

	/// Draws this object and all of it's sub-objects
	void draw();
	/// Abstract method for drawing this object
	virtual void drawSelf() = 0;
	/// Executed after all sub-objects are drawn
	virtual void drawSelfPost() {};

	/// Executed after the usual drawing functions - e.g. for rendering FBOs
	void drawAfter();
	/// Executed after the usual drawing functions - e.g. for rendering FBOs
	virtual void drawAfterSelf() {};

	const float & boundingSphereRadius() const { return mBoundingSphereRadius; }

	static void setGlobalDebugBoundingSpheres( bool enable ) { sDebugBoundingSpheres = enable; }

protected:
	void setBoundingSphere( const float & radius ) { mBoundingSphereRadius = radius; }
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

	/// Precalculated matrix
	mutable QMatrix4x4 mMatrix;
	/// Updates this object's matrix to match it's position and rotation
	void updateMatrix() const;

	void validateMatrix() const { if( mMatrixNeedsUpdate ) { updateMatrix(); mMatrixNeedsUpdate = false; } }

	void setParent( AObject * parent ) { mParent = parent; }
};

#endif
