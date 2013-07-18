#include "Terrain.hpp"

#include <utility/Triangle.hpp>

#include <QImage>
#include <QDebug>

#include <math.h>
#include <float.h>
#include <stddef.h>


Terrain::Terrain( QString heightMapPath, QVector3D size, QVector3D offset )
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
#define rawPosition(x,y) (rawPositions[(x)+(y)*mMapSize.width()])
	for( int w=0; w<mMapSize.width(); w++ )
		vertex( w, 0 ).position = rawPosition( w, 0 );
	for( int h=1; h<mMapSize.height()-1; h++ )
	{
		vertex( 0, h ).position = rawPosition( 0, h );
		for( int w=1; w<mMapSize.width()-1; w++ )
		{
			QVector3D smoothed = QVector3D(0,0,0);
			smoothed += rawPosition( w-1, h-1 );
			smoothed += rawPosition( w  , h-1 );
			smoothed += rawPosition( w+1, h-1 );
			smoothed += rawPosition( w-1, h   );
			smoothed += rawPosition( w  , h   );
			smoothed += rawPosition( w+1, h   );
			smoothed += rawPosition( w-1, h+1 );
			smoothed += rawPosition( w  , h+1 );
			smoothed += rawPosition( w+1, h+1 );
			smoothed /= 9.0f;
			vertex( w, h ).position = smoothed;
		}
		vertex( mMapSize.width()-1, h ).position = rawPosition( mMapSize.width()-1, h );
	}
	for( int w=0; w<mMapSize.width(); w++ )
		vertex( w, mMapSize.height()-1 ).position = rawPosition( w,  mMapSize.height()-1 );
#undef rawPosition
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
	mVertexBuffer.allocate( mVertices.data(), mVertices.size()*sizeof(Vertex) );
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


void Terrain::drawPatchMap( const QRect & rect )
{
	QRect rectToDraw = rect.intersected( QRect( QPoint(0,0), QSize(mMapSize.width()-1,mMapSize.height()-1) ) );
	if( rectToDraw.width() < 1 || rectToDraw.height() < 1 )
		return;	// need at least 4 vertices to build triangle strip
	if( rectToDraw.y() >= mMapSize.height()-1 )
		return;	// reached the bottom row - there is no next row to build triangle strips with

	mVertexBuffer.bind();
	mIndexBuffer.bind();
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_INDEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex,position) );
	glNormalPointer( GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex,normal) );
	glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex,texCoord) );

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

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	mVertexBuffer.release();
	mIndexBuffer.release();
}


void Terrain::draw()
{
	mVertexBuffer.bind();
	mIndexBuffer.bind();
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_INDEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex,position) );
	glNormalPointer( GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex,normal) );
	glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex,texCoord) );

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

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
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
	QVector3D axis = QVector3D::crossProduct( from, normal );
	float angle = acosf( QVector3D::dotProduct( from, normal ) ) * (180.0/M_PI);
	return QQuaternion::fromAxisAndAngle( axis, angle );
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
		if( intersectionDistance < length )
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
		if( intersectionDistance < length )
		{
			length = intersectionDistance;
			return true;
		}
	}

	return false;
}


bool Terrain::getLineIntersection( const QVector3D & origin, const QVector3D & direction, float & length ) const
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


bool Terrain::getLineIntersection( const QVector3D & origin, const QVector3D & direction, float & length, QVector3D & normal ) const
{
	if( !getLineIntersection( origin, direction, length ) )
		return false;
	normal = getNormal( origin + direction*length );
	return true;
}
