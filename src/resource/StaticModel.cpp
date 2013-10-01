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
		qDebug() << points;

		if( !points.isEmpty() )
		{
			int pos = points.takeFirst().toInt()-1;
			if( pos != -1 )
				vertex.position = positions->at( pos );
		}
		if( !points.isEmpty() )
		{
			int pos = points.takeFirst().toInt()-1;
			if( pos != -1 )
				vertex.texCoord = texCoords->at( pos );
		}
		if( !points.isEmpty() )
		{
			int pos = points.takeFirst().toInt()-1;
			if( pos != -1 )
				vertex.normal = normals->at( pos );
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

		while( line.endsWith( OBJ_LINESPLIT ) )
		{
			line.truncate( line.size()-1 );
			if( in.atEnd() )
			{
				break;
			}
			line += in.readLine().trimmed();
		}

		if( line.startsWith( OBJ_COMMENT ) || line.isEmpty() )
		{
			continue;
		}

		fields = line.split( ' ', QString::SkipEmptyParts );
		keyword = fields.takeFirst();

		if( keyword == OBJ_GROUP )
		{
			continue;
		}
		else if( keyword == OBJ_SMOOTHING )
		{
			continue;
		}
		else if( keyword == OBJ_VERTEX )
		{
			positions.append( Vector::takeFirstFromStringList3D( fields ) );
		}
		else if( keyword == OBJ_VERTEX_TEXCOORD )
		{
			texCoords.append( Vector::takeFirstFromStringList2D( fields ) );
		}
		else if( keyword == OBJ_VERTEX_NORMAL )
		{
			normals.append( Vector::takeFirstFromStringList3D( fields ) );
		}
		else if( keyword == OBJ_FACE )
		{
			faces.append( Face( fields, material, &positions, &texCoords, &normals ) );
		}
		else if( keyword == OBJ_MATERIAL_LIB )
		{
			continue;
		}
		else if( keyword == OBJ_MATERIAL_USE )
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

	int faceCount = 0;
	foreach( const Face & face, *faces )
	{
		GLuint mode = 0;
		int size = face.points.size();

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

		if( face.material != lastMat )
		{
			mParts.append( Part( current, count, mGLWidget, lastMat ) );

			lastMat = face.material;
			count = 0;
		}

		foreach( const VertexP3fN3fT2f & vertex, face.points )
		{
			if( mVertices.indexOf( vertex ) == -1 )
			{
				mVertices.append( vertex );
			}
			mIndices.append( mVertices.indexOf( vertex ) );

			current++;
			count++;
		}

		faceCount++;
	}

	mParts.append( Part( current, count, mGLWidget, faces->last().material ) );
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


StaticModel::StaticModel( GLWidget * glWidget, QString name ) :
	AResource()
{
	QSharedPointer<StaticModelData> n( new StaticModelData( glWidget, name ) );
	cache( n );
}


StaticModel::~StaticModel()
{
}

void StaticModel::draw( const QMatrix4x4 & viewMatrix, const QVector<QMatrix4x4> & instances )
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
			glLoadMatrix( viewMatrix * instance );

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
