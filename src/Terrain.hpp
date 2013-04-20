#ifndef TERRAIN_INCLUDED
#define TERRAIN_INCLUDED


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
#include <QGLShaderProgram>


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

	QPoint toMap( const QPointF & point ) const;	///< Converts a point in world coordinates to heightmap coordinates.
	QSize toMap( const QSizeF & size ) const;	///< Converts a size in world coordinates to heightmap coordinates.
	QRect toMap( const QRectF & rect ) const;	///< Converts a rectangle in world coordinates to heightmap coordinates.

	const QSize & mapSize() const { return mMapSize; }	///< The size of the heightmap.
	const QVector3D & size() const { return mSize; }	///< The size of the terrain.
	const QVector3D & offset() const { return mOffset; }	///< The offset of the terrain.
	
	/// Direct access to the vertex at heightmap coordinates.
	const QVector3D & getVertexPosition( int x, int y ) const
		{ return mVertices[x+y*mMapSize.width()]; }

protected:

private:
	QSize mMapSize;
	QVector3D mOffset;
	QVector3D mSize;
	QVector<QVector3D> mVertices;
	QVector<unsigned int> mIndices;
	QGLBuffer mIndexBuffer;
	QGLBuffer mVertexBuffer;
};


#endif
