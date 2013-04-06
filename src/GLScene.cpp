#include "GLScene.h"

#include <QtGui>
#include <GL/glu.h>
#include "teapot.h"


GLScene::GLScene( QGLWidget * glWidget, QObject * parent ) : QGraphicsScene( parent )
{
	mFrameCountSecond = 0;
	mFramesPerSecond = 0;
	mDragging = false;
	mGLWidget = glWidget;

	mFont = QFont( "Sans", 12, QFont::Normal );

	glWidget->makeCurrent();

	basicShader = new QGLShaderProgram( glWidget );
	basicShader->addShaderFromSourceFile( QGLShader::Vertex, "./data/basic.vsh" );
	basicShader->addShaderFromSourceFile( QGLShader::Fragment, "./data/basic.fsh" );
	if( !basicShader->link() )
		qWarning() << basicShader->log();

	QTimer * secondTimer = new QTimer( this );
	QObject::connect( secondTimer, SIGNAL(timeout()), this, SLOT(secondPassed()) );
	secondTimer->setInterval( 1000 );
	secondTimer->start();

	QTimer * updateTimer = new QTimer( this );
	QObject::connect( updateTimer, SIGNAL(timeout()), this, SLOT(update()) );
	updateTimer->setInterval( 10 );
	updateTimer->start();
}


QGraphicsProxyWidget * GLScene::addWidget( QWidget * widget, Qt::WindowFlags wFlags )
{
	QGraphicsProxyWidget * proxy = QGraphicsScene::addWidget( widget, wFlags );
	proxy->setFlag( QGraphicsItem::ItemIsMovable );
	proxy->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
	return proxy;
}


void GLScene::drawBackground( QPainter * painter, const QRectF & rect )
{
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glMatrixMode( GL_TEXTURE );	glPushMatrix();
	glMatrixMode( GL_PROJECTION );	glPushMatrix();
	glMatrixMode( GL_MODELVIEW );	glPushMatrix();

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
//	GLfloat blankMaterial[] = {0.0, 0.0, 0.0};
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

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0f, (float)width()/height(), 0.1f, 100.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glTranslatef( 0, 0, -2 );

	static float rotateX = 0.0f;
	static float rotateY = 0.0f;
	rotateY += mDrag.x()/4.0f + 1.0f;
	rotateX += mDrag.y()/4.0f;
	mDrag = QPoint( 0, 0 );
	glRotatef( rotateX, 1,0,0 );
	glRotatef( rotateY, 0,1,0 );
	glColor3f( 1, 1, 1 );
	basicShader->bind();
	teapot( 20, 1.0f, GL_FILL );
	basicShader->release();
/*
	glDisable( GL_LIGHTING );
	teapot( 10, 1.0f, GL_LINE );
	glEnable( GL_LIGHTING );
*/
//	glFlush();
	glFinish();

	glMatrixMode( GL_TEXTURE );	glPopMatrix();
	glMatrixMode( GL_PROJECTION );	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );	glPopMatrix();
	glPopAttrib();

	mFrameCountSecond++;

	painter->setPen( QColor(255,255,255) );
	painter->setFont( mFont );
	painter->drawText( rect, Qt::AlignTop | Qt::AlignRight, QString( tr("%1 FPS") ).arg(mFramesPerSecond) );

	GLenum error = glGetError();
	if( error )
		qWarning() << QString( reinterpret_cast<const char*>(gluErrorString(error)) );
}


void GLScene::secondPassed()
{
	mFramesPerSecond = mFrameCountSecond;
	mFrameCountSecond = 0;
}


void GLScene::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseMoveEvent( event );
	if( event->isAccepted() )
		return;
	if( mDragging )
	{
		mDrag += event->screenPos() - event->lastScreenPos();
		event->accept();
	}
}


void GLScene::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mousePressEvent( event );
	if( event->isAccepted() )
		return;
	mDragging = true;
}


void GLScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	mDragging = false;
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
