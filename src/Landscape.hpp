#ifndef LANDSCAPE_INCLUDED
#define LANDSCAPE_INCLUDED


#include <QString>
#include <QRect>
#include <QSizeF>
#include <QDebug>

#include "Terrain.hpp"
#include "Material.hpp"


class Landscape
{
public:
	class Blob
	{
		Landscape * mLandscape;
		Material * mMaterial;
		QVector2D mMaterialScale;
		QString mMaskPath;
		QRect mRect;
	public:
		Blob( Landscape * landscape, QRect rect, QString material, QVector2D materialScale, QString maskPath );
		~Blob();
		void draw();
		void drawPatchMap( const QRect & visible );
		void drawPatch( const QRectF & visible )
			{ drawPatchMap( mLandscape->getTerrain()->toMap(visible) ); }
	};

	Landscape( QGLWidget * glWidget, QString rootDir );
	~Landscape();

	void draw();
	void drawPatchMap( const QRect & visible );
	void drawPatch( const QRectF & visible )
		{ drawPatchMap( mTerrain->toMap(visible) ); }

	Terrain * getTerrain() { return mTerrain; }
	const Terrain * getTerrain() const { return mTerrain; }
	QGLWidget * getGLWidget() { return mGLWidget; }

private:
	QString mName;
	QGLWidget * mGLWidget;
	QVector<Blob*> mBlobs;
	Terrain * mTerrain;
	Material * mTerrainMaterial;
	QVector3D mTerrainSize;
	QVector3D mTerrainOffset;
	QVector2D mTerrainMaterialScale;
};


#endif
