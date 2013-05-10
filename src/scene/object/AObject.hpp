#ifndef OBJECT3D_INCLUDED
#define OBJECT3D_INCLUDED


#include <QQuaternion>
#include <QVector4D>
#include <QMatrix4x4>
#include <QLinkedList>
#include <QSharedPointer>


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
	AObject( Scene * scene );
	/// Copies an object
	AObject( const AObject & other );
	/// Frees this object
	virtual ~AObject() = 0;
	/// Copies an object
	AObject & operator=( const AObject & other );

	/// Returns the transformation matrix for this object's position and orientation
	const QMatrix4x4 & matrix() const { validateMatrix(); return mMatrix; }
	/// Returns the vector pointing along the positive X axis
	const QVector3D left() const { validateMatrix(); return mMatrix.row(0).toVector3D(); }
	/// Returns the vector pointing along the positive Y axis
	const QVector3D up() const { validateMatrix(); return mMatrix.row(1).toVector3D(); }
	/// Returns the vector pointing along the positive Z axis
	const QVector3D direction() const { validateMatrix(); return mMatrix.row(2).toVector3D(); }

	/// Adds a vector to the current position
	void move( const QVector3D & distance ) { mPosition += distance; mMatrixNeedsUpdate = true; }
	/// Adds a scalar to the current position on the X axis
	void moveX( const qreal & x ) { mPosition.setX(mPosition.x()+x); mMatrixNeedsUpdate = true; }
	/// Adds a scalar to the current position on the Y axis
	void moveY( const qreal & y ) { mPosition.setY(mPosition.y()+y); mMatrixNeedsUpdate = true; }
	/// Adds a scalar to the current position on the Z axis
	void moveZ( const qreal & z ) { mPosition.setZ(mPosition.z()+z); mMatrixNeedsUpdate = true; }
	/// Sets the position
	void setPosition( const QVector3D & position ) { mPosition = position; mMatrixNeedsUpdate = true; }
	/// Sets the position on the X axis
	void setPositionX( const qreal & x ) { mPosition.setX(x); mMatrixNeedsUpdate = true; }
	/// Sets the position on the Y axis
	void setPositionY( const qreal & y ) { mPosition.setY(y); mMatrixNeedsUpdate = true; }
	/// Sets the position on the Z axis
	void setPositionZ( const qreal & z ) { mPosition.setZ(z); mMatrixNeedsUpdate = true; }
	/// Sets the rotation
	void setRotation( const QQuaternion & rotation ) { mRotation = rotation; mMatrixNeedsUpdate = true; }

	/// The object's position
	const QVector3D & position() const { return mPosition; }
	/// The object's rotation
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
	void update( const float & delta );
	/// Draws this object and all of it's sub-objects
	void draw();
	/// Abstract method for updating this object
	virtual void updateSelf( const float & delta ) = 0;
	/// Abstract method for drawing this object
	virtual void drawSelf() = 0;
	/// Executed after all sub-objects are updated
	virtual void updateSelfPost( const float & delta ) {};
	/// Executed after all sub-objects are drawn
	virtual void drawSelfPost() {};
	/// Draw post processing effects
	void drawPostProc();
	/// Executet after the usual drawing functions - e.g. for rendering FBOs
	virtual void drawSelfPostProc() {};

protected:
	/// Precalculated matrix
	mutable QMatrix4x4 mMatrix;

	/// Updates this object's matrix to match it's position and rotation
	virtual void updateMatrix() const;

private:
	Scene * mScene;
	AObject * mParent;
	QVector3D mPosition;
	QQuaternion mRotation;
	QList< QSharedPointer<AObject> > mSubNodes;
	mutable bool mMatrixNeedsUpdate;

	void validateMatrix() const { if( mMatrixNeedsUpdate ) { updateMatrix(); mMatrixNeedsUpdate = false; } }

	void setParent( AObject * parent ) { mParent = parent; }
};

#endif
