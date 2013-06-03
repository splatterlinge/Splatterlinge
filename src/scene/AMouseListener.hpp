#ifndef SCENE_AMOUSELISTENER_INCLUDED
#define SCENE_AMOUSELISTENER_INCLUDED


#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>


class MouseMoveEvent
{
public:
	MouseMoveEvent( const QPointF & delta = QPointF(0,0) ) : mDelta(delta) {}
	const QPointF & delta() { return mDelta; }
	void setDelta( const QPointF & delta ) { mDelta = delta; }
private:
	QPointF mDelta;
};


/// Abstract mouse listener
/**
 * Register at Scene to receive mouse events.
 */
class AMouseListener
{
public:
	virtual void mousePressEvent( QGraphicsSceneMouseEvent * event ) = 0;
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) = 0;
	virtual void mouseMoveEvent( MouseMoveEvent * event ) = 0;
	virtual void mouseWheelEvent( QGraphicsSceneWheelEvent * event ) = 0;
};


#endif
