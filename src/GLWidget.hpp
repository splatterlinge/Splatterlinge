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

#ifndef GLWIDGET_INCLUDED
#define GLWIDGET_INCLUDED

#include <GL/glew.h>
#include <GL/glu.h>

#include <QGLWidget>

#include "utility/glWrappers.hpp"


class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	GLWidget( QWidget * parent = 0 ) :
		QGLWidget( parent )
		{ makeCurrent(); glewInit(); }

	GLWidget( QGLContext * context, QWidget * parent = 0 ) :
		QGLWidget( context, parent )
		{ makeCurrent(); glewInit(); }

	GLWidget( QGLFormat & format, QWidget * parent = 0 ) :
		QGLWidget( format, parent )
		{ makeCurrent(); glewInit(); }
};


#endif
