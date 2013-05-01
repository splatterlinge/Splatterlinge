#ifndef MOUSELISTENER_INCLUDED
#define MOUSELISTENER_INCLUDED


#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>


class MouseListener
{
public:
	virtual void mousePressEvent( QGraphicsSceneMouseEvent * event ) = 0;
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) = 0;
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) = 0;
	virtual void wheelEvent( QGraphicsSceneWheelEvent * event ) = 0;
};


#endif
