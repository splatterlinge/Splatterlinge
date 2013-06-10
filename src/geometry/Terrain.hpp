#ifndef GEOMETRY_TERRAIN_INCLUDED
#define GEOMETRY_TERRAIN_INCLUDED

#include <GLWidget.hpp>
#include <utility/Triangle.hpp>

#include <QString>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSize>
#include <QSizeF>
#include <QVector>
#include <QVector3D>
#include <QGLBuffer>

#include <math.h>


/// Generates and draws a mesh based on a heightmap.
/**
 * A terrain is a grid of vertices that lies within the X/Z plane.\n
 * The vertice's height is read from a heightmap.\n
 * The heightmap's resolution also defines the grid's resolution and can be of any size.\n
 */
class Terrain
{
public:
	/// Creates a new terrain from a heightmap.
	/**
	 * @param heightMapPath The path to an image file used as heightmap. This should be a monochrome image.
	 * @param size The volume occupied by this terrain.
	 * @param offset Where to put the origin of the terrain.
	 */
	Terrain( QString heightMapPath, QVector3D size = QVector3D(1,1,1), QVector3D offset = QVector3D(0,0,0) );

	~Terrain();

	/// Draws the complete terrain.
	/**
	 * The complete terrain is rendered using VBOs.
	 */
	void draw();

	/// Draws the terrain within a rectangle in world coordinates.
	/**
	 * The terrain is rendered using VBOs.
	 * @param x Mnimum X-coordinate of the rectangle in world coordinates.
	 * @param z Mnimum Z-coordinate of the rectangle in world coordinates.
	 * @param rangeX Width of the rectangle in X-direction using world coordinates.
	 * @param rangeZ Width of the rectangle in Z-direction using world coordinates.
	 */
	void drawPatch( float x, float z, float rangeX, float rangeZ )
		{ drawPatchMap( toMap( QRectF( x, z, rangeX, rangeZ ) ) ); }

	/// Draws the terrain within a rectangle in world coordinates.
	/**
	 * The terrain is rendered using VBOs.
	 * @param rect The rectangle to draw in world coordinates.
	 */
	void drawPatch( const QRectF & rect )
		{ drawPatchMap( toMap(rect) ); }

	/// Draws the terrain within a rectangle in heightmap coordinates.
	/**
	 * The terrain is rendered using VBOs.
	 * @param x Mnimum X-coordinate of the rectangle in heightmap coordinates.
	 * @param y Mnimum Y-coordinate of the rectangle in heightmap coordinates.
	 * @param width Width of the rectangle in heightmap coordinates.
	 * @param height Height of the rectangle in heightmap coordinates.
	 */
	void drawPatchMap( int x, int y, int width, int height )
		{ drawPatchMap( QRect( x, y, width, height ) ); }

	/// Draws the terrain within a rectangle in heightmap coordinates.
	/**
	 * The terrain is rendered using VBOs.
	 * @param rect The rectangle to draw in heightmap coordinates.
	 */
	void drawPatchMap( const QRect & rect );

	QPointF toMapF( const QVector3D & point ) const;	///< Converts a vector in world coordinates to heightmap coordinates.
	QPointF toMapF( const QPointF & point ) const;	///< Converts a point in world coordinates to heightmap coordinates.
	QSizeF toMapF( const QSizeF & size ) const;	///< Converts a size in world coordinates to heightmap coordinates.
	QRectF toMapF( const QRectF & rect ) const;	///< Converts a point in world coordinates to heightmap coordinates.
	QPoint toMap( const QVector3D & point ) const;	///< Converts a vector in world coordinates to heightmap coordinates.
	QPoint toMap( const QPointF & point ) const;	///< Converts a point in world coordinates to heightmap coordinates.
	QSize toMap( const QSizeF & size ) const;	///< Converts a size in world coordinates to heightmap coordinates.
	QRect toMap( const QRectF & rect ) const;	///< Converts a rectangle in world coordinates to heightmap coordinates.

	const QSize & mapSize() const { return mMapSize; }	///< The size of the heightmap.
	const QVector3D & size() const { return mSize; }	///< The size of the terrain.
	const QVector3D & offset() const { return mOffset; }	///< The offset of the terrain.

	const QVector3D & getVertexPosition( const int & x, const int & y ) const;	///< The vertex at heightmap coordinates.
	const QVector3D & getVertexPosition( const QPoint & p ) const;			///< The vertex at heightmap coordinates.
	const QVector3D & getVertexNormal( const int & x, const int & y ) const;	///< The normal at heightmap coordinates.
	const QVector3D & getVertexNormal( const QPoint & p ) const;			///< The normal at heightmap coordinates.

	QQuaternion getNormalRotation( const QVector3D & position, const QVector3D & from = QVector3D(0,1,0) ) const
		{ return getNormalRotation( QPointF( position.x(), position.z() ), from ); }
	QQuaternion getNormalRotation( const QPointF & position, const QVector3D & from = QVector3D(0,1,0) ) const;

	bool getTriangle( const QVector3D & position, Triangle & t ) const;
	bool getTriangle( const QPointF & position, Triangle & t ) const;
	Triangle getTriangle( const QVector3D & position ) const;
	Triangle getTriangle( const QPointF & position ) const;
	bool getNormal( const QVector3D & position, QVector3D & normal ) const;
	bool getNormal( const QPointF & position, QVector3D & normal ) const;
	QVector3D getNormal( const QVector3D & position ) const;
	QVector3D getNormal( const QPointF & position ) const;
	bool getHeight( const QVector3D & position, float & height ) const;
	bool getHeight( const QPointF & position, float & height ) const;
	float getHeight( const QVector3D & position ) const;
	float getHeight( const QPointF & position ) const;

	/// Calculates the intersection distance to the terrain. length is used as input and output.
	bool getLineIntersection( const QVector3D & origin, const QVector3D & direction, float & lengthl ) const;

	/// Calculates the intersection distance to the terrain. length is used as input and output.
	bool getLineIntersection( const QVector3D & origin, const QVector3D & direction, float & length, QVector3D & normal ) const;

protected:

private:
	class Vertex
	{
	public:
		Vertex() : position(), normal(0,1,0), texCoord() {}
		QVector3D position;
		QVector3D normal;
		QVector2D texCoord;
	};
	Vertex & vertex( const int & x, const int & y );
	Vertex & vertex( const QPoint & p );

	bool getLineQuadIntersection( const QVector3D & origin, const QVector3D & direction, const QPoint & quadMapCoord, float & length ) const;

	QSize mMapSize;
	QVector3D mOffset;
	QVector3D mSize;
	QVector<Vertex> mVertices;
	QGLBuffer mIndexBuffer;
	QGLBuffer mVertexBuffer;
	QSizeF mToMapFactor;
};


inline QPointF Terrain::toMapF( const QVector3D & point ) const
{
	return QPointF(
		(point.x()-(float)mOffset.x()) * mToMapFactor.width(),
		(point.z()-(float)mOffset.z()) * mToMapFactor.height()
	);
}


inline QPointF Terrain::toMapF( const QPointF & point ) const
{
	return QPointF(
		(point.x()-(float)mOffset.x()) * mToMapFactor.width(),
		(point.y()-(float)mOffset.z()) * mToMapFactor.height()
	);
}


inline QSizeF Terrain::toMapF( const QSizeF & size ) const
{
	return QSizeF(
		size.width() * mToMapFactor.width(),
		size.height() * mToMapFactor.height()
	);
}


inline QRectF Terrain::toMapF( const QRectF & rect ) const
{
	return QRectF(
		toMapF(rect.topLeft()),
		toMapF(rect.size())
	);
}


inline QPoint Terrain::toMap( const QVector3D & point ) const
{
	return QPoint(
		floorf( (point.x()-mOffset.x()) * mToMapFactor.width() ),
		floorf( (point.z()-mOffset.z()) * mToMapFactor.height() )
	);
}


inline QPoint Terrain::toMap( const QPointF & point ) const
{
	return QPoint(
		floorf( (point.x()-mOffset.x()) * mToMapFactor.width() ),
		floorf( (point.y()-mOffset.z()) * mToMapFactor.height() )
	);
}


inline QSize Terrain::toMap( const QSizeF & size ) const
{
	return QSize(
		ceilf( size.width() * mToMapFactor.width() ),
		ceilf( size.height() * mToMapFactor.height() )
	);
}


inline QRect Terrain::toMap( const QRectF & rect ) const
{
	return QRect(
		toMap(rect.topLeft()),
		toMap(rect.size())
	);
}


inline bool Terrain::getTriangle( const QVector3D & position, Triangle & t ) const
{
	return getTriangle( QPointF(position.x(),position.z()), t );
}


inline Triangle Terrain::getTriangle( const QVector3D & position ) const
{
	return getTriangle( QPointF(position.x(),position.z()) );
}


inline bool Terrain::getNormal( const QVector3D & position, QVector3D & normal ) const
{
	return getNormal( QPointF(position.x(),position.z()), normal );
}


inline QVector3D Terrain::getNormal( const QVector3D & position ) const
{
	return getNormal( QPointF(position.x(),position.z()) );
}


inline bool Terrain::getHeight( const QVector3D & position, float & height ) const
{
	return getHeight( QPointF(position.x(),position.z()), height );
}


inline float Terrain::getHeight( const QVector3D & position ) const
{
	return getHeight( QPointF(position.x(),position.z()) );
}


inline const QVector3D & Terrain::getVertexPosition( const int & x, const int & y ) const
{
	return mVertices[x+y*mMapSize.width()].position;
}


inline const QVector3D & Terrain::getVertexPosition( const QPoint & p ) const
{
	return getVertexPosition( p.x(), p.y() );
}


inline const QVector3D & Terrain::getVertexNormal( const int & x, const int & y ) const
{
	return mVertices[x+y*mMapSize.width()].normal;
}


inline const QVector3D & Terrain::getVertexNormal( const QPoint & p ) const
{
	return getVertexNormal( p.x(), p.y() );
}


inline Terrain::Vertex & Terrain::vertex( const int & x, const int & y )
{
	return mVertices[x+y*mMapSize.width()];
}


inline Terrain::Vertex & Terrain::vertex( const QPoint & p )
{
	return vertex( p.x(), p.y() );
}


#endif
