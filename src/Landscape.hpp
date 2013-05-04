#ifndef LANDSCAPE_INCLUDED
#define LANDSCAPE_INCLUDED


#include <QString>
#include <QRect>
#include <QSizeF>
#include <QDebug>

#include "GLWidget.hpp"
#include "Terrain.hpp"
#include "Material.hpp"
#include "Object3D.hpp"


class GLWidget;
class Shader;
class TextureRenderer;


class Landscape : public Object3D
{
public:
	class Blob
	{
		GLWidget * mGLWidget;
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
			{ drawPatchMap( mLandscape->terrain()->toMap(visible) ); }
	};

	Landscape( GLScene * scene, QString rootDir );
	~Landscape();

	virtual void updateSelf( const float & delta );
	virtual void drawSelf();
	virtual void drawSelfPost();
	virtual void drawSelfPostProc();

	Terrain * terrain() { return mTerrain; }
	const Terrain * terrain() const { return mTerrain; }

private:
	QString mName;
	QVector<Blob*> mBlobs;
	Terrain * mTerrain;
	Material * mTerrainMaterial;
	QVector3D mTerrainSize;
	QVector3D mTerrainOffset;
	QVector2D mTerrainMaterialScale;

	float mWaterHeight;
	Shader * mWaterShader;
	TextureRenderer * mReflectionRenderer;
};


#endif
