#ifndef SCENE_AMOUSELISTENER_INCLUDED
#define SCENE_AMOUSELISTENER_INCLUDED


#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>


/// Abstract mouse listener
/**
 * Register at Scene to receive mouse events.
 */
class AMouseListener
{
public:
	virtual void mousePressEvent( QGraphicsSceneMouseEvent * event ) = 0;
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) = 0;
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) = 0;
	virtual void wheelEvent( QGraphicsSceneWheelEvent * event ) = 0;
};


#endif
