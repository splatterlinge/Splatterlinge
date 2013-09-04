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

#ifndef GLVIEW_INCLUDED
#define GLVIEW_INCLUDED

#include <utility/glWrappers.hpp>
#include <utility/alWrappers.hpp>

#include <QGraphicsView>


class QWidget;
class QGraphicsScene;
class QResizeEvent;

class StartMenuWindow;
class DebugWindow;
class HelpWindow;
class GfxOptionWindow;
class Scene;
class GLWidget;
class World;


/// View on a scene
class View : public QGraphicsView
{
	Q_OBJECT
public:
	View( QWidget * parent = 0 );
	virtual ~View();

	GLWidget * glWidget() const { return mGLWidget; }

	// Overrides:
	virtual Scene * scene() const { return mScene; }

protected:
	// Overrides:
	virtual void resizeEvent( QResizeEvent * event );

private:
	GLWidget * mGLWidget;
	Scene * mScene;
	World * mWorld;

	ALCdevice * mALDevice;
	ALCcontext * mALContext;

	void initGL();
	void initAL();
	void initScene();
};


#endif
