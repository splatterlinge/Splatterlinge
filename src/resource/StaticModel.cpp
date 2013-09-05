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

#include "StaticModel.hpp"

#include <scene/object/AObject.hpp>

#include <QDebug>
#include <QVector3D>
#include <float.h>


Face::Face( QStringList &fields, QString &material, QVector<QVector3D> *positions, QVector<QVector2D> *texCoords, QVector<QVector3D> *normals )
{
	VertexP3fN3fT2f vertex;
	QStringList points;

	while( !fields.isEmpty() )
	{
		points = fields.takeFirst().split( '/' );

		if( !points.isEmpty() )
		{
			vertex.position = positions->at( points.takeFirst().toInt()-1 );
		}
		if( !points.isEmpty() )
		{
			vertex.texCoord = texCoords->at( points.takeFirst().toInt()-1 );
		}
		if( !points.isEmpty() )
		{
			vertex.normal = normals->at( points.takeFirst().toInt()-1 );
		}

		this->points.append( vertex );
		this->material = material;
	}
}


Part::Part( unsigned int current, unsigned int count, GLWidget * widget, QString & material )
{
	this->start = current - count;
	this->count = count;
	if( !material.isEmpty() )
		this->material = new Material( widget, material );
	else
		this->material = NULL;
}


RESOURCE_CACHE(StaticModelData);


StaticModelData::StaticModelData( GLWidget * glWidget, QString name ) :
	AResourceData( name ),
	mGLWidget( glWidget ),
	mName( name )
{
	mMode = 0;
}


StaticModelData::~StaticModelData()
{
	unload();
}


void StaticModelData::unload()
{
	if( !loaded() )
		return;
	qDebug() << "-" << this << "StaticModelData" << uid();

	foreach( const Part & part, mParts )
	{
		if( part.material )
		{
			delete part.material;
		}
	}

	mParts.clear();
	mVertices.clear();
	mIndices.clear();

	mVertexBuffer.release();
	mVertexBuffer.destroy();

	mIndexBuffer.release();
	mIndexBuffer.destroy();

	AResourceData::unload();
}


bool StaticModelData::load()
{
	unload();
	qDebug() << "+" << this << "StaticModelData" << uid();

	parse();

	return AResourceData::load();
}


bool StaticModelData::parse()
{
	QFile file( baseDirectory()+mName+'/'+mName+".obj" );
	QString line;
	QString keyword;
	QStringList fields;
	QVector<Face> faces;
	QVector<QVector3D> positions;
	QVector<QVector2D> texCoords;
	QVector<QVector3D> normals;
	QString material;

	if( !file.open( QIODevice::ReadOnly ) ) {
		qCritical() << "!!" << this << "StaticModelData" << uid() << "Could not open "<< file.fileName() << ": " << file.errorString();
		return false;
	}

	QTextStream in( &file );

	while( !in.atEnd() ) {
		line = in.readLine().trimmed();

		while( line.endsWith( '\\' ) )
		{
			line.truncate( line.size()-1 );
			if( in.atEnd() )
			{
				break;
			}
			line += in.readLine().trimmed();
		}

		if( line.startsWith( '#' ) || line.isEmpty() )
		{
			continue;
		}

		fields = line.split( ' ', QString::SkipEmptyParts );
		keyword = fields.takeFirst();

		if( keyword == "g" )
		{
			continue;
		}
		else if( keyword == "s" )
		{
			continue;
		}
		else if( keyword == "v" )
		{
			positions.append( Vector::V3f( fields ) );
		}
		else if( keyword == "vt" )
		{
			texCoords.append( Vector::V2f( fields ) );
		}
		else if( keyword == "vn" )
		{
			normals.append( Vector::V3f( fields ) );
		}
		else if( keyword == "f" )
		{
			faces.append( Face( fields, material, &positions, &texCoords, &normals ) );
		}
		else if( keyword == "mtllib" )
		{
			continue;
		}
		else if( keyword == "usemtl" )
		{
			material = generateMaterialName( file, fields );
		}
		else
		{
			qWarning() << "!" << this << "StaticModelData" << uid() << "Unknown keyword" << fields.takeFirst() << "detected.";
		}
	}

	file.close();

	generateParts( & faces );
	generateBuffers();

	return true;
}


void StaticModelData::generateBuffers()
{
	mVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
	mVertexBuffer.create();
	mVertexBuffer.bind();
	mVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mVertexBuffer.allocate( mVertices.constData(), mVertices.size() * sizeof( VertexP3fN3fT2f ) );
	mVertexBuffer.release();

	mIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
	mIndexBuffer.create();
	mIndexBuffer.bind();
	mIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mIndexBuffer.allocate( mIndices.constData(), mIndices.size() * sizeof( unsigned int ) );
	mIndexBuffer.release();
}


void StaticModelData::generateParts( QVector<Face> * faces)
{
	mParts.clear();
	QString lastMat;
	unsigned int current = 0;
	unsigned int count = 0;

	QVector<Face>::ConstIterator iter = faces->constBegin();
	for( ; iter != faces->constEnd(); ++iter )
	{
		GLuint mode = 0;
		int size = (*iter).points.size();

		switch( size )
		{
			case 3:
				mode = GL_TRIANGLES;
				break;
			case 4:
				mode = GL_QUADS;
				break;
			default:
				qCritical() << "!!" << this << "StaticModelData" << uid() << "Only 3 or 4 vertices per face are supported!";
				break;
		}

		if( mMode == 0 )
			mMode = mode;
		else if( mMode != mode )
			qCritical() << "!!" << this << "StaticModelData" << uid() << "Switching between different counts of vertices per face is unsupported!" ;

		if( (*iter).material != lastMat || iter+1 == faces->constEnd() )
		{
			mParts.append( Part( current, count, mGLWidget, lastMat ) );

			lastMat = (*iter).material;
			count = 0;
		}

		foreach( const VertexP3fN3fT2f & vertex, (*iter).points )
		{
			if( mVertices.indexOf( vertex ) == -1 )
			{
				mVertices.append( vertex );
			}
			mIndices.append( mVertices.indexOf( vertex ) );

			current++;
			count++;
		}
	}
}


QString StaticModelData::generateMaterialName( QFile & file, QStringList & fields )
{
	QFileInfo fileinfo( file );
	QFileInfo mat( MaterialData::baseDirectory()+fileinfo.baseName()+'_'+fields.takeFirst() );
	if( mat.exists() )
	{
		return mat.fileName();
	}
	else
	{
		qWarning() << "!" << this << "StaticModelData" << uid() << "Material" << mat.fileName() << "not found.";
		return "";
	}
}


StaticModel::StaticModel( Scene * scene, QString name ) :
	AResource(),
	mScene( scene )
{
	QSharedPointer<StaticModelData> n( new StaticModelData( scene->glWidget(), name ) );
	cache( n );
}


StaticModel::~StaticModel()
{
}

void StaticModel::draw( const QVector<QMatrix4x4> & instances )
{
	data()->vertexBuffer().bind();
	data()->indexBuffer().bind();

	glEnableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glEnableClientState();
	VertexP3fN3fT2f::glPointerVBO();

	glPushMatrix();

	foreach( const Part & part, data()->parts() )
	{
		if( part.material )
		{
			part.material->bind();
		}

		foreach( const QMatrix4x4 & instance, instances )
		{
			glLoadMatrix( mScene->eye()->viewMatrix() * instance );

			glDrawElements(
				data()->mode(),
				part.count,
				GL_UNSIGNED_INT,
				(void*)((size_t)(sizeof(unsigned int)*(	// convert index to pointer
					part.start		// index to start
				) ) )
			);
		}

		if( part.material )
		{
			part.material->release();
		}
	}

	glPopMatrix();

	glDisableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glDisableClientState();

	data()->vertexBuffer().release();
	data()->indexBuffer().release();
}


void StaticModel::draw()
{
	data()->vertexBuffer().bind();
	data()->indexBuffer().bind();

	glEnableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glEnableClientState();
	VertexP3fN3fT2f::glPointerVBO();

	glPushMatrix();

	foreach( const Part & part, data()->parts() )
	{
		if( part.material )
		{
			part.material->bind();
		}

		glDrawElements(
			GL_TRIANGLES,
			part.count,
			GL_UNSIGNED_INT,
			(void*)((size_t)(sizeof(unsigned int)*(	// convert index to pointer
				part.start		// index to start
			) ) )
		);

		if( part.material )
		{
			part.material->release();
		}
	}

	glPopMatrix();

	glDisableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glDisableClientState();

	data()->vertexBuffer().release();
	data()->indexBuffer().release();
}
