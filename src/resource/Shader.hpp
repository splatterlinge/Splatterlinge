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

#ifndef RESOURCE_SHADER_INCLUDED
#define RESOURCE_SHADER_INCLUDED


#include "AResource.hpp"

#include <GLWidget.hpp>

#include <QDebug>


class QGLShaderProgram;


/// Compiled shader program data
class ShaderData : public AResourceData
{
public:
	ShaderData( GLWidget * glWidget, QString name );
	virtual ~ShaderData();

	QGLShaderProgram * program() { return mProgram; }
	const QGLShaderProgram * program() const { return mProgram; }

	const QString & name() const { return mName; }

	// Overrides:
	virtual bool load();
	virtual void unload();

	static QString baseDirectory() { return AResourceData::baseDirectory()+"shader/"; }

private:
	GLWidget * mGLWidget;
	QString mName;

	QGLShaderProgram * mProgram;
};


/// Shader program
class Shader : public AResource<ShaderData>
{
public:
	Shader( GLWidget * glWidget, QString name );
	virtual ~Shader();

	QGLShaderProgram * program() { return data()->program(); }

	void bind();
	void release();
};


#endif
