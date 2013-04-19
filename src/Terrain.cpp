#include "Terrain.hpp"

#include <QImage>
#include <QDebug>


Terrain::Terrain( QString terrainPath, QVector3D scale, QVector3D offset )
{
	QImage heightMap( terrainPath+"/height.png" );
	mMapSize = heightMap.size();
	mScale = scale;
	mOffset = offset;
	mVertices.clear();
	mIndices.clear();

	// prepare positions
	QVector<QVector3D> rawPositions;
	for( int h=0; h<mMapSize.height(); h++ )
	{
		for( int w=0; w<mMapSize.width(); w++ )
		{
			rawPositions.push_back( offset + QVector3D(
				w*(mScale.x()/mMapSize.width()),
				(float)qRed( heightMap.pixel( w, h ) )*(mScale.y()/256.0),
				h*(mScale.z()/mMapSize.height())
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
			mVertices.push_back( QVector3D( (float)w/(float)mMapSize.width(), (float)h/(float)mMapSize.height(), 0 ) );
		}
	}
	mVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
	mVertexBuffer.create();
	mVertexBuffer.bind();
	mVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mVertexBuffer.allocate( mVertices.data(), mVertices.size()*sizeof(QVector3D) );

	// indices
	for( int h=0; h<mMapSize.height()-1; h++ )
	{
		for( int w=0; w<mMapSize.width(); w++ )
		{
			mIndices.push_back( w + h*mMapSize.width() );
			mIndices.push_back( w + (h+1)*mMapSize.width() );
		}
	}
	mIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
	mIndexBuffer.create();
	mIndexBuffer.bind();
	mIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mIndexBuffer.allocate( mIndices.data(), mIndices.size()*sizeof(unsigned int) );
}


QPoint Terrain::toMap( const QPointF & point ) const
{
	return QPoint(
		(point.x()-mOffset.x()) * (mMapSize.width()/mScale.x()),
		(point.y()-mOffset.z()) * (mMapSize.height()/mScale.z())
	);
}


QSize Terrain::toMap( const QSizeF & size ) const
{
	return QSize(
		size.width() * (mMapSize.width()/mScale.x()),
		size.height() * (mMapSize.height()/mScale.z())
	);
}


QRect Terrain::toMap( const QRectF & rect ) const
{
	return QRect(
		toMap(rect.topLeft()),
		toMap(rect.size())
	);
}


void Terrain::drawPatchMap( int x, int y, int width, int height )
{
	int w = x;
	if( w < 0 )
	{
		width += w;
		w = 0;
	}
	if( w >= mMapSize.width() )
		w = mMapSize.width()-1;
	if( w+width > mMapSize.width() )
		width = mMapSize.width()-w;
	if( width <= 0 )
		return;

	mVertexBuffer.bind();
	mIndexBuffer.bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer( 3, GL_FLOAT, 0, 0 );
	glNormalPointer( GL_FLOAT, 0, (void*)((size_t)mMapSize.width()*mMapSize.height()*sizeof(QVector3D)) );
	glTexCoordPointer( 3, GL_FLOAT, 0, (void*)((size_t)mMapSize.width()*mMapSize.height()*sizeof(QVector3D)*2) );
	for( int h=y; h<y+height; h++ )
	{
		if( h<0 || h>=mMapSize.height()-1 )
			continue;
		glDrawElements( GL_TRIANGLE_STRIP, width*2, GL_UNSIGNED_INT, (void*)((size_t)(w*2*sizeof(unsigned int)+mMapSize.width()*2*sizeof(unsigned int)*h)) );
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
	for( int h=0; h<mMapSize.height()-1; h++ )
	{
		glDrawElements( GL_TRIANGLE_STRIP, mMapSize.width()*2, GL_UNSIGNED_INT, (void*)((size_t)(mMapSize.width()*2*sizeof(unsigned int)*h)) );
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	mVertexBuffer.release();
	mIndexBuffer.release();
}
