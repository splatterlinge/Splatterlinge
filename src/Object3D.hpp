#ifndef OBJECT3D_INCLUDED
#define OBJECT3D_INCLUDED


#include <QQuaternion>
#include <QVector4D>
#include <QMatrix4x4>
#include <QLinkedList>
#include <QSharedPointer>


class GLScene;


class Object3D
{
public:
	Object3D( GLScene * scene );
	Object3D( const Object3D & other );
	virtual ~Object3D();
	Object3D & operator=( const Object3D & other );

	const QMatrix4x4 & matrix() const { validateMatrix(); return mMatrix; }
	const QVector3D left() const { validateMatrix(); return mMatrix.row(0).toVector3D(); }
	const QVector3D up() const { validateMatrix(); return mMatrix.row(1).toVector3D(); }
	const QVector3D direction() const { validateMatrix(); return mMatrix.row(2).toVector3D(); }

	void setPosition( const QVector3D & position ) { mPosition = position; mMatrixNeedsUpdate = true; }
	void setPositionX( const qreal & x ) { mPosition.setX(x); mMatrixNeedsUpdate = true; }
	void setPositionY( const qreal & y ) { mPosition.setY(y); mMatrixNeedsUpdate = true; }
	void setPositionZ( const qreal & z ) { mPosition.setZ(z); mMatrixNeedsUpdate = true; }
	void setRotation( const QQuaternion & rotation ) { mRotation = rotation; mMatrixNeedsUpdate = true; }

	const QVector3D & position() const { return mPosition; }
	const QQuaternion & rotation() const { return mRotation; }

	void add( QSharedPointer<Object3D> other );
	void remove( QSharedPointer<Object3D> other );
	Object3D * parent() { return mParent; }
	GLScene * scene() { return mScene; }
	QList< QSharedPointer<Object3D> > & subNodes() { return mSubNodes; }

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
	GLScene * mScene;
	Object3D * mParent;
	QVector3D mPosition;
	QQuaternion mRotation;
	QList< QSharedPointer<Object3D> > mSubNodes;
	mutable bool mMatrixNeedsUpdate;

	void validateMatrix() const { if( mMatrixNeedsUpdate ) { updateMatrix(); mMatrixNeedsUpdate = false; } }

	void setParent( Object3D * parent ) { mParent = parent; }
};

#endif
