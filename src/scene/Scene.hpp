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

#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED

#include "object/Eye.hpp"
#include "scene/object/World.hpp"
#include "scene/object/creature/Player.hpp"

#include <QGraphicsScene>
#include <QElapsedTimer>
#include <QRectF>
#include <QGLBuffer>


class QPainter;
class QGraphicsItem;
class QGraphicsProxyWidget;
class QGLWidget;
class QKeyEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

class GLWidget;
class DebugWindow;
class StartMenuWindow;
class AMouseListener;
class AKeyListener;
class TextureRenderer;
class Shader;


/// Scene manager and interface to Qt
/**
 *
 */
class Scene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit Scene( GLWidget * glWidget, QObject * parent = 0 );
	~Scene();

	// Overrides:
	void drawBackground( QPainter * painter, const QRectF & rect );
	QGraphicsProxyWidget * addWidget( QWidget * widget, Qt::WindowFlags wFlags = 0 );
	void setSceneRect( const QRectF & rect );
	void setSceneRect( qreal x, qreal y, qreal w, qreal h ) { setSceneRect( QRectF( x, y, w, h ) ); }

	GLWidget * glWidget() { return mGLWidget; }

	AObject * root() const { return mRoot; }
	void setRoot( AObject * root ) { mRoot = root; }

	Eye * eye() const { return mEye; }
	void setEye( Eye * eye ) { mEye = eye; }

	void addKeyListener( AKeyListener * listener ) { mKeyListeners.append( listener ); }
	void addMouseListener( AMouseListener * listener ) { mMouseListeners.append( listener ); }
	void removeKeyListener( AKeyListener * listener ) { mKeyListeners.removeOne( listener ); }
	void removeMouseListener( AMouseListener * listener ) { mMouseListeners.removeOne( listener ); }

	void setMouseGrabbing( bool enable );
	bool isMouseGrabbing() const { return mMouseGrabbing; }

	void setWireFrame( bool enable ) { mWireFrame = enable; }
	bool wireFrame() const { return mWireFrame; }
	void setMultiSample( bool enable ) { mMultiSample = enable; }
	bool multiSample() const { return mMultiSample; }
	void setStereo( bool enable ) { mStereo = enable; resizeStereoFrameBuffers(QSize(width(),height())); }
	bool stereo() const { return mStereo; }
	void setStereoEyeDistance( float distance ) { mStereoEyeDistance = distance; }
	float stereoEyeDistance() const { return mStereoEyeDistance; }
	void setStereoUseOVR( bool useOVR ) { mStereoUseOVR = useOVR; }
	bool stereoUseOVR() const { return mStereoUseOVR; }

	StartMenuWindow * startMenuWindow() { return mStartMenuWindow; }
	DebugWindow * debugWindow() { return mDebugWindow; }

protected:
	// Overrides:
	void keyPressEvent( QKeyEvent * event );
	void keyReleaseEvent( QKeyEvent * event );
	void mousePressEvent( QGraphicsSceneMouseEvent * event );
	void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
	void wheelEvent( QGraphicsSceneWheelEvent * event );

private:
	static const GLfloat sQuadVertices[];
	static QGLBuffer sQuadVertexBuffer;

	GLWidget * mGLWidget;

	DebugWindow * mDebugWindow;
	StartMenuWindow * mStartMenuWindow;

	QElapsedTimer mElapsedTimer;
	double mDelta;
	int mFrameCountSecond;
	int mFramesPerSecond;
	QFont mFont;
	bool mWireFrame;
	bool mMultiSample;
	bool mStereo;
	float mStereoEyeDistance;
	bool mStereoUseOVR;
	Shader * mOVRShader;

	bool mMouseGrabbing;

	QList<AMouseListener*> mMouseListeners;
	QList<AKeyListener*> mKeyListeners;
	Eye * mEye;
	AObject * mRoot;

	TextureRenderer * mLeftTextureRenderer;
	TextureRenderer * mRightTextureRenderer;

	void resizeStereoFrameBuffers( const QSize & screenSize );
	void drawStereoFrameBuffers();
	void drawFPS( QPainter * painter, const QRectF & rect );
	void drawHUD( QPainter * painter, const QRectF & rect );
	void applyDefaultStatesGL();
	void pushAllGL();
	void popAllGL();
	void updateObjects( const double & delta );
	void drawObjects();

private slots:
	void secondPassed();
};


#endif
