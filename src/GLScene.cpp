#include "GLScene.h"

#include <QtGui>
#include <GL/glu.h>
#include <stdio.h>

#include "teapot.h"


GLScene::GLScene()
{
	frameCount = 0;
	isDragging = false;

	QTimer * secondTimer = new QTimer( this );
	QObject::connect( secondTimer, SIGNAL(timeout()), this, SLOT(secondPassed()) );
	secondTimer->setInterval( 1000 );
	secondTimer->start();

	QTimer * updateTimer = new QTimer( this );
	QObject::connect( updateTimer, SIGNAL(timeout()), this, SLOT(update()) );
	updateTimer->setInterval( 10 );
	updateTimer->start();
}

/*
void GLScene::addItem( QGraphicsItem * item )
{
	item->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
	QGraphicsScene::addItem( item );
}
*/

QGraphicsProxyWidget * GLScene::addWidget( QWidget * widget, Qt::WindowFlags wFlags )
{
	QGraphicsProxyWidget * proxy = QGraphicsScene::addWidget( widget );
	proxy->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
	return proxy;
}


void GLScene::secondPassed()
{
	fprintf( stdout, "FPS: %d\r", frameCount );
	fflush( stdout );
	frameCount = 0;
}


void GLScene::drawBackground( QPainter * painter, const QRectF & )
{
	if( (painter->paintEngine()->type() != QPaintEngine::OpenGL2) &&
		(painter->paintEngine()->type() != QPaintEngine::OpenGL))
	{
		qWarning("GLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
		return;
	}

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel( GL_SMOOTH );
	glClearColor( 0, 0, 0, 0 );

	GLfloat diffuseMaterial[] = {0.2, 0.6, 1.0};
	GLfloat specularMaterial[] = {1.0, 1.0, 1.0};
	GLfloat emissiveMaterial[] = {0.0, 0.0, 0.0};
	GLfloat specularLight[] = {1.0, 1.0, 1.0};
	GLfloat ambientLight[] = {0.01, 0.01, 0.01};
	GLfloat diffuseLight[] = {0.9, 1.0, 1.0};
	GLfloat blankMaterial[] = {0.0, 0.0, 0.0};
	GLfloat shininess[] = {64};
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularLight );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emissiveMaterial );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//	glViewport( 0, 0, width(), height() );
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluPerspective( 60.0f, (float)width()/height(), 0.1f, 100.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glTranslatef( 0, 0, -2 );

	static float rotateX = 0.0f;
	static float rotateY = 0.0f;
	rotateY += drag.x()/4.0f + 1.0f;
	rotateX += drag.y()/4.0f;
	drag = QPoint(0,0);
	glRotatef( rotateX, 1,0,0 );
	glRotatef( rotateY, 0,1,0 );
	glColor3f( 1, 1, 1 );
	teapot( 20, 1.0f, GL_FILL );
/*
	glDisable( GL_LIGHTING );
	teapot( 10, 1.0f, GL_LINE );
	glEnable( GL_LIGHTING );
*/
//	glFlush();
	glFinish();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glPopAttrib();
	frameCount++;
}

void GLScene::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseMoveEvent( event );
	if( event->isAccepted() )
		return;
	if( isDragging )
	{
		drag += event->screenPos() - event->lastScreenPos();
		event->accept();
	}
}

void GLScene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mousePressEvent( event );
	if( event->isAccepted() )
		return;
	isDragging = true;
}

void GLScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	isDragging = false;
}

void GLScene::wheelEvent( QGraphicsSceneWheelEvent * event )
{
	QGraphicsScene::wheelEvent( event );

}

void GLScene::keyPressEvent( QKeyEvent * event )
{
	QGraphicsScene::keyPressEvent( event );
	if( event->isAccepted() )
		return;
	switch( event->key() )
	{
	default:
		event->ignore();
		break;
	}
}
