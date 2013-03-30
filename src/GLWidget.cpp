#include <stdio.h>
#include <GL/glu.h>

#include <QMouseEvent>
#include <QTimer>

#include <QDebug>
#include <assert.h>

#include "teapot.h"

#include "GLWidget.h"


GLWidget::GLWidget( QGLFormat & glFormat, QWidget * parent ) : QGLWidget( glFormat, parent )
{
	frameCount = 0;

	QTimer * secondTimer = new QTimer( this );
	QObject::connect( secondTimer, SIGNAL(timeout()), this, SLOT(secondPassed()) );
	secondTimer->setInterval( 1000 );
	secondTimer->start();

	QTimer * updateTimer = new QTimer( this );
	QObject::connect( updateTimer, SIGNAL(timeout()), this, SLOT(update()) );
	updateTimer->setInterval( 10 );
	updateTimer->start();
}


void GLWidget::secondPassed()
{
	fprintf( stdout, "FPS: %d\r", frameCount );
	fflush( stdout );
	frameCount = 0;
}


void GLWidget::initializeGL()
{
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel( GL_SMOOTH );
// 	glEnable( GL_POLYGON_SMOOTH );
// 	glEnable( GL_LINE_SMOOTH );
// 	glPointSize( 1.0 );
// 	glLineWidth( 1.0 );
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
}


void GLWidget::resizeGL( int w, int h )
{
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0f, (float)w/h, 0.1f, 100.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}


static const char * glErrorString( GLenum err )
{
	switch(err)
	{
		case GL_INVALID_ENUM: return "Invalid Enum";
		case GL_INVALID_VALUE: return "Invalid Value";
		case GL_INVALID_OPERATION: return "Invalid Operation";
		case GL_STACK_OVERFLOW: return "Stack Overflow";
		case GL_STACK_UNDERFLOW: return "Stack Underflow";
		case GL_OUT_OF_MEMORY: return "Out of Memory";
		case GL_TABLE_TOO_LARGE: return "Table too Large";
		default: return "Unknown Error";
	}
}


void GLWidget::paintGL()
{
	frameCount++;
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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
	
	GLenum err = glGetError();
	if( err != GL_NO_ERROR )
	{
		qDebug() << glErrorString( err );
	}
}


void GLWidget::mousePressEvent( QMouseEvent * event )
{
	lastMousePos = event->pos();
}


void GLWidget::mouseMoveEvent( QMouseEvent * event )
{
	drag += event->pos() - lastMousePos;
	lastMousePos = event->pos();
}


void GLWidget::keyPressEvent( QKeyEvent * event )
{
	switch( event->key() )
	{
	case Qt::Key_Escape:
		close();
		break;
	default:
		event->ignore();
		break;
	}
}
