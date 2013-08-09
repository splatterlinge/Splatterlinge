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
