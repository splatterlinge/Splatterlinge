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

#include "Shader.hpp"

#include <GLWidget.hpp>

#include <QGLShaderProgram>
#include <QDebug>


RESOURCE_CACHE(ShaderData);


ShaderData::ShaderData( GLWidget * glWidget, QString name ) :
	AResourceData( name ),
	mGLWidget(glWidget),
	mName(name),
	mProgram(0)
{
}


ShaderData::~ShaderData()
{
	unload();
}


void ShaderData::unload()
{
	if( !loaded() )
		return;
	qDebug() << "-" << this << "ShaderData" << uid();

	delete mProgram;
	mProgram = 0;

	AResourceData::unload();
}


bool ShaderData::load()
{
	unload();
	qDebug() << "+" << this << "ShaderData" << uid();

	mProgram = new QGLShaderProgram( mGLWidget );
	mProgram->addShaderFromSourceFile( QGLShader::Vertex, baseDirectory()+mName+".vert" );
	mProgram->addShaderFromSourceFile( QGLShader::Fragment, baseDirectory()+mName+".frag" );
	if( !mProgram->link() )
	{
		qWarning() << mProgram->log();
		return false;
	}

	return AResourceData::load();
}


Shader::Shader( GLWidget * glWidget, QString name ) : AResource()
{
	QSharedPointer<ShaderData> n( new ShaderData( glWidget, name ) );
	cache( n );
}


Shader::~Shader()
{
}


void Shader::bind()
{
	data()->program()->bind();
}


void Shader::release()
{
	data()->program()->release();
}
