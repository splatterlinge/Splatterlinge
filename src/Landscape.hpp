#ifndef LANDSCAPE_INCLUDED
#define LANDSCAPE_INCLUDED


#include <QString>
#include <QRect>
#include <QDebug>

#include "Terrain.hpp"
#include "Material.hpp"


class Landscape
{
public:
	class Blob
	{
	public:
		Blob( QRect rect, QString material ) { qDebug() << "Blob:" << rect << material; }
	};

	Landscape( QGLWidget * glWidget, QString rootDir );
	~Landscape();

	void draw();

	Terrain * getTerrain() { return mTerrain; }
	const Terrain * getTerrain() const { return mTerrain; }

private:
	Terrain * mTerrain;
	Material * mTerrainMaterial;
	QVector3D mTerrainSize;
	QVector3D mTerrainOffset;
	QVector2D mTerrainMaterialScale;
};


#endif
