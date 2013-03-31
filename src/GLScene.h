#ifndef GLSCENE_INCLUDED
#define GLSCENE_INCLUDED

#include <QGraphicsScene>
#include <QPointF>


class QGraphicsItem;
class QGraphicsProxyWidget;


class GLScene : public QGraphicsScene
{
	Q_OBJECT
public:
	GLScene();
	void drawBackground( QPainter * painter, const QRectF & rect );
//	void addItem( QGraphicsItem * item );
	QGraphicsProxyWidget * addWidget( QWidget * widget, Qt::WindowFlags wFlags = 0 );

protected:
	void keyPressEvent( QKeyEvent * event );
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void wheelEvent(QGraphicsSceneWheelEvent * wheelEvent);

private:
	int frameCount;
	QPointF drag;
	bool isDragging;

private slots:
	void secondPassed();
};


#endif
