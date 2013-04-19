#ifndef TERRAIN_INCLUDED
#define TERRAIN_INCLUDED


#include <QString>
#include <QVector>
#include <QVector3D>
#include <QGLBuffer>


class Terrain
{
public:
	Terrain( QString terrainPath, float heightFactor );
	void draw();
	const QVector3D & getVertexPosition( int x, int y ) const { return mVertices[x+y*mWidth]; }
	QVector3D & getVertexPosition( int x, int y ) { return mVertices[x+y*mWidth]; }

protected:

private:
	int mWidth;
	int mHeight;
	int mHeightFactor;
	QVector<QVector3D> mVertices;
	QVector<unsigned int> mIndices;
	QGLBuffer mIndexBuffer;
	QGLBuffer mVertexBuffer;
};


#endif
