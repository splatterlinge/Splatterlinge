#include "Terrain.hpp"

#include <QImage>
#include <QDebug>


Terrain::Terrain( QString terrainPath, float heightFactor )
{
	QImage heightMap( terrainPath+"height.png" );
	mWidth = heightMap.width();
	mHeight = heightMap.height();
	mHeightFactor = heightFactor;
	
	mVertices.clear();
	// positions
	QVector<QVector3D> rawPositions;
	for( int h=0; h<heightMap.height(); h++ )
	{
		for( int w=0; w<heightMap.width(); w++ )
		{
			rawPositions.push_back( QVector3D( w, (float)qRed( heightMap.pixel( w, h ) )*(heightFactor/256.0), h ) );
		}
	}
#define rawPosition(x,y) (rawPositions[(x)+(y)*mWidth])
	for( int w=0; w<heightMap.width(); w++ )
		mVertices.push_back( rawPosition( w, 0 ) );
	for( int h=1; h<heightMap.height()-1; h++ )
	{
		mVertices.push_back( rawPosition( 0, h ) );
		for( int w=1; w<heightMap.width()-1; w++ )
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
		mVertices.push_back( rawPosition( mWidth-1, h ) );
	}
	for( int w=0; w<heightMap.width(); w++ )
		mVertices.push_back( rawPosition( w, mHeight-1 ) );
#undef rawPosition
	// normals
	for( int h=0; h<heightMap.height()-1; h++ )
	{
		for( int w=0; w<heightMap.width()-1; w++ )
		{
			mVertices.push_back( QVector3D::normal( getVertexPosition(w,h), getVertexPosition(w,h+1), getVertexPosition(w+1,h) ) );
		}
		mVertices.push_back( QVector3D(0,1,0) );	// last vertex in row
	}
	for( int w=0; w<heightMap.width(); w++ )	// last row
	{
		mVertices.push_back( QVector3D(0,1,0) );
	}
	// texture coordinates
	for( int h=0; h<heightMap.height(); h++ )
	{
		for( int w=0; w<heightMap.width(); w++ )
		{
//			mVertices.push_back( QVector3D( (float)w/(float)mWidth, (float)h/(float)mHeight, 0 ) );
			mVertices.push_back( QVector3D( (float)w/(float)8, (float)h/(float)8, 0 ) );
		}
	}
	mVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
	mVertexBuffer.create();
	mVertexBuffer.bind();
	mVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mVertexBuffer.allocate( mVertices.data(), mVertices.size()*sizeof(QVector3D) );

	mIndices.clear();
	for( int h=0; h<heightMap.height()-1; h++ )
	{
		for( int w=0; w<heightMap.width(); w++ )
		{
			mIndices.push_back( w + h*mWidth );
			mIndices.push_back( w + (h+1)*mWidth );
		}
	}
	mIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
	mIndexBuffer.create();
	mIndexBuffer.bind();
	mIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mIndexBuffer.allocate( mIndices.data(), mIndices.size()*sizeof(unsigned int) );
}


void Terrain::draw()
{
	glPushMatrix();
	glTranslatef( -mWidth/2.0f, -mHeightFactor/2.0f, -mHeight/2.0f );
	mVertexBuffer.bind();
	mIndexBuffer.bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer( 3, GL_FLOAT, 0, 0 );
	glNormalPointer( GL_FLOAT, 0, (void*)((size_t)mWidth*mHeight*sizeof(QVector3D)) );
	glTexCoordPointer( 3, GL_FLOAT, 0, (void*)((size_t)mWidth*mHeight*sizeof(QVector3D)*2) );
	for( int h=0; h<mHeight-1; h++ )
	{
		glDrawElements( GL_TRIANGLE_STRIP, mWidth*2, GL_UNSIGNED_INT, (void*)((size_t)(mWidth*2*sizeof(unsigned int)*h)) );
//		glDrawElements( GL_LINE_STRIP, mWidth*2, GL_UNSIGNED_INT, (void*)((size_t)(mWidth*2*sizeof(unsigned int)*h)) );
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	mVertexBuffer.release();
	mIndexBuffer.release();
	glPopMatrix();
}
