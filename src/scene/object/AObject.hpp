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
 * 
 */
class AObject
{
public:
	AObject( Scene * scene );
	AObject( const AObject & other );
	virtual ~AObject();
	AObject & operator=( const AObject & other );

	const QMatrix4x4 & matrix() const { validateMatrix(); return mMatrix; }
	const QVector3D left() const { validateMatrix(); return mMatrix.row(0).toVector3D(); }
	const QVector3D up() const { validateMatrix(); return mMatrix.row(1).toVector3D(); }
	const QVector3D direction() const { validateMatrix(); return mMatrix.row(2).toVector3D(); }

	void move( const QVector3D & distance ) { mPosition += distance; mMatrixNeedsUpdate = true; }
	void moveX( const qreal & x ) { mPosition.setX(mPosition.x()+x); mMatrixNeedsUpdate = true; }
	void moveY( const qreal & y ) { mPosition.setY(mPosition.y()+y); mMatrixNeedsUpdate = true; }
	void moveZ( const qreal & z ) { mPosition.setZ(mPosition.z()+z); mMatrixNeedsUpdate = true; }
	void setPosition( const QVector3D & position ) { mPosition = position; mMatrixNeedsUpdate = true; }
	void setPositionX( const qreal & x ) { mPosition.setX(x); mMatrixNeedsUpdate = true; }
	void setPositionY( const qreal & y ) { mPosition.setY(y); mMatrixNeedsUpdate = true; }
	void setPositionZ( const qreal & z ) { mPosition.setZ(z); mMatrixNeedsUpdate = true; }
	void setRotation( const QQuaternion & rotation ) { mRotation = rotation; mMatrixNeedsUpdate = true; }

	const QVector3D & position() const { return mPosition; }
	const QQuaternion & rotation() const { return mRotation; }

	void add( QSharedPointer<AObject> other );
	void remove( QSharedPointer<AObject> other );
	AObject * parent() { return mParent; }
	Scene * scene() { return mScene; }
	QList< QSharedPointer<AObject> > & subNodes() { return mSubNodes; }

	void update( const float & delta );
	void draw();
	virtual void updateSelf( const float & delta ) = 0;
	virtual void drawSelf() = 0;
	virtual void updateSelfPost( const float & delta ) {};
	virtual void drawSelfPost() {};
	void drawPostProc();
	virtual void drawSelfPostProc() {};

protected:
	mutable QMatrix4x4 mMatrix;

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
