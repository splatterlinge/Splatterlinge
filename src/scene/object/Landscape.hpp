#ifndef SCENE_OBJECT_LANDSCAPE_INCLUDED
#define SCENE_OBJECT_LANDSCAPE_INCLUDED

#include "AObject.hpp"
#include "geometry/Terrain.hpp"

#include <QString>
#include <QRect>
#include <QSizeF>
#include <QVector2D>
#include <QDebug>


class GLWidget;
class Shader;
class TextureRenderer;
class Material;


/// A Landscape consisting of terrain and water
/**
 * 
 */
class Landscape : public AObject
{
public:
	class Blob
	{
		GLWidget * mGLWidget;
		Landscape * mLandscape;
		Material * mMaterial;
		QVector2D mMaterialScale;
		QString mBlobMapPath;
		GLuint mBlobMap;
		QRect mRect;
	public:
		Blob( Landscape * landscape, QRect rect, QString material, QVector2D materialScale, QString maskPath );
		~Blob();
		void drawPatchMap( const QRect & visible );
		void drawPatch( const QRectF & visible )
			{ drawPatchMap( mLandscape->terrain()->toMap(visible) ); }
	};

	Landscape( Scene * scene, QString rootDir );
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
	TextureRenderer * mRefractionRenderer;
	float mWaterClippingPlaneOffset;
	
	void renderReflection();
	void renderRefraction();
};


#endif
