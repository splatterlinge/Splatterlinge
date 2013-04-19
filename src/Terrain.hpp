#ifndef TERRAIN_INCLUDED
#define TERRAIN_INCLUDED


#include <QString>
#include <QVector>
#include <QVector3D>
#include <QGLBuffer>


class Terrain
{
public:
	Terrain( QString terrainPath, QVector3D scale = QVector3D(1,1,1), QVector3D offset = QVector3D(0,0,0) );

	void draw();
	
	void drawPatch( const QRectF & rect )
		{ drawPatchMap( toMap(rect) ); }

	void drawPatch( float x, float z, float rangeX, float rangeZ )
		{ drawPatchMap( toMap( QRectF( x, z, rangeX, rangeZ ) ) ); }

	void drawPatchMap( const QRect & rect )
		{ drawPatchMap( rect.x(), rect.y(), rect.width(), rect.height() ); }

	void drawPatchMap( int x, int y, int width, int height );

	QPoint toMap( const QPointF & point ) const;
	QSize toMap( const QSizeF & size ) const;
	QRect toMap( const QRectF & rect ) const;

	const QSize & mapSize() const { return mMapSize; }

	const QVector3D & scale() const { return mScale; }
	const QVector3D & offset() const { return mOffset; }
	
	const QVector3D & getVertexPosition( int x, int y ) const
		{ return mVertices[x+y*mMapSize.width()]; }

protected:

private:
	QSize mMapSize;
	QVector3D mOffset;
	QVector3D mScale;
	QVector<QVector3D> mVertices;
	QVector<unsigned int> mIndices;
	QGLBuffer mIndexBuffer;
	QGLBuffer mVertexBuffer;
};


#endif
