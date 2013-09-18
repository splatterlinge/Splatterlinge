/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
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

#ifdef OVR_ENABLED
#include "OVR.h"
#endif


class QPainter;
class QGraphicsItem;
class QGraphicsProxyWidget;
class QGLWidget;
class QKeyEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

class GLWidget;
class OptionWindow;
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

	void setPaused( bool enable ) { mPaused = enable; }
	bool paused() { return mPaused; }
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

#ifdef OVR_ENABLED
	QQuaternion OVROrientation();
#endif

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

	StartMenuWindow * mStartMenuWindow;

	QElapsedTimer mElapsedTimer;
	double mDelta;
	bool mPaused;
	int mFrameCountSecond;
	int mFramesPerSecond;
	QFont mFont;
	bool mWireFrame;
	bool mMultiSample;
	bool mStereo;
	float mStereoEyeDistance;
	bool mStereoUseOVR;

#ifdef OVR_ENABLED
	Shader * mOVRShader;
	float mOVRLensViewportShift;
	float mOVRDistortionScale;
	OVR::DeviceManager * mOVRDeviceManager;
	OVR::HMDDevice * mOVRHMDDevice;
	OVR::SensorDevice * mOVRSensorDevice;
	OVR::SensorFusion mOVRSensorFusion;
	OVR::HMDInfo mOVRHMDInfo;
	float OVRLensViewportShift( const OVR::HMDInfo & hmdInfo ) const
	{
		float lensOffset = hmdInfo.LensSeparationDistance * 0.5f;
		float lensShift = hmdInfo.HScreenSize * 0.25f - lensOffset;
		return 4.0f * lensShift / hmdInfo.HScreenSize;
	}
	float OVRDistortionFn( const OVR::HMDInfo & hmdInfo, const float & r ) const
	{
		float rsq   = r * r;
		float scale = r * (hmdInfo.DistortionK[0]
			+ hmdInfo.DistortionK[1] * rsq
			+ hmdInfo.DistortionK[2] * rsq * rsq
			+ hmdInfo.DistortionK[3] * rsq * rsq * rsq);
		return scale;
	}
	float OVRDistortionScale( const OVR::HMDInfo & hmdInfo, const float & lensViewportShift, const float & aspect, const float & fitX, const float & fitY ) const
	{
		float dx = fitX - lensViewportShift;
		float dy = fitY / aspect;
		float fitRadius = sqrtf(dx * dx + dy * dy);
		return OVRDistortionFn( hmdInfo, fitRadius ) / fitRadius;
	}
#endif

	bool mMouseGrabbing;
	bool mBlinkingState;

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

public slots:
	void toggleMenu();

private slots:
	void secondPassed();
	void halfSecondPassed();
};


#endif
