#include "Terrain.hpp"

#include <QImage>
#include <QDebug>


Terrain::Terrain( QString heightMapPath, QVector3D size, QVector3D offset )
{
	QImage heightMap( heightMapPath );
	mMapSize = heightMap.size();
	mSize = size;
	mOffset = offset;
	mVertices.clear();

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
		mVertices.push_back( rawPosition( w, 0 ) );
	for( int h=1; h<mMapSize.height()-1; h++ )
	{
		mVertices.push_back( rawPosition( 0, h ) );
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
			mVertices.push_back( smoothed );
		}
		mVertices.push_back( rawPosition( mMapSize.width()-1, h ) );
	}
	for( int w=0; w<mMapSize.width(); w++ )
		mVertices.push_back( rawPosition( w, mMapSize.height()-1 ) );
#undef rawPosition
	// normals
	for( int h=0; h<mMapSize.height()-1; h++ )
	{
		for( int w=0; w<mMapSize.width()-1; w++ )
		{
			mVertices.push_back( QVector3D::normal(
				getVertexPosition(w,h),
				getVertexPosition(w,h+1),
				getVertexPosition(w+1,h)
			) );
		}
		mVertices.push_back( QVector3D(0,1,0) );	// last vertex in row
	}
	for( int w=0; w<mMapSize.width(); w++ )	// last row
	{
		mVertices.push_back( QVector3D(0,1,0) );
	}
	// texture coordinates
	for( int h=0; h<mMapSize.height(); h++ )
	{
		for( int w=0; w<mMapSize.width(); w++ )
		{
			mVertices.push_back( QVector3D( (float)w, (float)h, 0 ) );
		}
	}
	mVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
	mVertexBuffer.create();
	mVertexBuffer.bind();
	mVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mVertexBuffer.allocate( mVertices.data(), mVertices.size()*sizeof(QVector3D) );

	// indices
	QVector<unsigned int> indices;
	for( int h=0; h<mMapSize.height()-1; h++ )
	{
		for( int w=0; w<mMapSize.width(); w++ )
		{
			indices.push_back( w + h*mMapSize.width() );
			indices.push_back( w + (h+1)*mMapSize.width() );
		}
	}
	mIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
	mIndexBuffer.create();
	mIndexBuffer.bind();
	mIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mIndexBuffer.allocate( indices.data(), indices.size()*sizeof(unsigned int) );
}


Terrain::~Terrain()
{
	mVertices.clear();
	mVertexBuffer.destroy();
	mIndexBuffer.destroy();
}


QPointF Terrain::toMapF( const QVector3D & point ) const
{
	return QPointF(
		(point.x()-(float)mOffset.x()) * ((float)mMapSize.width()/(float)mSize.x()),
		(point.z()-(float)mOffset.z()) * ((float)mMapSize.height()/(float)mSize.z())
	);
}


QPoint Terrain::toMap( const QVector3D & point ) const
{
	return QPoint(
		(point.x()-mOffset.x()) * (mMapSize.width()/mSize.x()),
		(point.z()-mOffset.z()) * (mMapSize.height()/mSize.z())
	);
}


QPoint Terrain::toMap( const QPointF & point ) const
{
	return QPoint(
		(point.x()-mOffset.x()) * (mMapSize.width()/mSize.x()),
		(point.y()-mOffset.z()) * (mMapSize.height()/mSize.z())
	);
}


QSize Terrain::toMap( const QSizeF & size ) const
{
	return QSize(
		size.width() * (mMapSize.width()/mSize.x()),
		size.height() * (mMapSize.height()/mSize.z())
	);
}


QRect Terrain::toMap( const QRectF & rect ) const
{
	return QRect(
		toMap(rect.topLeft()),
		toMap(rect.size())
	);
}


void Terrain::drawPatchMap( const QRect & rect )
{
	QRect rectToDraw = rect.intersected( QRect( QPoint(0,0), mMapSize ) );
	if( rectToDraw.width() <= 1 || rectToDraw.height() <= 1 )
		return;	// need at least 4 vertices to build triangle strip
	if( rectToDraw.y() >= mMapSize.height()-1 )
		return;	// reached the bottom row - there is no next row to build triangle strips with

	mVertexBuffer.bind();
	mIndexBuffer.bind();
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_INDEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	
	glVertexPointer( 3, GL_FLOAT, 0, 0 );
	glNormalPointer( GL_FLOAT, 0, (void*)((size_t)mMapSize.width()*mMapSize.height()*sizeof(QVector3D)) );
	glTexCoordPointer( 3, GL_FLOAT, 0, (void*)((size_t)mMapSize.width()*mMapSize.height()*sizeof(QVector3D)*2) );

	for( int slice=rectToDraw.y(); slice<rectToDraw.y()+rectToDraw.height(); slice++ )
	{
		glDrawElements(
			GL_TRIANGLE_STRIP,
			rectToDraw.width()*2,
			GL_UNSIGNED_INT,
			(void*)((size_t)(2*sizeof(unsigned int)*(	// convert index to pointer
				rectToDraw.x() + mMapSize.width()*slice	// index to start
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


void Terrain::draw()
{
	mVertexBuffer.bind();
	mIndexBuffer.bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer( 3, GL_FLOAT, 0, 0 );
	glNormalPointer( GL_FLOAT, 0, (void*)((size_t)mMapSize.width()*mMapSize.height()*sizeof(QVector3D)) );
	glTexCoordPointer( 3, GL_FLOAT, 0, (void*)((size_t)mMapSize.width()*mMapSize.height()*sizeof(QVector3D)*2) );

	for( int slice=0; slice<mMapSize.height()-1; slice++ )
	{
		glDrawElements(
			GL_TRIANGLE_STRIP,
			mMapSize.width()*2,
			GL_UNSIGNED_INT,
			(void*)((size_t)(2*sizeof(unsigned int)*(	// convert index to pointer
				mMapSize.width()*slice			// index to start
			) ) )
		);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	mVertexBuffer.release();
	mIndexBuffer.release();
}


float Terrain::getHeight( const QVector3D & position )
{
	QPointF posF = toMapF( position );
	QPoint pos = QPoint( posF.x(), posF.y() );

	if( pos.x() >= mMapSize.width()-1 )
		pos.setX( mMapSize.width()-2 );
	if( pos.y() >= mMapSize.height()-1 )
		pos.setY( mMapSize.height()-2 );
	if( pos.x() < 0 )
		pos.setX( 0 );
	if( pos.y() < 0 )
		pos.setY( 0 );

	posF = QPointF( posF.x()-(float)pos.x(), posF.y()-(float)pos.y() );

	QVector3D v1;
	QVector3D v2;
	QVector3D v3;

	if( posF.x() + posF.y() < 1.0f )
	{
		v1 = getVertexPosition( pos.x(), pos.y() );
		v2 = getVertexPosition( pos.x(), pos.y()+1 );
		v3 = getVertexPosition( pos.x()+1, pos.y() );
	} else {
		v1 = getVertexPosition( pos.x()+1, pos.y()+1 );
		v2 = getVertexPosition( pos.x()+1, pos.y() );
		v3 = getVertexPosition( pos.x(), pos.y()+1 );
	}

	QVector3D normal = QVector3D::normal( v1, v2, v3 );
	return ( (normal.x()*(position.x()-v1.x())) + (normal.z()*(position.z()-v1.z())) ) / ( -normal.y() ) + v1.y();
}
