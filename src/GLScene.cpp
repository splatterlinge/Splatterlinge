#include "GLScene.hpp"

#include <QtGui>
#include <GL/glu.h>
#include <math.h>
#include "Terrain.hpp"
#include "teapot.h"


GLScene::GLScene( QGLWidget * glWidget, QObject * parent ) : QGraphicsScene( parent )
{
	mFrameCountSecond = 0;
	mFramesPerSecond = 0;
	mDragging = false;
	mGLWidget = glWidget;

	mForwardPressed = false;
	mLeftPressed = false;
	mBackwardPressed = false;
	mRightPressed = false;
	mUpPressed = false;
	mDownPressed = false;
	mSpeedPressed = false;

	mFont = QFont( "Sans", 12, QFont::Normal );

	mGLWidget->makeCurrent();
	
	mTerrain = new Terrain("./data/terrain/", QVector3D(1000,100,1000), QVector3D(-500,-50,-500) );

	basicShader = new QGLShaderProgram( glWidget );
	basicShader->addShaderFromSourceFile( QGLShader::Vertex, "./data/shader/versatile.medium.vsh" );
	basicShader->addShaderFromSourceFile( QGLShader::Fragment, "./data/shader/versatile.medium.fsh" );
	if( !basicShader->link() )
		qWarning() << basicShader->log();

	mDiffuseMap1 = mGLWidget->bindTexture( QPixmap("./data/texture/KirksEntry.diffuse.png"), GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mNormalMap1 = mGLWidget->bindTexture( QPixmap("./data/texture/KirksEntry.normal.png"), GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mSpecularMap1 = mGLWidget->bindTexture( QPixmap("./data/texture/KirksEntry.specular.png"), GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );

	mDiffuseMap2 = mGLWidget->bindTexture( QPixmap("./data/texture/DirtyConcrete.diffuse.png"), GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mNormalMap2 = mGLWidget->bindTexture( QPixmap("./data/texture/DirtyConcrete.normal.png"), GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mSpecularMap2 = mGLWidget->bindTexture( QPixmap("./data/texture/DirtyConcrete.specular.png"), GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );

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
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	glClearColor( 0, 0, 0, 0 );

	GLfloat specularLight[] = {1.0, 1.0, 1.0};
	GLfloat ambientLight[] = {0.01, 0.01, 0.01};
	GLfloat diffuseLight[] = {0.9, 1.0, 1.0};
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularLight );
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );

	GLfloat diffuseMaterial[] = {1.0, 1.0, 1.0};
	GLfloat specularMaterial[] = {1.0, 1.0, 1.0};
	GLfloat emissiveMaterial[] = {0.0, 0.0, 0.0};
	GLfloat shininess[] = {127};
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emissiveMaterial );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0f, (float)width()/height(), 0.1f, 1000.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	static float rotateX = 0.0f;
	static float rotateY = 0.0f;
	rotateY += mDrag.x()/4.0f;
	rotateX += mDrag.y()/4.0f;
	mDrag = QPoint( 0, 0 );
	glRotatef( rotateX, 1,0,0 );
	glRotatef( rotateY, 0,1,0 );
	
	float moveX = 0.0f;
	float moveY = 0.0f;
	float moveZ = 0.0f;
	static float posX = 0.0f;
	static float posY = 0.0f;
	static float posZ = 0.0f;
	if( mForwardPressed )
		moveZ -= 1.0f;
	if( mBackwardPressed )
		moveZ += 1.0f;
	if( mLeftPressed )
		moveX -= 1.0f;
	if( mRightPressed )
		moveX += 1.0f;
	if( mUpPressed )
		moveY += 1.0f;
	if( mDownPressed )
		moveY -= 1.0f;
	if( mSpeedPressed )
	{
		moveX *= 1.0;
		moveY *= 1.0;
		moveZ *= 1.0;
	} else {
		moveX *= 0.2;
		moveY *= 0.2;
		moveZ *= 0.2;
	}
	posX += cosf( rotateY*(M_PI/180.0) ) * moveX - sinf( rotateY*(M_PI/180.0) ) * moveZ;
	posZ += sinf( rotateY*(M_PI/180.0) ) * moveX + cosf( rotateY*(M_PI/180.0) ) * moveZ;
	posY += moveY;
	glTranslatef( -posX, -posY, -posZ );
	
	GLfloat lightPosition[] = {1.0, 0.5, 1.0, 0.0};
	glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );
/*
	glColor3f( 1, 1, 1 );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	mTerrain->draw();
*/
	basicShader->bind();
	basicShader->setUniformValue( "diffusemap", 0 );
	basicShader->setUniformValue( "specularmap", 1 );
	basicShader->setUniformValue( "normalmap", 2 );
	basicShader->setUniformValue( "activelights", 1 );

	glEnable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE2 );	glBindTexture( GL_TEXTURE_2D, mNormalMap1 );
	glActiveTexture( GL_TEXTURE1 );	glBindTexture( GL_TEXTURE_2D, mSpecularMap1 );
	glActiveTexture( GL_TEXTURE0 );	glBindTexture( GL_TEXTURE_2D, mDiffuseMap1 );

	glPushMatrix();
	glTranslatef( 0, -16, 0 );
	glDisable( GL_CULL_FACE );
	teapot( 20, 32.0f, GL_FILL );
	glEnable( GL_CULL_FACE );
	glPopMatrix();
	
	glActiveTexture( GL_TEXTURE2 );	glBindTexture( GL_TEXTURE_2D, mNormalMap2 );
	glActiveTexture( GL_TEXTURE1 );	glBindTexture( GL_TEXTURE_2D, mSpecularMap2 );
	glActiveTexture( GL_TEXTURE0 );	glBindTexture( GL_TEXTURE_2D, mDiffuseMap2 );
	glMatrixMode( GL_TEXTURE );	glPushMatrix();
	glScalef( 100.0f, 100.0f, 100.0f );
	mTerrain->draw();
	glMatrixMode( GL_TEXTURE );	glPopMatrix();
//	mTerrain->drawPatch( posX-200, posZ-200, 400, 400 );

	basicShader->release();

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
	case Qt::Key_W:
		mForwardPressed = true;
		break;
	case Qt::Key_A:
		mLeftPressed = true;
		break;
	case Qt::Key_S:
		mBackwardPressed = true;
		break;
	case Qt::Key_D:
		mRightPressed = true;
		break;
	case Qt::Key_Space:
		mUpPressed = true;
		break;
	case Qt::Key_Shift:
		mSpeedPressed = true;
		break;
	case Qt::Key_Control:
		mDownPressed = true;
		break;
	default:
		event->ignore();
		return;
	}
	event->accept();
}


void GLScene::keyReleaseEvent( QKeyEvent * event )
{
	QGraphicsScene::keyReleaseEvent( event );
	if( event->isAccepted() )
		return;
	switch( event->key() )
	{
	case Qt::Key_W:
		mForwardPressed = false;
		break;
	case Qt::Key_A:
		mLeftPressed = false;
		break;
	case Qt::Key_S:
		mBackwardPressed = false;
		break;
	case Qt::Key_D:
		mRightPressed = false;
		break;
	case Qt::Key_Space:
		mUpPressed = false;
		break;
	case Qt::Key_Shift:
		mSpeedPressed = false;
		break;
	case Qt::Key_Control:
		mDownPressed = false;
		break;
	default:
		event->ignore();
		return;
	}
	event->accept();
}
