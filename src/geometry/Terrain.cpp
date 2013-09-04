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

#include "Terrain.hpp"

#include <utility/Triangle.hpp>
#include <utility/Quaternion.hpp>

#include <QImage>
#include <QDebug>

#include <math.h>
#include <float.h>
#include <stddef.h>


Terrain::Terrain( const QString & heightMapPath, const QVector3D & size, const QVector3D & offset, const int & smoothingPasses )
{
	QImage heightMap( heightMapPath );
	if( heightMap.isNull() )
	{
		qFatal( "\"%s\" not found!", heightMapPath.toLocal8Bit().constData() );
	}
	mMapSize = heightMap.size();
	mSize = size;
	mOffset = offset;
	mToMapFactor = QSizeF( (float)mMapSize.width()/(float)mSize.x(), (float)mMapSize.height()/(float)mSize.z() );

	mVertices.resize( mMapSize.width() * mMapSize.height() );

	// prepare positions
	QVector<QVector3D> rawPositions;
	for( int h=0; h<mMapSize.height(); h++ )
	{
		for( int w=0; w<mMapSize.width(); w++ )
		{
			rawPositions.push_back( offset + QVector3D(
				w*(mSize.x()/mMapSize.width()),
				(float)qRed( heightMap.pixel( w, h ) )*(mSize.y()/256.0),
				h*(mSize.z()/mMapSize.height())
			) );
		}
	}

	// smoothed positions
	for( int i=0; i<smoothingPasses; i++ )
		rawPositions = smoothedPositions( rawPositions, mMapSize );

	// copy smoothed positions to vertex array
	for( int i=0; i<mVertices.size(); i++ )
	{
		mVertices[i].position = rawPositions[i];
	}

	// normals
	for( int h=0; h<mMapSize.height()-1; h++ )
	{
		for( int w=0; w<mMapSize.width()-1; w++ )
		{
			vertex( w, h ).normal = QVector3D::normal(
				getVertexPosition( w,   h ),
				getVertexPosition( w,   h+1 ),
				getVertexPosition( w+1, h )
			);
		}
		vertex( mMapSize.width()-1, h ).normal = QVector3D(0,1,0);	// last vertex in row
	}
	for( int w=0; w<mMapSize.width(); w++ )
	{
		vertex( w, mMapSize.height()-1 ).normal = QVector3D(0,1,0);	// last row
	}

	// texture coordinates
	for( int h=0; h<mMapSize.height(); h++ )
	{
		for( int w=0; w<mMapSize.width(); w++ )
		{
			vertex( w, h ).texCoord = QVector2D( w, h );
		}
	}
	mVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
	mVertexBuffer.create();
	mVertexBuffer.bind();
	mVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mVertexBuffer.allocate( mVertices.data(), mVertices.size()*VertexP3fN3fT2f::size() );
	mVertexBuffer.release();

	// indices
	QVector<unsigned int> indices;
	for( int h=0; h<mMapSize.height()-1; ++h )
	{
		for( int w=0; w<mMapSize.width(); ++w )
		{
			indices.push_back( w + h*(unsigned int)mMapSize.width() );
			indices.push_back( w + (h+1)*(unsigned int)mMapSize.width() );
		}
	}
	mIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
	mIndexBuffer.create();
	mIndexBuffer.bind();
	mIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mIndexBuffer.allocate( indices.data(), indices.size()*sizeof(unsigned int) );
	mIndexBuffer.release();
}


Terrain::~Terrain()
{
	mVertices.clear();
	mVertexBuffer.destroy();
	mIndexBuffer.destroy();
}


QVector< QVector3D > Terrain::smoothedPositions( const QVector< QVector3D > & in, const QSize & size )
{
	QVector< QVector3D > out( in.size() );

#define inPosition(x,y) (in[(x)+(y)*size.width()])
#define outPosition(x,y) (out[(x)+(y)*size.width()])
	for( int w=0; w<mMapSize.width(); w++ )
		outPosition( w, 0 ) = inPosition( w, 0 );
	for( int h=1; h<mMapSize.height()-1; h++ )
	{
		outPosition( 0, h ) = inPosition( 0, h );
		for( int w=1; w<mMapSize.width()-1; w++ )
		{
			QVector3D smoothed = QVector3D(0,0,0);
			smoothed += inPosition( w-1, h-1 );
			smoothed += inPosition( w  , h-1 );
			smoothed += inPosition( w+1, h-1 );
			smoothed += inPosition( w-1, h   );
			smoothed += inPosition( w  , h   )*4.0;
			smoothed += inPosition( w+1, h   );
			smoothed += inPosition( w-1, h+1 );
			smoothed += inPosition( w  , h+1 );
			smoothed += inPosition( w+1, h+1 );
			smoothed /= 12.0f;
			outPosition( w, h ) = smoothed;
		}
		outPosition( mMapSize.width()-1, h ) = inPosition( mMapSize.width()-1, h );
	}
	for( int w=0; w<mMapSize.width(); w++ )
		outPosition( w, mMapSize.height()-1 ) = inPosition( w,  mMapSize.height()-1 );
#undef inPosition
#undef outPosition
	return out;
}


void Terrain::drawPatchMap( const QRect & rect )
{
	QRect rectToDraw = rect.intersected( QRect( QPoint(0,0), QSize(mMapSize.width()-1,mMapSize.height()-1) ) );
	if( rectToDraw.width() < 1 || rectToDraw.height() < 1 )
		return;	// need at least 4 vertices to build triangle strip
	if( rectToDraw.y() >= mMapSize.height()-1 )
		return;	// reached the bottom row - there is no next row to build triangle strips with

	mVertexBuffer.bind();
	mIndexBuffer.bind();
	glEnableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glEnableClientState();
	VertexP3fN3fT2f::glPointerVBO();

	for( int slice=rectToDraw.y(); slice<rectToDraw.y()+rectToDraw.height(); slice++ )
	{
		glDrawElements(
			GL_TRIANGLE_STRIP,
			rectToDraw.width()*2+2,
			GL_UNSIGNED_INT,
			(const GLvoid*)((size_t)(2*sizeof(unsigned int)*(	// convert index to pointer
				rectToDraw.x() + mMapSize.width()*slice		// index to start
			) ) )
		);
		/*
		//TODO: something like this should be faster
		glDrawRangeElements(
			GL_TRIANGLE_STRIP,
			slice*(unsigned int)mMapSize.width()+rectToDraw.x(),
			(slice+1)*(unsigned int)mMapSize.width()+rectToDraw.x()+rectToDraw.width()+2,
			rectToDraw.width()*2+2,
			GL_UNSIGNED_INT,
			(const GLvoid*)((size_t)(2*sizeof(unsigned int)*(	// convert index to pointer
				rectToDraw.x() + mMapSize.width()*slice		// index to start
			) ) )
		);
		*/
	}

	glDisableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glDisableClientState();

	mVertexBuffer.release();
	mIndexBuffer.release();
}


void Terrain::draw()
{
	mVertexBuffer.bind();
	mIndexBuffer.bind();
	glEnableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glEnableClientState();
	VertexP3fN3fT2f::glPointerVBO();

	for( int slice=0; slice<mMapSize.height()-1; slice++ )
	{
		glDrawElements(
			GL_TRIANGLE_STRIP,
			mMapSize.width()*2,
			GL_UNSIGNED_INT,
			(const GLvoid*)((size_t)(2*sizeof(unsigned int)*(	// convert index to pointer
				mMapSize.width()*slice			// index to start
			) ) )
		);
	}

	glDisableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glDisableClientState();
	mVertexBuffer.release();
	mIndexBuffer.release();
}


bool Terrain::getTriangle( const QPointF & position, Triangle & t ) const
{
	QPoint pos = QPoint( position.x(), position.y() );

	if( pos.x() >= mMapSize.width()-1 || pos.y() >= mMapSize.height()-1 || pos.x() < 0 || pos.y() < 0 )
		return false;

	QPointF fraction = QPointF( position.x()-(float)pos.x(), position.y()-(float)pos.y() );

	if( fraction.x() + fraction.y() < 1.0f )
	{
		t.setP( getVertexPosition( pos.x(), pos.y() ) );
		t.setQ( getVertexPosition( pos.x(), pos.y()+1 ) );
		t.setR( getVertexPosition( pos.x()+1, pos.y() ) );
	} else {
		t.setP( getVertexPosition( pos.x()+1, pos.y()+1 ) );
		t.setQ( getVertexPosition( pos.x()+1, pos.y() ) );
		t.setR( getVertexPosition( pos.x(), pos.y()+1 ) );
	}

	return true;
}


Triangle Terrain::getTriangle( const QPointF & position ) const
{
	QPoint pos = QPoint( position.x(), position.y() );

	if( pos.x() >= mMapSize.width()-1 )
		pos.setX( mMapSize.width()-2 );
	if( pos.y() >= mMapSize.height()-1 )
		pos.setY( mMapSize.height()-2 );
	if( pos.x() < 0 )
		pos.setX( 0 );
	if( pos.y() < 0 )
		pos.setY( 0 );

	QPointF fraction = QPointF( position.x()-(float)pos.x(), position.y()-(float)pos.y() );
	if( fraction.x() + fraction.y() < 1.0f )
	{
		return Triangle(
			getVertexPosition( pos.x(), pos.y() ),
			getVertexPosition( pos.x(), pos.y()+1 ),
			getVertexPosition( pos.x()+1, pos.y() )
		);
	}
	return Triangle(
		getVertexPosition( pos.x()+1, pos.y()+1 ),
		getVertexPosition( pos.x()+1, pos.y() ),
		getVertexPosition( pos.x(), pos.y()+1 )
	);
}


bool Terrain::getHeight( const QPointF & position, float & height ) const
{
	Triangle t;
	if( !getTriangle( toMapF(position), t ) )
		return false;
	QVector3D normal = t.normal();
	height = ( (normal.x()*(position.x()-t.p().x())) + (normal.z()*(position.y()-t.p().z())) ) / ( -normal.y() ) + t.p().y();
	return true;
}


float Terrain::getHeight( const QPointF & position ) const
{
	Triangle t = getTriangle( toMapF(position) );
	QVector3D normal = t.normal();
	return ( (normal.x()*(position.x()-t.p().x())) + (normal.z()*(position.y()-t.p().z())) ) / ( -normal.y() ) + t.p().y();
}


bool Terrain::getNormal( const QPointF & position, QVector3D & normal ) const
{
	Triangle t;
	if( !getTriangle( toMapF(position), t ) )
		return false;
	normal = t.normal();
	return true;
}


QVector3D Terrain::getNormal( const QPointF & position ) const
{
	Triangle t = getTriangle( toMapF(position) );
	return t.normal();
}


QQuaternion Terrain::getNormalRotation( const QPointF & position, const QVector3D & from ) const
{
	QVector3D normal = getNormal( position );
	return Quaternion::fromTo( from, normal );
}


bool Terrain::getLineQuadIntersection( const QVector3D & origin, const QVector3D & direction, const QPoint & quadMapCoord, float & length ) const
{
	QPoint pos = quadMapCoord;
	if( pos.x() >= mMapSize.width()-1 )
		pos.setX( mMapSize.width()-2 );
	if( pos.y() >= mMapSize.height()-1 )
		pos.setY( mMapSize.height()-2 );
	if( pos.x() < 0 )
		pos.setX( 0 );
	if( pos.y() < 0 )
		pos.setY( 0 );

	float intersectionDistance;

	Triangle t1(
		getVertexPosition( pos.x(), pos.y() ),
		getVertexPosition( pos.x(), pos.y()+1 ),
		getVertexPosition( pos.x()+1, pos.y() )
	);

	if( t1.intersectRay( origin, direction, &intersectionDistance ) )
	{
		if( intersectionDistance < length && intersectionDistance > 0.0f )
		{
			length = intersectionDistance;
			return true;
		}
	}

	Triangle t2(
		getVertexPosition( pos.x()+1, pos.y()+1 ),
		getVertexPosition( pos.x()+1, pos.y() ),
		getVertexPosition( pos.x(), pos.y()+1 )
	);

	if( t2.intersectRay( origin, direction, &intersectionDistance ) )
	{
		if( intersectionDistance < length && intersectionDistance > 0.0f )
		{
			length = intersectionDistance;
			return true;
		}
	}

	return false;
}


bool Terrain::intersectLine( const QVector3D & origin, const QVector3D & direction, float & length, QVector3D * normal ) const
{
	QPoint mapFrom = toMap( origin );
	QPoint mapTo = toMap( origin + direction * length );

	int x = mapFrom.x();
	int y = mapFrom.y();
	int dx = abs( mapTo.x() - x );
	int dy = abs( mapTo.y() - y );
	int sx = ( x < mapTo.x() ) ? 1 : -1;
	int sy = ( y < mapTo.y() ) ? 1 : -1;
	int error = dx - dy;

	int ox = 1;
	int oy = 1;
	if( dx < dy )
		oy = 0;
	else
		ox = 0;

	while( true )
	{
		bool intersects = false;
		intersects |= getLineQuadIntersection( origin, direction, QPoint(x, y), length );
		intersects |= getLineQuadIntersection( origin, direction, QPoint(x+ox, y+oy), length );
		intersects |= getLineQuadIntersection( origin, direction, QPoint(x-ox, y-oy), length );
		if( intersects )
		{
			if( normal )
				*normal = getNormal( origin + direction*length );
			return true;
		}

		if( x == mapTo.x() && y == mapTo.y() )
			break;

		int error2 = 2 * error;
		if( error2 > -dy )
		{
			error -= dy;
			x += sx;
		}
		if( error2 < dx )
		{
			error += dx;
			y += sy;
		}
	}
	return false;
}
