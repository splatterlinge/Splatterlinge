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

#include "StaticModel.hpp"

#include <scene/object/AObject.hpp>

#include <QDebug>
#include <QVector3D>
#include <float.h>


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

	foreach( Part part, mParts )
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
	QFile file( mName );
	QString line;
	QString keyword;
	QStringList fields;
	QVector<Face> faces;
	QVector<QVector3D> positions;
	QVector<QVector2D> texCoords;
	QVector<QVector3D> normals;
	QString material;
	float x, y, z;
	float u, v;

	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;

	if( !file.open( QIODevice::ReadOnly ) ) {
		qDebug() << file.errorString();
		return false;
	}

	QTextStream in( &file );

	while( !in.atEnd() ) {
		line = in.readLine().trimmed();

		while( line.endsWith( "\\" ) )
		{
			line.truncate( line.size()-1 );
			if( in.atEnd() )
			{
				break;
			}
			line += in.readLine().trimmed();
		}

		if( line.startsWith( "#" ) || line.isEmpty() )
		{
			continue;
		}

		fields = line.split( " ", QString::SkipEmptyParts );
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
			x = fields.takeFirst().toFloat();
			if( x > maxX )
				maxX = x;
			if( x < minX )
				minX = x;

			y = fields.takeFirst().toFloat();
			if( y > maxY )
				maxY = y;
			if( y < minY )
				minY = y;

			z = fields.takeFirst().toFloat();
			positions.append( QVector3D( x, y, z ) );
		}
		else if( keyword == "vt" )
		{
			u = fields.takeFirst().toFloat();
			v = fields.takeFirst().toFloat();
			texCoords.append( QVector2D( u, v ) );
		}
		else if( keyword == "vn" )
		{
			x = fields.takeFirst().toFloat();
			y = fields.takeFirst().toFloat();
			z = fields.takeFirst().toFloat();
			normals.append( QVector3D( x, y, z ) );
		}
		else if( keyword == "f" )
		{
			Face face;
			VertexP3fN3fT2f vertex;
			QStringList points;

			while( !fields.isEmpty() )
			{
				points = fields.takeFirst().split( "/" );

				if( !points.isEmpty() )
				{
					vertex.position = positions.at( points.takeFirst().toInt()-1 );
				}
				if( !points.isEmpty() )
				{
					vertex.texCoord = texCoords.at( points.takeFirst().toInt()-1 );
				}
				if( !points.isEmpty() )
				{
					vertex.normal = normals.at( points.takeFirst().toInt()-1 );
				}

				face.points.append( vertex );
				face.material = material;
			}
			faces.append( face );
		}
		else if( keyword == "mtllib" )
		{
			continue;
		}
		else if( keyword == "usemtl" )
		{
			QFileInfo fileinfo( file );
			QFileInfo mat( "./data/material/"+fileinfo.baseName()+"_"+fields.takeFirst() );
			if( mat.exists() )
			{
				material = mat.fileName();
			}
			else
			{
				qWarning() << "?" << this << "Material" << mat.fileName() << "not found.";
			}
		}
		else
		{
			qWarning() << "?" << this << "Unknown keyword" << fields.takeFirst() << "detected.";
		}
	}

	file.close();

	mSize.setWidth(maxX-minX);
	mSize.setHeight(maxY-minY);

	mParts.clear();
	QString lastMat = faces.first().material;
	unsigned int current = 0;
	unsigned int count = 0;

	foreach( Face face, faces )
	{
		int mode = 0;
		int size = face.points.size();

		if( size == 3 )
			mode = GL_TRIANGLES;
		else if( size == 4 )
			mode = GL_QUADS;
		else
			qCritical() << "!" << this << "Invalid number of points detected! Please check your OBJ file.";

		if( mMode == 0 )
		{
			mMode = mode;
		}
		else
		{
			if( mMode == 0 )
			{
				mMode = mode;
			}
			else if( mMode != mode )
			{
				qCritical() << "!" << this << "Different draw modes detected! Please check your OBJ file." ;
			}
		}

		if( face.material != lastMat || face == faces.last() )
		{
			Part p;
			p.start = current - count;
			p.count = count;
			p.material = new Material( mGLWidget, lastMat );
			mParts.append(p);

			lastMat = face.material;
			count = 0;
		}

		foreach( VertexP3fN3fT2f vertex, face.points )
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

	return true;
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

	foreach( Part part, data()->parts() )
	{
		if( part.material )
		{
			part.material->bind();
		}

		foreach( QMatrix4x4 instance, instances )
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

	foreach( Part part, data()->parts() )
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
